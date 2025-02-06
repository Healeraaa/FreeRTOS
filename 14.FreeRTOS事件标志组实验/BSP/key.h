#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

// KEY1
#define KEY_PORT_1 GPIOA
#define KEY1_PIN GPIO_PIN_0
#define KEY1 LL_GPIO_IsInputPinSet(KEY_PORT_1, KEY1_PIN)

#define KEY_PORT_2 GPIOD
// KEY2
#define KEY2_PIN GPIO_PIN_1
#define KEY2 LL_GPIO_IsInputPinSet(KEY_PORT_2, KEY2_PIN)
// KEY3
#define KEY3_PIN GPIO_PIN_3
#define KEY3 LL_GPIO_IsInputPinSet(KEY_PORT_2, KEY3_PIN)
// KEY4
#define KEY4_PIN GPIO_PIN_5
#define KEY4 LL_GPIO_IsInputPinSet(KEY_PORT_2, KEY4_PIN)
// KEY5
#define KEY5_PIN GPIO_PIN_7
#define KEY5 LL_GPIO_IsInputPinSet(KEY_PORT_2, KEY5_PIN)

void Key_Init(void);
uint8_t Key_GetNum(uint8_t mode);

#endif
