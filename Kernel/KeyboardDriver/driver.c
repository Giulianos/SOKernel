#include <stdint.h>
#include "../tty/tty.h"
#include "driver.h"

extern void outputB(uint16_t reg, uint8_t data);
extern uint8_t inputB(uint16_t reg);

static uint8_t didOccurFirstInterrupt = 1;

void keyboardDriver()
{
  static uint8_t phase = 0;
  static uint8_t secondB = 0;
  static uint8_t firstB;

  if(!didOccurFirstInterrupt)
  {
    didOccurFirstInterrupt=1;
    return;
  }
  if(phase == 0)
  {
    firstB = inputB(0x60);
    if(firstB == 0xE0)
      phase++;
    else
    {
      keyPressed_tty(scancodeToKeycode(firstB, secondB));
    }
  }
  else
  {
    secondB = inputB(0x60);
    phase = 0;
    keyPressed_tty(scancodeToKeycode(firstB, secondB));
  }
}
