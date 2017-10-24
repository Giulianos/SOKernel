#include "sbrk.h"
#include "../../PageAllocator/pageAllocator.h"

uint64_t syscall_sbrk(){
  return allocatePage();
}
