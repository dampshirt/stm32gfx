
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "lcd.h"

static void SystemClock_Config(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    initGPIO();
    initSPI();
    initUART();
    initLCD();

    fillScreen(ST7735_BLACK);

    while (1)
    {
        /* tiny squares */
    	fillRect(ST7735_RAND_X, ST7735_RAND_Y, 10, 10, ST7735_SURPRISE);
    	LL_mDelay(30);
    }
}

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

    if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1);
    
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_HSI_Enable();

    while(LL_RCC_HSI_IsReady() != 1);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
    LL_RCC_PLL_Enable();

    while(LL_RCC_PLL_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_SetSystemCoreClock(48000000);
}
