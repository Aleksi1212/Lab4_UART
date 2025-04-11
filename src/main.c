#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "constants.h"
#include "pin_config.h"
#include "uart.h"

int main(void) {
    stdio_init_all();

    init_input_pin(SW_0);
    init_uart_pin(uart1, UART_TX_PIN, UART_RX_PIN);

    int state = 1;

    bool pressed = false;
    bool write = true;

    const uint8_t AT_message[] = "AT\n";
    const uint8_t VER_message[] = "AT+VER\n";
    const uint8_t DevEui_message[] = "AT+ID=DevEui\n";

    while (true)
    {
        switch (state)
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
                write = !write_uart(uart1, AT_message, &state);
            }
            break;
        
        case 2:
            read_uart(uart1, &state);
            break;
        case 3:
            // Test
            printf("State 3\n");
            state = 1;
            break;
        default:
            break;
        }
    }
    

    return 0;
}
