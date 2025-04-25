#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "constants.h"
#include "pin_config.h"
#include "uart.h"
#include "str_utils.h"

int main(void) {
    stdio_init_all();

    clear_terminal();

    init_input_pin(SW_0);
    init_uart_pin(uart1, UART_TX_PIN, UART_RX_PIN);

    int step = 1;

    bool pressed = false;
    bool write = true;

    const uint8_t AT_command[] = "AT\n";
    const uint8_t VER_command[] = "AT+VER\n";
    const uint8_t DevEui_command[] = "AT+ID=DevEui\n";

    while (true)
    {
        switch (step)
        {
        case 1:
            if (pressed && gpio_get(SW_0) == 0) {
                pressed = false;
            }
            else if (!pressed && gpio_get(SW_0) > 0) {
                write = !write;
                pressed = true;
                sleep_ms(10);
            }

            if (write) {
                printf("Connecting to LoRa module...\n");
                write = !write_uart(uart1, AT_command);
                step = 2;
            }
            break;
        case 2:
            read_at_command(uart1, &step);
            break;
        case 3:
            write_uart(uart1, VER_command);
            read_ver_command(uart1, &step);
            break;
        case 4:
            write_uart(uart1, DevEui_command);
            read_deveui_command(uart1, &step);
            break;
        case 5:
            step = 1;
            printf("\n");
            break;
        default:
            step = 1;
            printf("\n");
            break;
        }
    }
    

    return 0;
}
