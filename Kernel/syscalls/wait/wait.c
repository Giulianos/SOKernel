#include <stdint.h>
#include "../../Scheduler/process.h"

int syscall_wait(uint64_t pid)
{
    waitProcess(currentProc(), pid);
    return 1;
}
