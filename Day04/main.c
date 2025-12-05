#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

void print_grid(int** grid, int rows, int columns) {
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] >= 0 && grid[row][column] < 4) {
				printf("x");
			}
			else if (grid[row][column] >= 0) {
				printf("@");
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}
}

void print_grid_values(int** grid, int rows, int columns) {
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] >= 0) {
				printf("%d", grid[row][column]);
			}
			else {
				printf(".");
			}
		}
		printf("\n");
	}
}

int** initialize_grid(FILE* file, int* rows, int* columns) {
	*rows = 0;
	*columns = 0;
	for (char* line = get_next_line(file); line != NULL; line = get_next_line(file)) {
		*rows += 1;
		if (*columns == 0) {
			for (int i = 0; line[i] != '\0'; i++) {
				*columns += 1;
			}
		}

		free(line);
	}
	fseek(file, 0, SEEK_SET);

	int** grid = (int **) malloc(*rows * sizeof(int *));
	for (int i = 0; i < *rows; i++) {
		grid[i] = (int *) malloc(*columns * sizeof(int));
	}

	int row = 0;
	for (char* line = get_next_line(file); line != NULL; line = get_next_line(file)) {
		for (int column = 0; column < *columns; column++) {
			if (line[column] == '@') {
				grid[row][column] = 0;
			}
			else {
				grid[row][column] = -9; // adding the maximum 8 still results in a negative value (for NOT rolls of paper)
			}
		}
		row += 1;

		free(line);
	}

	return (grid);
}

void add_one(int** grid, int row, int column, int rows, int columns) {
	if (row < 0 || column < 0 || row >= rows || column >= columns) {
		return;
	}

	grid[row][column] += 1;
}

void add_one_to_surrounding(int** grid, int row, int column, int rows, int columns) {
	for (int r = -1; r <= 1; r++) {
		for (int c = -1; c <= 1; c++) {
			if (r == 0 && c == 0) {
				continue;
			}
			add_one(grid, row + r, column + c, rows, columns);
		}
	}
}

int main(int argc, char *argv[]) {
	char* file_name = "input";
	if (argc > 1) {
		file_name = argv[1];
	}
	FILE* input = fopen(file_name, "r");

	int rows;
	int columns;
	int** grid = initialize_grid(input, &rows, &columns);

	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] >= 0) {
				add_one_to_surrounding(grid, row, column, rows, columns);
			}
		}
	}

	int accessible_rolls = 0;
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] >= 0 && grid[row][column] < 4) {
				accessible_rolls += 1;
			}
		}
	}

	printf("Accessible Rolls of Paper: %d\n", accessible_rolls);
}

