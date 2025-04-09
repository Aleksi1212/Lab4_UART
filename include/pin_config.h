#include "pico/stdlib.h"

#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

void init_input_pin(uint gpio);
void init_uart_pin(uart_inst_t *uart, uint tx_pin, uint rx_pin);

#endif