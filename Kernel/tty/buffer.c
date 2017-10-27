#include <stdint.h>
#include "buffer.h"

void putCharBuffer(kbBuffer_t * b, uint8_t c)
{
  if(b->l == MAX_KB_BUFFER)
  {
    if(b->f == 0)
      return;
    b->buffer[0] = c;
    b->l = 1;
  }
  else
  {
    b->buffer[b->l] = c;
    b->l++;
  }
}

uint8_t getCharBuffer(kbBuffer_t * b)
{
  if(b->f == b->l)
    return 0;
  if(b->f == MAX_KB_BUFFER)
  {
    if(b->l == 0)
    {
      b->f = 0;
      return 0;
    }
    b->f = 1;
  }
  else
    b->f++;
  return b->buffer[b->f-1];
}

uint8_t bufferIsEmpty(kbBuffer_t * b)
{
  return ((b->f == b->l)||(b->f == MAX_KB_BUFFER && b->l == 0))?1:0;
}

void eraseFromBuffer(kbBuffer_t * b)
{
  if(bufferIsEmpty(b))
    return;
  b->l = (b->l==0)?MAX_KB_BUFFER:(b->l-1);
}
