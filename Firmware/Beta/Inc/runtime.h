#ifndef FREEEEG32_RUNTIME_H
#define FREEEEG32_RUNTIME_H
#include <stdint.h>
uint32_t runtime_init(void); /* Reset flags; initializes a 1 MHz timer that runs in Sleep. */
uint32_t runtime_micros(void);
/* Idle WFI is off by default: bench testing (rev V) showed the core staying in WFI with
   enabled USB/EXTI IRQs pending. Build with -DFREEEEG32_IDLE_WFI=1 to re-enable it. */
#ifndef FREEEEG32_IDLE_WFI
#define FREEEEG32_IDLE_WFI 0
#endif
#if FREEEEG32_IDLE_WFI
#define FREEEEG32_IDLE_NAME "WFI"
#else
#define FREEEEG32_IDLE_NAME "spin"
#endif
void runtime_sleep(void); /* Ordinary WFI Sleep (or nothing); caller masks IRQs and checks work first. */
int runtime_watchdog_start(void); /* IWDG1, nominal 32.768 s; LSI tolerance applies. */
void runtime_watchdog_feed(void);
#endif
