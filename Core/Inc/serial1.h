#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"



extern UART_HandleTypeDef huart1;



extern void serial1_begin(uint32_t baud);
extern uint8_t serial1_available();
extern uint8_t serial1_read();
extern void serial1_print(const char *str);
extern void serial1_println(const char *str);
extern void serial1_print_itoa(uint16_t ints);
extern void serial1_putchar(char ch);



#ifdef __cplusplus
}
#endif
#endif
