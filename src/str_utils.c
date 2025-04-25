#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "str_utils.h"

int split_string(char *str, const char *delimeter, char *output_arr[], int max_count)
{
    int count = 0;
    char *token = strtok(str, delimeter);

    while (token != NULL && count < max_count)
    {
        output_arr[count++] = token;
        token = strtok(NULL, delimeter);
    }

    return count; 
}

void remove_char(char *str, const char chr)
{
    int i = 0, j = 0;

    while (str[i]) {
        if (str[i] != chr) {
            str[j++] = str[i];
        }
        i++;
    }

    str[j] = '\0';
}

void lower_case(char *str)
{
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void clear_terminal()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

