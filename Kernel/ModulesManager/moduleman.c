#include <lib.h>
#include "modules.h"
#include "../PageAllocator/pageAllocator.h"

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

static uint8_t * expandedModulesArea = (uint8_t *)0xA00000; //10MB
static uint8_t * runtimePage = (uint8_t *)12;
static uint8_t * userlandLogicPage = (uint8_t *)0x1FE00000;

static uint8_t expandedModulesQuantity __attribute__ ((section (".data"))); //BSS will be cleared, so let's put it in data

void loadModulesToKernel();
static void loadModuleToKernel(uint8_t ** module, uint8_t ** targetModuleAddress, uint8_t moduleNum);
void loadModuleToRun(uint8_t id);
static uint32_t readUint32(uint8_t ** address);


/* Loads modules  */
void loadModulesToKernel()
{
  uint8_t * currentModule = (uint8_t *)(&endOfKernelBinary);
  //Read how many modules were packed
  uint32_t modulesQuantity = readUint32(&currentModule);
  uint8_t * moduleDest = expandedModulesArea;
  uint8_t i;
  for(i=0; i<modulesQuantity; i++)
  {
    loadModuleToKernel(&currentModule, &moduleDest, i);
  }
  expandedModulesQuantity=modulesQuantity;
	userlandLogicPage = (uint8_t *)getLogicalUserlandPage();
}

uint8_t getModulesQuantity()
{
  return expandedModulesQuantity;
}

void changeRuntimePage(uint8_t * pageDir)
{
  runtimePage = pageDir;
}

module_t * getModules()
{
  return modules;
}

void loadModuleToKernel(uint8_t ** module, uint8_t ** targetModuleAddress, uint8_t moduleNum)
{
	uint32_t moduleSize = readUint32(module);
  modules[moduleNum].id = moduleNum;
  modules[moduleNum].dir = *targetModuleAddress;
  modules[moduleNum].size = moduleSize;
	memcpy(*targetModuleAddress, *module, moduleSize);
	*module += moduleSize;
  *targetModuleAddress += moduleSize;
}

void loadModule(uint8_t id, uint64_t text_section)
{
  memcpy((void *)text_section, modules[id].dir, modules[id].size);
}

void runLoadedModule()
{
  ((EntryPoint)userlandLogicPage)();
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
