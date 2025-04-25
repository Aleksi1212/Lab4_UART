#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "constants.h"
#include "str_utils.h"
#include "uart.h"

static bool read_uart_char(char *str, char *cmp, int *pos, int *attempts)
{
    char chr = uart_getc(uart1);

    if (*pos < MAX_RESPONSE_SIZE - 1 && (chr != '\r' && chr != '\n')) {
        str[*pos] = chr;
        *pos += 1;
        return false;
    }

    if (chr == '\r' || chr == '\n') {
        str[*pos] = '\0';
        if (strstr(str, cmp) != NULL) {
            *pos = 0;
            return true;
        }
    }

    if (attempts != NULL)
        *attempts += 1;
    return false;
}

bool write_uart(uart_inst_t *uart, const uint8_t *message)
{
    uart_write_blocking(uart, message, strlen(message));
    return true;
}

void read_at_command(uart_inst_t *uart, int *step)
{
    char str[MAX_RESPONSE_SIZE];

    int pos = 0;
    int attempts = 0;
    
    bool ok = false;

    while (attempts < 5) {
        if (ok) {
            printf("Connected to LoRa module\n");
            *step = 3;
            break;
        }
            
        bool readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
        while (readable) {
            ok = read_uart_char(str, "+AT: OK", &pos, &attempts);
            if (ok) break;

            readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
        }

        if (!readable)
            attempts++;
    }

    if (attempts == 5) {
        *step = 1;
        printf("Module not responding\n");
    }
}

void read_ver_command(uart_inst_t *uart, int *step)
{
    char str[MAX_RESPONSE_SIZE];
    int pos = 0;

    bool readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
    while (readable) {
        if (read_uart_char(str, "+VER:", &pos, NULL)) {
            char *splitted_ver[2];
            int split_count = split_string(str, ":", splitted_ver, 2);

            if (split_count >= 2) {
                remove_char(splitted_ver[1], ' ');
                printf("Firmware version: %s\n", splitted_ver[1]);
            } else {
                fprintf(stderr, "ERROR: Invalid firmware version format (%s)\n", str);
                *step = 1;
                return;
            }
            break;
        }

        readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
    }

    if (readable) {
        *step = 4;
    } else {
        printf("Module stopped responding\n");
        *step = 1;
    }
}

void read_deveui_command(uart_inst_t *uart, int *step)
{
    char str[MAX_RESPONSE_SIZE];
    int pos = 0;

    bool readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
    while (readable) {
        if (read_uart_char(str, "+ID: DevEui,", &pos, NULL)) {
            char *splitted_deveui[2];
            int split_count = split_string(str, ",", splitted_deveui, 2);

            if (split_count >= 2) {
                remove_char(splitted_deveui[1], ':');
                remove_char(splitted_deveui[1], ' ');
                lower_case(splitted_deveui[1]);

                printf("DevEui: %s\n", splitted_deveui[1]);
            } else {
                fprintf(stderr, "ERROR: Invalid DevEui format (%s)\n", str);
                *step = 1;
                return;
            }
            break;
        }

        readable = uart_is_readable_within_us(uart, READ_UART_TIME_OUT_MS * 1000);
    }
    
    if (readable) {
        *step = 5;
    } else {
        printf("Module stopped responding\n");
        *step = 1;
    }
}
