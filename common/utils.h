#ifndef UTILS_H
# define UTILS_H

#include <stdio.h>
#include <stdint.h>

char* get_next_line(FILE* file);

char** parse_grid(FILE* file, size_t* rows, size_t* columns);
void print_grid(char** grid, size_t rows, size_t columns);
char grid_get(char** grid, int64_t row, int64_t column, size_t rows, size_t columns);
int grid_set(char** grid, int64_t row, int64_t column, size_t rows, size_t columns, char c);

#endif
