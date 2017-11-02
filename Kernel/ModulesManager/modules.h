#ifndef _MODULESMAN_MODULES_H
#define _MODULESMAN_MODULES_H

typedef int (*EntryPoint)();
typedef struct
{
  char * name;
  uint8_t id;
  uint8_t * dir;
  uint32_t size;
} module_t;

void loadModulesToKernel();
void loadModule(uint8_t id, void * text_section);
void runLoadedModule();
uint8_t getModulesQuantity();
module_t * getModules();

#endif
