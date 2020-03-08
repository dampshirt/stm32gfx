#include "gpio.h"
#include "main.h"

void initGPIO(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

    LL_GPIO_ResetOutputPin(PORT_LED, PIN_LED);
    LL_GPIO_ResetOutputPin(PORT_RES, PIN_RES | PIN_CS | PIN_DC);

    GPIO_InitStruct.Pin = PIN_LED;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(PORT_LED, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_UART_TX;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(PORT_UART_TX, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_RES | PIN_CS | PIN_DC;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(PORT_RES, &GPIO_InitStruct);
}
