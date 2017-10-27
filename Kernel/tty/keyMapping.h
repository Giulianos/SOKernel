#ifndef _KEY_MAPPING
#define _KEY_MAPPING

  #include "../KeyboardDriver/driver.h"
  #include <stdint.h>

  #define SHIFT_PRESSED 0x01
  #define CAPS_LOCK_ENABLED 0x02
  #define CONTROL_PRESSED 0x04
  #define ALT_PRESSED 0x08
  #define ALT_GR_PRESSED 0x10
  #define RIGHT_SHIFT_PRESSED 0x20

  uint8_t getAsciiUS(keycode_t key, uint8_t state);
  uint8_t updateStateUS(keycode_t key, uint8_t *state);
  uint8_t getAsciiLA(keycode_t key, uint8_t state);
  uint8_t updateStateLA(keycode_t key, uint8_t *state);

  #define getAscii(k, s) getAsciiUS(k, s)
  #define updateState(k, s) updateStateUS(k, s)

#endif
