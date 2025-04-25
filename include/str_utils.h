#ifndef STR_UTILS_H
#define STR_UTILS_H

int split_string(char *str, const char *delimeter, char *output_arr[], int max_count);
void remove_char(char *str, const char chr);
void lower_case(char *str);
void clear_terminal();

#endif