#ifndef _PAGING_H
#define _PAGING_H

void mapUserspace(uint64_t pageAddr);
uint64_t getLogicalUserlandPage();

#endif
