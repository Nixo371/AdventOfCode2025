#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/utils.h"

#define DEBUG 0

int main(int argc, char *argv[]) {
	char* file_name = "input";
	if (argc > 1) {
		file_name = argv[1];
	}
	FILE* input = fopen(file_name, "r");
	if (input == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	size_t rows;
	size_t columns;
	char** grid = parse_grid(input, &rows, &columns);
	int64_t** paths = (int64_t **) malloc(rows * sizeof(int64_t *));
	for (size_t i = 0; i < rows; i++) {
		paths[i] = (int64_t *) malloc(columns * sizeof(int64_t));
		for (size_t j = 0; j < columns; j++) {
			paths[i][j] = 0;
		}
	}

	size_t splits = 0;
	size_t total_paths = 0;
	for (size_t row = 0; row < rows; row++) {
		for (size_t column = 0; column < columns; column++) {
			char c = grid_get(grid, row, column, rows, columns);
			if (c == '\0') {
				perror("grid_get");
				printf("Error getting grid[%lu][%lu]\n", row, column);
			}

			if (c == 'S') {
				paths[row][column] = 1;
			}
			if ((c == 'S' || c == '|') && grid_get(grid, row + 1, column, rows, columns) != '^') {
				if (grid_set(grid, row + 1, column, rows, columns, '|') == 0) {
					paths[row + 1][column] += paths[row][column];
				}
			}
			else if ((c == 'S' || c == '|') && grid_get(grid, row + 1, column, rows, columns) == '^') {
				// if out of bounds we don't do anything so we can leave this unhandled
				if (grid_set(grid, row + 1, column - 1, rows, columns, '|') == 0) {
					paths[row + 1][column - 1] += paths[row][column];
				}

				if (grid_set(grid, row + 1, column + 1, rows, columns, '|') == 0) {
					paths[row + 1][column + 1] += paths[row][column];
				}

				splits += 1;
			}
		}
		if (DEBUG) {
			print_grid(grid, rows, columns);
			for (size_t i = 0; i < columns; i++) {
				printf("-");
			}
			printf("\n");
		}
	}

	for (size_t column = 0; column < columns; column++) {
		total_paths += paths[rows - 1][column];
	}

	printf("Times Split (Part 1): %lu\n", splits);
	printf("Different Paths (Part 2): %lu\n", total_paths);
}

