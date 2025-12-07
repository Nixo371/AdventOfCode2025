#include "utils.h"

#include <stddef.h>
#include <stdlib.h>

char* get_next_line(FILE* file) {
	if (feof(file) != 0) {
		return (NULL);
	}

	int line_size = 0;
	int max_line_size = 64;
	
	char* line = (char *) malloc(max_line_size * sizeof(char));
	while (fread(line + line_size, 1, sizeof(char), file)) {
		line_size += 1;
		if (line_size >= max_line_size) {
			max_line_size *= 2;
			line = realloc(line, max_line_size);
		}

		if (line[line_size - 1] == '\n') {
			line_size -= 1;
			break;
		}
	}

	if (feof(file) != 0) {
		free(line);

		return (NULL);
	}

	if (ferror(file) != 0) {
		free(line);

		perror("fread");
		exit(EXIT_FAILURE);
	}

	line = realloc(line, line_size + 1);
	line[line_size] = '\0';

	return (line);
}

char** parse_grid(FILE* file, size_t* rows, size_t* columns) {
	*columns = 16;
	*rows = 16;

	char** grid = (char **) malloc(*rows * sizeof(char *));
	for (size_t i = 0; i < *rows ; i++) {
		grid[i] = (char *) malloc(*columns * sizeof(char));
	}

	size_t row = 0;
	size_t column = 0;
	for (char* line = get_next_line(file); line != NULL; line = get_next_line(file)) {
		column = 0;
		if (row >= *rows) {
			*rows *= 2;
			grid = (char **) realloc(grid, *rows * sizeof(char *));
			for (size_t i = *rows / 2; i < *rows; i++) {
				grid[i] = (char *) malloc(*columns * sizeof(char));
			}
		}
		while (line[column] != '\0') {
			if (column >= *columns) {
				*columns *= 2;
				for (size_t i = 0; i < *rows; i++) {
					grid[i] = (char *) realloc(grid[i], *columns * sizeof(char));
				}
			}
			grid[row][column] = line[column];
			column += 1;
		}
		free(line);
		row += 1;
	}

	*rows = row;
	*columns = column;
	grid = (char **) realloc(grid, *rows * sizeof(char *));
	for (size_t i = 0; i < *rows ; i++) {
		grid[i] = (char *) realloc(grid[i], (*columns * sizeof(char)) + 1);
		grid[i][*columns] = '\0';
	}

	return (grid);
}

void print_grid(char **grid, size_t rows, size_t columns) {
	for (size_t row = 0; row < rows; row++) {
		for (size_t column = 0; column < columns; column++) {
			printf("%c", grid[row][column]);
		}
		printf("\n");
	}
}

char grid_get(char** grid, int64_t row, int64_t column, size_t rows, size_t columns) {
	if (row < 0 || (size_t)row >= rows || column < 0 || (size_t)column >= columns) {
		return ('\0');
	}

	return (grid[row][column]);
}

int grid_set(char** grid, int64_t row, int64_t column, size_t rows, size_t columns, char c) {
	if (row < 0 || (size_t)row >= rows || column < 0 || (size_t)column >= columns) {
		return (1);
	}
	grid[row][column] = c;

	return (0);
}
