#include "serial1.h"
#include "stdlib.h"

UART_HandleTypeDef huart1;

#define RX_BUFFER_SIZE 250

typedef struct _ring_buffer {
	unsigned char buffer[RX_BUFFER_SIZE];
	int head;
	int tail;
} ring_buffer;
ring_buffer rx_buffer[1] =  { { 0 }, 0, 0 };

void store_char(uint8_t c, ring_buffer *rx_buffer){
	uint8_t i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;
	if (i != rx_buffer->tail) {
		rx_buffer->buffer[rx_buffer->head] = c;
		rx_buffer->head = i;
	}
}

void USART1_IRQHandler(void) {
	if ((huart1.Instance->SR & USART_SR_RXNE) != 0) {
		uint8_t c = huart1.Instance->DR;
		//uint8_t c = USART1->DR;
		store_char(c, &rx_buffer[0]);
	}
}

void serial1_begin(uint32_t baud) {
	HAL_UART_MspInit(&huart1);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = baud;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK){
	  Error_Handler();
	}

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

uint8_t serial1_available(){
	return (RX_BUFFER_SIZE + rx_buffer->head - rx_buffer->tail) % RX_BUFFER_SIZE;
}
uint8_t serial1_read(){
     if (rx_buffer->head == rx_buffer->tail) {
           return -1;
     } else {
           uint8_t c =
                 rx_buffer->buffer[rx_buffer->tail];
           rx_buffer->tail =
                 (rx_buffer->tail + 1) % RX_BUFFER_SIZE;
           return c;
     }
}
void serial1_print(const char *str) {
	while(*str){
    serial1_putchar(*str);
    str++;
  }
}
void serial1_println(const char *str) {
  while(*str){
    serial1_putchar(*str);
    str++;
  }
  serial1_putchar('\r');
  serial1_putchar('\n');
}
void serial1_print_itoa(uint16_t ints) {
	char cid[6];
	const char* str = itoa(ints,cid,10);
	while(*str){
		serial1_putchar(*str);
		str++;
	}
}
void serial1_putchar(char ch){
	while (!(huart1.Instance->SR & USART_SR_TC));
	huart1.Instance->DR = ch;
	//USART1->DR = ch;
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}




