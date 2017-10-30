#ifndef _PAGING_H
#define _PAGING_H

void mapProcess(uint64_t pageAddr);
void mapPhysical(uint64_t logic, uint64_t physical);
uint64_t getLogicalUserlandPage();

#endif
