#include <stdint.h>
#include "interrupts.h"

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1

extern void outputB(uint16_t reg, uint8_t data);
extern uint8_t inputB(uint16_t reg);
extern void sti();
extern void cli();

/* -- Handlers prototypes -- */
extern void timerTickHandler();
extern void keyboardHandler();
extern void systemCallHandler();
extern void spuriousInt7Handler();
extern void spuriousInt15Handler();

static void writePICMask(uint16_t mask);
static void setIDTEntry(uint16_t entry, uint64_t offset);

static IDTEntry_t* IDT = (IDTEntry_t*)0x0;

void configureInterrupts()
{
  cli();
  //0111 1111 0111 1000
  writePICMask(0x7F78); //Habilito solo las interrupciones de teclado, timer tick y las spurious (7 y 15)
  //initMouse();
  setIDTEntry(0x20, (uint64_t)timerTickHandler); //Handler del timer tick
  setIDTEntry(0x21, (uint64_t)keyboardHandler); //Handler de la interrupcion de teclado
  //setIDTEntry(0x2C, (uint64_t)mouseHandler); //Handler de la interrupcion de mouse
  setIDTEntry(0x80, (uint64_t)systemCallHandler);
  setIDTEntry(0x27, (uint64_t)spuriousInt7Handler);
  setIDTEntry(0x2F, (uint64_t)spuriousInt15Handler);
  sti();//Habilito las interrupciones
}

void setIDTEntry(uint16_t entry, uint64_t offset)
{
  IDT[entry].offset_1 = (uint16_t)(offset&0xffff);
  IDT[entry].offset_2 = (uint16_t)((offset>>16)&0xffff);
  IDT[entry].offset_3 = (uint32_t)((offset>>32)&0xffffffff);
  IDT[entry].selector = 0x08;
  IDT[entry].ist = 0x00;
  IDT[entry].type_attr = 0x8e;
  IDT[entry].zero = 0x00;
}

void writePICMask(uint16_t mask)
{
  outputB(PIC_MASTER_DATA, (uint8_t)(mask&0xFF));
  outputB(PIC_SLAVE_DATA, (uint8_t)((mask>>8)&0xFF));
}
