#include <stdlib.h>
#include <lib.h>

#define MSG_QUEUE_IDSTR_LEN 255

static int current_message_queue_id = 0;

typedef struct message * message_t;
typedef struct message_queue * message_queue_t;

struct message_queue
{
  int id;
  char id_str[MSG_QUEUE_IDSTR_LEN];
  message_t first_msg;
  message_t last_msg;
  int msg_quantity;
};

struct message
{
  char * msg;
  size_t len;
  message_t next;
};

message_queue_t new_message_queue(char * id_str)
{
  message_queue_t ret = (message_queue_t)k_malloc(sizeof(struct message_queue));

  ret->id = current_message_queue_id++;
  k_memcpy( (void *)ret->id_str,
            (void *)id_str,
            MSG_QUEUE_IDSTR_LEN );
  ret->msg_quantity = 0;
  ret->first_msg = NULL;
  ret->last_msg = NULL;
  return ret;
}

int write_message_queue(message_queue_t mq, const char * msg, size_t count)
{
  message_t new_msg  = (message_t)k_malloc(sizeof(struct message));

  if(new_msg == NULL) {
    k_log("Couldn't allocate space for message\n");
    return -1;
  }
  new_msg->msg = k_malloc(sizeof(char)*count);
  new_msg->next = NULL;
  new_msg->len = count;
  if(new_msg->msg == NULL) {
    k_log("Couldn't allocate space for message\n");
    k_free(new_msg);
    return -1;
  }
  k_memcpy( (void *)new_msg->msg,
            (void *)msg,
            count );
  if(mq->first_msg == NULL) {
    mq->first_msg = new_msg;
    mq->last_msg = new_msg;
  } else {
    mq->last_msg->next = new_msg;
    mq->last_msg = new_msg;
  }
  mq->msg_quantity++;
  //Check if any process is blocked in the msg queue;
  return 1;
}

int read_message_queue(message_queue_t mq, char * buf, size_t count)
{
  message_t aux;
  if(mq->first_msg == NULL)
    //Here we block the process

  count = (count > mq->first_msg->len) ? mq->first_msg->len : count;
  k_memcpy( (void *)buf,
            (void *)mq->first_msg->msg,
            count );
  k_free(mq->first_msg->msg);
  aux = mq->first_msg;
  mq->first_msg = mq->first_msg->next;
  mq->msg_quantity--;
  k_free(aux);
  return count;
}

const char * get_id_str_message_queue(message_queue_t mq)
{
  return (const char *)mq->id_str;
}

int free_message_queue(message_queue_t mq)
{
  int i = 0;
  message_t aux;

  for(; i < mq->msg_quantity; i++) {
    k_free(mq->first_msg->msg);
    aux = mq->first_msg;
    mq->first_msg = mq->first_msg->next;
    k_free(aux);
  }
  k_free(mq);
  return 1;
}
