#ifndef UART_H
#define UART_H

bool write_uart(uart_inst_t *uart, const uint8_t *message);
void read_at_command(uart_inst_t *uart, int *step);
void read_ver_command(uart_inst_t *uart, int *step);
void read_deveui_command(uart_inst_t *uart, int *step);

#endif