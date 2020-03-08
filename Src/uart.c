#include "uart.h"
#include "main.h"

char usartTxBuffer[USART_TX_MAX_SIZE];

void usartPrint(char * buf)
{
	while(*buf != 0)
	{
		LL_USART_TransmitData8(USART1, *buf++);
		while (!LL_USART_IsActiveFlag_TXE(USART1)) {};
	}
}

void initUART(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	LL_GPIO_AF_EnableRemap_USART1();

	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_EVEN;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART1);
	LL_USART_Enable(USART1);
}
