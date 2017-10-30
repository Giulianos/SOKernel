#ifndef _BUFFER_H
#define _BUFFER_H

#define MAX_KB_BUFFER 4096 //dos pantallas, redondeo a 4KB

typedef struct {
  int f; //first character in the buffer
  int l; //last character in the buffer
  char buffer[MAX_KB_BUFFER+1];
} kbBuffer_t;

uint8_t getCharBuffer(kbBuffer_t * b);
void putCharBuffer(kbBuffer_t * b, uint8_t c);
uint8_t bufferIsEmpty(kbBuffer_t * b);
void eraseFromBuffer(kbBuffer_t * b);

#endif
