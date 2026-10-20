#ifndef FREEEEG32_RUNTIME_H
#define FREEEEG32_RUNTIME_H
#include <stdint.h>
uint32_t runtime_init(void); /* Reset flags; initializes a 1 MHz timer that runs in Sleep. */
uint32_t runtime_micros(void);
void runtime_sleep(void); /* Ordinary WFI Sleep only; caller masks IRQs and checks work first. */
int runtime_watchdog_start(void); /* IWDG1, nominal 32.768 s; LSI tolerance applies. */
void runtime_watchdog_feed(void);
#endif
