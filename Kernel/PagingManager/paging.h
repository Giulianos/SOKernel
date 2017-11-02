#ifndef _PAGING_H
#define _PAGING_H

void mapProcess(void * pageAddr);
void mapPhysical(void * logic, void * physical);
void * getLogicalUserlandPage();

#endif
