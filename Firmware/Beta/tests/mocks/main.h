#ifndef MOCK_MAIN_H
#define MOCK_MAIN_H
#include <stdint.h>
#define DMA_BUFFER
#define ADC1_DRDY_Pin 1
#define HAL_OK 0
#define HAL_ERROR 1
#define HAL_SAI_STATE_READY 0
#define HAL_UART_STATE_READY 0
typedef struct { int state; } SAI_HandleTypeDef;
typedef struct { int state; } UART_HandleTypeDef;
int HAL_SAI_DMAStop(SAI_HandleTypeDef *sai);
int HAL_SAI_GetState(SAI_HandleTypeDef *sai);
uint32_t HAL_SAI_GetError(SAI_HandleTypeDef *sai);
int HAL_SAI_Receive_DMA(SAI_HandleTypeDef *sai, uint8_t *buffer, uint16_t words);
#define HAL_SAI_ERROR_AFSDET 0x4u
#define HAL_SAI_ERROR_LFSDET 0x8u
#define SAI_IT_AFSDET 0x20u
#define SAI_IT_LFSDET 0x40u
#define SAI_FLAG_AFSDET 0x20u
#define SAI_FLAG_LFSDET 0x40u
#define __HAL_SAI_DISABLE_IT(sai, it) ((void)(sai), (void)(it))
#define __HAL_SAI_CLEAR_FLAG(sai, flag) ((void)(sai), (void)(flag))
int HAL_UART_GetState(UART_HandleTypeDef *uart);
int HAL_UART_Transmit_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, uint16_t size);
uint32_t HAL_GetTick(void);
extern uint32_t SystemCoreClock;
uint32_t HAL_RCC_GetHCLKFreq(void);
uint32_t HAL_GetREVID(void);
void HAL_Delay(uint32_t delay);
#define __DMB() ((void)0)
extern uint32_t test_primask;
void test_set_primask(uint32_t mask);
#define __get_PRIMASK() test_primask
#define __disable_irq() (test_primask = 1)
#define __set_PRIMASK(x) test_set_primask(x)
#define NVIC_SystemReset() ((void)0)
#endif
