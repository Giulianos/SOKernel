#ifndef BLOCKED_QUEUE_LIST_H
#define BLOCKED_QUEUE_LIST_H

  #include "../thread_queue/thread_queue.h"

  /* Returns a blocked queue id which is then used to access the queue */
  int new_blocked_queue();
  /* Gets a blocked queue from the list of queues */
  thread_queue_t get_blocked_queue(int queue_id);
  /* Removes a blocked queue from the list of queues */
  int remove_blocked_queue(int queue_id);

#endif
