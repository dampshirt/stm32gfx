#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

#define PIN_LED         LL_GPIO_PIN_13
#define PORT_LED        GPIOC

#define PIN_UART_TX     LL_GPIO_PIN_6
#define PORT_UART_TX    GPIOB

#define PIN_RES         LL_GPIO_PIN_4
#define PORT_RES        GPIOA
#define PIN_CS          LL_GPIO_PIN_10
#define PORT_CS         GPIOA
#define PIN_DC          LL_GPIO_PIN_11
#define PORT_DC         GPIOA

#endif