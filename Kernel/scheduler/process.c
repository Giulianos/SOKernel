#include <stdlib.h>
#include <lib.h>
#include <page_allocator.h>
#include <paging.h>
#include "../ModulesManager/modules.h"
#include "process.h"
#include "thread.h"
#include "thread_queue/thread_queue.h"


extern int get_new_pid_scheduler();

static void free_process_page(void * physical, void * logical);
static void unblock_waiting_threads(process_t process);

process_t create_process(int module, int ppid, int vt_id, int flags)
{
	process_t ret = (process_t)k_malloc(sizeof(struct process));

  ret->pid = get_new_pid_scheduler();
  ret->ppid = ppid;
	ret->vt_id = vt_id;
	ret->code = allocatePage();
  ret->heap = NULL;
	ret->waiting_queue_id = create_blocked_queue();

	loadModule((unsigned char)module, ret->code);

	ret->threads = new_thread_queue();
	create_thread(get_logical_userland_page(), ret);
	add_process_list(get_pl(), ret);
	#ifdef PROCESS_DEBUG_MSG
	k_log("Process with pid:%d has been created!\n", ret->pid);
	#endif

	return ret;
}

process_t clone_process(process_t process, thread_t calling_thread)
{
	process_t ret = (process_t)k_malloc(sizeof(struct process));

	ret->pid = get_new_pid_scheduler();
	ret->ppid = process->pid;
	ret->vt_id = process->vt_id;
	ret->code = allocatePage();
  ret->heap = NULL;
	ret->waiting_queue_id = create_blocked_queue();

	k_memcpy(ret->code, process->code, pageSize());
	ret->threads = new_thread_queue();
	clone_thread(calling_thread, ret);
	add_process_list(get_pl(), ret);
	#ifdef PROCESS_DEBUG_MSG
	k_log("Process with pid:%d has been created!\n", ret->pid);
	#endif

	return ret;
}

void replace_process_image(process_t process, int module)
{
	/* We replace the module loaded */
	loadModule((unsigned char)module, process->code);
	/* Then, freeing the thread-list terminates all process threads */
	free_thread_queue(process->threads);
	/* Finally we create a new thread-list and add a new thread */
	process->threads = new_thread_queue();
	create_thread(get_logical_userland_page(), process);
}

void kill_process(process_t process)
{
	k_log("Killing process %d\n", process->pid);
	free_process_page(process->code, NULL);
	each_pagemap(process->heap, free_process_page);
	unblock_waiting_threads(process);
  //Freeing the thread-list terminates all process threads
	free_thread_queue(process->threads);
	free_blocked_queue(process->waiting_queue_id);
	remove_process_list(get_pl(), process);
  k_free((void *)process);
}

void free_process_page(void * physical, void * logical)
{
	freePage(physical);
}

void * allocate_page_heap_process(process_t process)
{
	void * new_addr;
	void * new_addr_phys;

	if(process == NULL)
		return NULL;

	if(process->heap == NULL)
		new_addr = get_logical_userland_heap_base_page();
	else
		new_addr = (void *)((size_t)get_last_logical(process->heap) + (size_t)pageSize());
	process->heap = add_pagemap(process->heap, new_addr_phys = allocatePage(), new_addr);
	map_physical_reload(new_addr, new_addr_phys);

	return new_addr;
}

void unblock_waiting_threads(process_t process)
{
	while((unblock_from_queue_thread(process->waiting_queue_id, NULL))!=-1) {
		k_log("Unblocking waiting thread!\n");
	}
}

thread_t get_main_thread_process(process_t process)
{
	return peek_thread_queue(process->threads);
}
