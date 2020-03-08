#ifndef _SPI_H_
#define _SPI_H_

#include "main.h"

extern SPI_HandleTypeDef hspi1;

#define PIN_SPI_SCK 	LL_GPIO_PIN_5
#define PIN_SPI_MOSI	LL_GPIO_PIN_7
#define PORT_SPI		GPIOA

void initSPI(void);
#define writeSPI(buff, size) HAL_SPI_Transmit(&hspi1, buff, size, HAL_MAX_DELAY);

#endif
