#ifndef UART_H
#define UART_H

bool write_uart(uart_inst_t *uart, const uint8_t *message, int *state);
void read_uart(uart_inst_t *uart, int *state);

#endif