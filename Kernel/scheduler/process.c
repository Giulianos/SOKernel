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

process_t create_process(int module, int ppid, int vt_id, int flags)
{
	process_t ret = (process_t)k_malloc(sizeof(struct process));

  ret->pid = get_new_pid_scheduler();
  ret->ppid = ppid;
	ret->vt_id = vt_id;
	ret->code = allocatePage();
  ret->heap = NULL;

	loadModule((unsigned char)module, ret->code);

	ret->threads = new_thread_queue();
	create_thread(get_logical_userland_page(), ret);
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

	k_memcpy(ret->code, process->code, pageSize());
	ret->threads = new_thread_queue();
	clone_thread(calling_thread, ret);
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
	free_process_page(process->code, NULL);
	each_pagemap(process->heap, free_process_page);
  //Freeing the thread-list terminates all process threads
	free_thread_queue(process->threads);
  k_free((void *)process);
}

void free_process_page(void * physical, void * logical)
{
	freePage(physical);
}

thread_t get_main_thread_process(process_t process)
{
	return peek_thread_queue(process->threads);
}
