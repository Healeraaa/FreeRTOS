#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

// KEY1
#define KEY1_PORT GPIOA
#define KEY1_PIN GPIO_PIN_0
#define KEY1 LL_GPIO_IsInputPinSet(KEY1_PORT, KEY1_PIN)

void Key_Init(void);
uint8_t Key_GetNum(uint8_t mode);

#endif
