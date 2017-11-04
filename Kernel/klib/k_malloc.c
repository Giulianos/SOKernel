#include <stdlib.h>
#include <lib.h>
#include <page_allocator.h>

#define NALLOC (pageSize()/sizeof(Header))

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

static Header *morecore(size_t nblocks);
void k_free(void *ptr);

void * k_malloc(size_t nbytes)
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

  k_log("(k_malloc) requested a page\n");

  if (nunits < NALLOC) {
    nunits = NALLOC;
  }
  freemem = allocatePage(nunits * sizeof(Header));
  if (freemem == NULL) {
    return NULL;
  }
  insertp = (Header *) freemem;
  insertp->s.size = nunits;
  k_free((void *) (insertp + 1));
  return freep;
}

void k_free(void *ptr)
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
