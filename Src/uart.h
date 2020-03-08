#ifndef _UART_H_
#define _UART_H_

#define PORT_UART_TX    GPIOB
#define PIN_UART_TX     LL_GPIO_PIN_6
#define PORT_UART_RX    GPIOB
#define PIN_UART_RX     LL_GPIO_PIN_7

#define USART_TX_MAX_SIZE 32
#define DEBUG_PRINTF(...) 	sprintf(usartTxBuffer, __VA_ARGS__);\
							usartPrint(usartTxBuffer);

extern char usartTxBuffer[USART_TX_MAX_SIZE]; 

void usartPrint(char * buf);
void initUART(void);

#endif
