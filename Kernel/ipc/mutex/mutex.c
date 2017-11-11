#include "mutex.h"
#include "../../scheduler/scheduler.h"
#include <lib.h>

#define MX_IDSTR_LEN 255

typedef struct mutex * mutex_t;

struct mutex
{
  int id;
  char id_str[MX_IDSTR_LEN];
  int state;
  int blocked_queue_id;
};

static void unblock_thread_callback_mutex(void * info);

static int current_mutex_id = 0;

mutex_t create_mutex(char * id_str)
{
  mutex_t ret = (mutex_t)k_malloc(sizeof(struct mutex));

  ret->id = current_mutex_id++;
  k_strncpy( (void *)ret->id_str,
            (void *)id_str,
            MX_IDSTR_LEN );
  ret->state = 0;
  ret->blocked_queue_id = create_blocked_queue();

  return ret;
}

int lock_mutex(mutex_t mutex)
{
  if(mutex->state == -1) {
    block_thread(current_thread(), mutex->blocked_queue_id, (void *)mutex);
    return 1;
  }
  mutex->state = -1;
  return 1;
}

int unlock_mutex(mutex_t mutex)
{
  mutex->state = 0;
  unblock_from_queue_thread(mutex->blocked_queue_id, unblock_thread_callback_mutex);
  return 1;
}

void unblock_thread_callback_mutex(void * info)
{
  mutex_t mutex = (mutex_t)info;
  /* This callback is executed if there was a blocked process in the queue,
  in that case it blocks the mutex again */
  mutex->state = -1;
}
int get_id_mutex(mutex_t mutex)
{
  return mutex->id;
}

const char * get_id_str_mutex(mutex_t mutex)
{
  return (const char *)mutex->id_str;
}

int free_mutex(mutex_t mutex)
{
  free_blocked_queue(mutex->blocked_queue_id);
  k_free(mutex);
  return 1;
}
