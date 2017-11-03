#include <lib.h>
#include "modules.h"
#include "../PageAllocator/pageAllocator.h"
#include "../PagingManager/paging.h"

static module_t modules[256] __attribute__ ((section (".data"))); //BSS will be cleared, so let's put it in data

extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

/*
*      +--------------------+
*      |       KERNEL       |
*      | MODULES/KERNEL BSS | 0x0000000 - endOfKernel
*      |....................|
*      +--------------------+
*      |   COPIED MODULES   | 0x0A00000 - 0x13FFFFF
*      +--------------------+
*      |    RUNTIME PAGE    | 0x1400000 - 0x15FFFFF
*      +--------------------+
*
*/

static void * expandedModulesArea = (void *)0xA00000; //10MB
static void * runtimePage = (void *)12;
static void * userlandLogicPage = (void *)0x1FE00000;

static uint8_t expandedModulesQuantity __attribute__ ((section (".data"))); //BSS will be cleared, so let's put it in data

void loadModulesToKernel();
static void loadModuleToKernel(void ** module, void ** targetModuleAddress, uint8_t moduleNum);
void loadModuleToRun(uint8_t id);
static uint32_t readUint32(void ** address);


/* Loads modules  */
void loadModulesToKernel()
{
  void * currentModule = (void *)(&endOfKernelBinary);
  //Read how many modules were packed
  uint32_t modulesQuantity = readUint32(&currentModule);
  void * moduleDest = expandedModulesArea;
  uint8_t i;
  for(i=0; i<modulesQuantity; i++)
  {
    loadModuleToKernel(&currentModule, &moduleDest, i);
  }
  expandedModulesQuantity=modulesQuantity;
	userlandLogicPage = getLogicalUserlandPage();
}

uint8_t getModulesQuantity()
{
  return expandedModulesQuantity;
}

void changeRuntimePage(void * pageDir)
{
  runtimePage = pageDir;
}

module_t * getModules()
{
  return modules;
}

void loadModuleToKernel(void ** module, void ** targetModuleAddress, uint8_t moduleNum)
{
	uint32_t moduleSize = readUint32(module);
  modules[moduleNum].id = moduleNum;
  modules[moduleNum].dir = *targetModuleAddress;
  modules[moduleNum].size = moduleSize;
	k_memcpy(*targetModuleAddress, *module, moduleSize);
	*module += moduleSize;
  *targetModuleAddress += moduleSize;
}

void loadModule(uint8_t id, void * text_section)
{
  k_log("Loading module %d to address %x!\n", id, text_section);
  k_memcpy((void *)text_section, modules[id].dir, modules[id].size);
}

void runLoadedModule()
{
  ((EntryPoint)userlandLogicPage)();
}

static uint32_t readUint32(void ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
