#include "pico/stdlib.h"

#include "pin_config.h"
#include "constants.h"

void init_input_pin(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

void init_uart_pin(uart_inst_t *uart, uint tx_pin, uint rx_pin)
{
    uart_init(uart, BAUD_RATE);

    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
}