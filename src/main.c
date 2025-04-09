#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "constants.h"
#include "pin_config.h"

int main(void) {
    stdio_init_all();

    init_input_pin(SW_0);
    init_uart_pin(uart1, UART_TX_PIN, UART_RX_PIN);

    bool pressed = false;
    bool send = true;

    const uint8_t message[] = "AT\n";
    char str[255];
    int pos = 0;

    while (true)
    {
        if (pressed && gpio_get(SW_0) == 0) {
            pressed = false;
        }
        else if (!pressed && gpio_get(SW_0) > 0) {
            send = !send;
            pressed = true;
            sleep_ms(10);
        }

        if (send) {
            printf("Sending...\n");
            uart_write_blocking(uart1, message, strlen(message));
            printf("Sent.\n");

            send = false;
        }
        while (uart_is_readable(uart1)) {
            char chr = uart_getc(uart1);
            if (chr == '\r' || chr == '\n') {
                str[pos] = '\0';
                printf("Recieved: %s\n", str);
                pos = 0;
            }
            else {
                if (pos < 254) {
                    str[pos++] = chr;
                }
            }
        }
    }
    

    return 0;
}
