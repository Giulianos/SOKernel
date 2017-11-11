#include "semaphore.h"
#include "../../scheduler/scheduler.h"
#include <lib.h>

#define SEM_IDSTR_LEN 255

typedef struct semaphore * semaphore_t;

struct semaphore
{
  int id;
  char id_str[SEM_IDSTR_LEN];
  int count;
  int blocked_queue_id;
};

static void unblock_thread_callback_semaphore(void * info);

static int current_semaphore_id = 0;

semaphore_t create_semaphore(int initial_value, char * id_str)
{
  semaphore_t ret = (semaphore_t)k_malloc(sizeof(struct semaphore));

  ret->id = current_semaphore_id++;
  k_strncpy( (void *)ret->id_str,
            (void *)id_str,
            SEM_IDSTR_LEN );
  ret->count = initial_value;
  ret->blocked_queue_id = create_blocked_queue();

  return ret;
}

int wait_semaphore(semaphore_t semaphore)
{
  semaphore->count--;
  if(semaphore->count < 0) {
    block_thread(current_thread(), semaphore->blocked_queue_id, (void *)semaphore);
  }
  return 1;
}

int signal_semaphore(semaphore_t semaphore)
{
  semaphore->count++;
  unblock_from_queue_thread(semaphore->blocked_queue_id, unblock_thread_callback_semaphore);
  return 1;
}

void unblock_thread_callback_semaphore(void * info)
{
  /*nothing to do when unlocking a blocked thread*/
  return;
}

int get_id_semaphore(semaphore_t semaphore)
{
  return semaphore->id;
}

const char * get_id_str_semaphore(semaphore_t semaphore)
{
  return (const char *)semaphore->id_str;
}

int free_semaphore(semaphore_t semaphore)
{
  free_blocked_queue(semaphore->blocked_queue_id);
  k_free(semaphore);
  return 1;
}
