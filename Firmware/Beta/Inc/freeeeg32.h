#ifndef FREEEEG32_H
#define FREEEEG32_H
#include <stdint.h>
/* USB IRQ only enqueues bytes. All parsing, SPI and replies run in main. */
void FreeEEG32_Receive(const uint8_t *data, uint32_t length);
void FreeEEG32_DataReady(uint16_t pin);
void FreeEEG32_Run(void);
#endif
