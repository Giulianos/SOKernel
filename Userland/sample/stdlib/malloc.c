#include <stdlib.h>
#include <stdint.h>
#include "stdlib.h"

#define PAGE_SIZE 0x200000

#define NALLOC (PAGE_SIZE/sizeof(Header))

typedef long Align;

union header {
  struct {
    union header *next;
    size_t size;
  } s;

  Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

static Header *morecore(size_t nblocks);
static void * sbrk();
void free(void *ptr);

void * malloc(size_t nbytes)
{
  Header *currp;
  Header *prevp;
  size_t nunits;

  nunits = ((nbytes + sizeof(Header) - 1) / sizeof(Header)) + 1;
  if (freep == NULL) {
    base.s.next = &base;
    base.s.size = 0;
    freep = &base;
  }
  prevp = freep;
  currp = prevp->s.next;
  for (; ; prevp = currp, currp = currp->s.next) {
    if (currp->s.size >= nunits) {
      if (currp->s.size == nunits) {
        prevp->s.next = currp->s.next;
      } else {
        currp->s.size -= nunits;
        currp += currp->s.size;
        currp->s.size = nunits;
      }
      freep = prevp;
      return (void *) (currp + 1);
    }
    if (currp == freep) {
      if ((currp = morecore(nunits)) == NULL) {
        return NULL;
      }
    }
  }
}

static Header *morecore(size_t nunits)
{
  void *freemem;
  Header *insertp;

  if (nunits < NALLOC) {
    nunits = NALLOC;
  }
  freemem = sbrk();
  if (freemem == NULL) {
    return NULL;
  }
  insertp = (Header *) freemem;
  insertp->s.size = nunits;
  free((void *) (insertp + 1));
  return freep;
}

void free(void *ptr)
{
  Header *insertp, *currp;

  insertp = ((Header *) ptr) - 1;
  for (currp = freep; !((currp < insertp) && (insertp < currp->s.next)); currp = currp->s.next) {
    if ((currp >= currp->s.next) && ((currp < insertp) || (insertp < currp->s.next))) {
      break;
    }
  }
  if ((insertp + insertp->s.size) == currp->s.next) {
    insertp->s.size += currp->s.next->s.size;
    insertp->s.next = currp->s.next->s.next;
  }
  else {
    insertp->s.next = currp->s.next;
  }
  if ((currp + currp->s.size) == insertp) {
    currp->s.size += insertp->s.size;
    currp->s.next = insertp->s.next;
  }
  else {
    currp->s.next = insertp;
  }
  freep = currp;
}

void * sbrk()
{
  return (void *)systemCall(0x2d, 0, 0, 0, 0, 0);
}
