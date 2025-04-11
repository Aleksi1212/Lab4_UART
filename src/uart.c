#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "constants.h"
#include "uart.h"

bool write_uart(uart_inst_t *uart, const uint8_t *message, int *state)
{
    uart_write_blocking(uart, message, strlen(message));

    *state = 2;
    return true;
}

void read_uart(uart_inst_t *uart, int *state)
{
    char str[MAX_RESPONSE_SIZE];

    int pos = 0;
    int attempts = 0;
    
    bool ok = false;

    while (attempts < 5) {
        if (ok)
            break;

        bool readable = uart_is_readable_within_us(uart, 5000);
        if (!readable) {
            attempts++;
            continue;
        }

        while (readable) {
            char chr = uart_getc(uart1);

            if (pos < MAX_RESPONSE_SIZE - 1 && (chr != '\r' && chr != '\n')) {
                str[pos++] = chr;
            }

            if (chr == '\r' || chr == '\n') {
                str[pos] = '\0';
                if (strcmp(str, "+AT: OK") == 0) {
                    printf("Connected to LoRa module\n");
                    pos = 0;
                    ok = true;
                    *state = 3;
                    break;
                } else {
                    attempts++;
                }
            }
        }
    }

    if (attempts == 5) {
        *state = 1;
        printf("Module not responding\n");
    }
}