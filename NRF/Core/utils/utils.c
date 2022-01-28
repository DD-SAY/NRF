#include "utils.h"

#if 0
  #include "tim.h"
  void delay_us(uint16_t us) {
    uint16_t compare = (0XFFFF - us - 5);
    __HAL_TIM_SET_COUNTER(&DELAY_TIM, compare);

    HAL_TIM_Base_Start(&DELAY_TIM);
    while (compare < 0XFFFF - 5) {
      compare = __HAL_TIM_GET_COUNTER(&DELAY_TIM);
    }
    HAL_TIM_Base_Stop(&DELAY_TIM);
  }
#endif

#if 1
  #include "usart.h"
  #ifdef __GNUC__
  int __io_putchar(int ch) {
    HAL_UART_Transmit(&PRINTF_UART, (uint8_t *)&ch, 1, 0xFF);
    return ch;
  }
  #else
  int fputc(int ch, FILE *f) {
          HAL_UART_Transmit(&PRINTF_UART, (uint8_t *)&ch, 1, 0xFF);
          return ch;
      }
  #endif
#endif
