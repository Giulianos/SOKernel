#include <stdlib.h>
#include <lib.h>
#include "../PageAllocator/pageAllocator.h"
#include "../ModulesManager/modules.h"
#include "process.h"
#include "thread.h"
#include "thread_queue/thread_queue.h"

extern int get_new_pid_scheduler();

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
	offer_thread_queue(ret->threads, create_thread(ret->code, ret), NULL);
	#ifdef PROCESS_DEBUG_MSG
	k_log("Process with pid:%d has been created!\n", ret->pid);
	#endif

	return ret;
}

void kill_process(process_t process)
{
  //Threads should be terminated here
  k_free((void *)process);
}

thread_t get_main_thread_process(process_t process)
{
	return peek_thread_queue(process->threads);
}
