#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"

#define DEBUG 0

enum operation {
	ADD,
	MUL
};

typedef struct s_equation {
	size_t number_size;
	size_t number_count;
	int64_t* numbers;
	enum operation op;
} equation;

typedef struct s_equations {
	size_t equation_size;
	size_t equation_count;
	equation* equations;
} equations;

int64_t parse_number(char* line, size_t length) {
	char* number_str = strndup(line, length);
	int64_t number = atol(number_str);

	free(number_str);
	return (number);
}

int is_operation(char c) {
	if (c == '+' || c == '*') {
		return (1);
	}
	
	return (0);
}

int64_t evaluate_equation(equation eq) {
	int64_t value;
	if (eq.op == ADD) {
		value = 0;
		for (size_t i = 0; i < eq.number_count; i++) {
			value += eq.numbers[i];
		}
	}
	else if (eq.op == MUL) {
		value = 1;
		for (size_t i = 0; i < eq.number_count; i++) {
			value *= eq.numbers[i];
		}
	}

	return (value);
}

void print_grid(char** grid, size_t rows) {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; grid[i][j] != '\0'; j++) {
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

// start_column and end_column are inclusive
int64_t evaluate_equation2(char** grid, size_t start_column, size_t end_column, size_t rows) {
	if (start_column == 0 && end_column == 0) {
		return (0);
	}

	if (DEBUG) {
		printf("Evaluation equation between %ld and %ld\n", start_column, end_column);
	}

	int64_t value;
	if (grid[rows - 1][start_column] == '+') {
		if (DEBUG) {
			// printf("Addition\n");
		}
		value = 0;
		for (int c = end_column; c >= (int)start_column; c--) {
			int num = 0;
			if (DEBUG) {
				// printf("ADD: ");
			}
			for (size_t r = 0; r < rows - 1; r++) {
				if (DEBUG) {
					printf("grid[%ld][%d] = '%c'\n", r, c, grid[r][c]);
				}
				if (grid[r][c] == ' ') {
					continue;
				}
				num = (num * 10) + (grid[r][c] - '0');
			}
			value += num;
			if (DEBUG) {
				// printf("\n");
			}
		}
	}
	else if (grid[rows - 1][start_column] == '*') {
		if (DEBUG) {
			// printf("Multiplication\n");
		}
		value = 1;
		for (int c = end_column; c >= (int)start_column; c--) {
			int num = 0;
			if (DEBUG) {
				// printf("MUL: ");
			}
			for (size_t r = 0; r < rows - 1; r++) {
				if (DEBUG) {
					printf("grid[%ld][%d] = '%c'\n", r, c, grid[r][c]);
				}
				if (grid[r][c] == ' ') {
					continue;
				}
				num = (num * 10) + (grid[r][c] - '0');
			}
			value *= num;
			if (DEBUG) {
				// printf("\n");
			}
		}
	}

	return (value);
}

int64_t evaluate_equations2(char** grid, size_t rows) {
	size_t columns = 0;
	while (grid[0][columns] != '\0') {
		columns += 1;
	}
	if (DEBUG) {
		printf("%ld columns\n", columns);
	}

	int64_t total = 0;

	size_t start_c = 0;
	for (size_t i = 0; i < columns; i++) {
		if (grid[rows - 1][i] == '+' || grid[rows - 1][i] == '*') {
			if (DEBUG) {
				printf("Next equation between columns %ld and %ld\n", start_c, i - 2);
			}
			if (i >= 2) {
				total += evaluate_equation2(grid, start_c, i - 2, rows);
			}
			start_c = i;
		}
	}
	total += evaluate_equation2(grid, start_c, columns - 1, rows);

	return (total);
}

void init_equations(equations* equations) {
	equations->equation_size = 16;
	equations->equation_count = 0;

	equations->equations = (equation *) malloc(equations->equation_size * sizeof(equation));
	for (size_t i = 0; i < equations->equation_size; i++) {
		equations->equations[i].number_size = 4;
		equations->equations[i].number_count = 0;
		equations->equations[i].numbers = (int64_t *) malloc(equations->equations[i].number_size * sizeof(int64_t));
	}
}

void insert_number(equations* equations, int64_t number, size_t equation_i) {
	if (equation_i >= equations->equation_size) {
		if (DEBUG) {
			printf("Exceeded maximum equation size of %lu, reallocating...\n", equations->equation_size);
		}
		equations->equation_size *= 2;
		equations->equations = realloc(equations->equations, equations->equation_size * sizeof(equation));
		for (size_t i = equations->equation_size / 2; i < equations->equation_size; i++) {
			equations->equations[i].number_size = 4;
			equations->equations[i].number_count = 0;
			equations->equations[i].numbers = (int64_t *) malloc(equations->equations[i].number_size * sizeof(int64_t));
		}
		if (DEBUG) {
			printf("Reallocated successfully\n");
		}
	}

	size_t number_i = equations->equations[equation_i].number_count;
	if (number_i >= equations->equations[equation_i].number_size) {
		if (DEBUG) {
			printf("Exceeded maximum number size of %lu, reallocating...\n", equations->equations[equation_i].number_size);
		}
		equations->equations[equation_i].number_size *= 2;
		equations->equations[equation_i].numbers = realloc(equations->equations[equation_i].numbers, equations->equations[equation_i].number_size);
		if (DEBUG) {
			printf("Reallocated successfully\n");
		}
	}

	if (DEBUG) {
		printf("Inserting %ld into equation %ld at position %ld\n", number, equation_i, number_i);
	}
	equations->equations[equation_i].numbers[number_i] = number;

	if (equations->equations[equation_i].number_count == 0) {
		if (DEBUG) {
			printf("First number of equation %ld, incrementing equation count\n", equation_i);
		}
		equations->equation_count += 1;
	}
	equations->equations[equation_i].number_count += 1;
}

void print_equations(equations* equations) {
	for (size_t i = 0; i < equations->equation_count; i++) {
		for (size_t j = 0; j < equations->equations[i].number_count; j++) {
			printf("%ld ", equations->equations[i].numbers[j]);
			if (j < equations->equations[i].number_count - 1) {
				if (equations->equations[i].op == ADD) {
					printf("+ ");
				}
				else if (equations->equations[i].op == MUL) {
					printf("* ");
				}
				else {
					printf("? ");
				}
			}
		}
		printf("\n");
	}
}

void parse_line(equations* equations, char* line) {
	if (equations->equations == NULL) {
		init_equations(equations);
	}

	int is_number = 1;

	size_t i = 0;
	size_t number_start;
	size_t number_length;
	size_t equation_i = 0;
	while (line[i] != '\0') {
		while (line[i] == ' ') {
			i += 1;
		}
		if (line[i] == '\0') {
			break;
		}

		if (is_operation(line[i])) {
			is_number = 0;
		}

		if (is_number) {
			number_start = i;
			number_length = 0;
			while (line[i] != ' ' && line[i] != '\0') {
				number_length += 1;
				i += 1;
			}

			int64_t number = parse_number(line + number_start, number_length);
			if (DEBUG) {
				printf("Inserting [%ld] into equation %lu\n", number, equation_i);
			}
			insert_number(equations, number, equation_i);
		}
		else {
			if (line[i] == '+') {
				equations->equations[equation_i].op = ADD;
			}
			else if (line[i] == '*') {
				equations->equations[equation_i].op = MUL;
			}
			else {
				printf("WHAT THE FUCK IS THIS OPERATION (%c)\n", line[i]);
			}

			i += 1;
		}

		equation_i += 1;
	}
}

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

	size_t grid_rows = 8;
	size_t row = 0;
	char** grid = (char **) malloc(grid_rows * sizeof(char *));

	equations equations = {0};
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		parse_line(&equations, line);

		if (row >= grid_rows) {
			grid_rows *= 2;
			grid = realloc(grid, grid_rows * sizeof(char *));
		}
		grid[row] = line;
		row += 1;
	}

	if (DEBUG) {
		print_equations(&equations);
	}

	int64_t grand_total = 0;
	for (size_t i = 0; i < equations.equation_count; i++) {
		int64_t value = evaluate_equation(equations.equations[i]);
		grand_total += value;
	}

	if (DEBUG) {
		print_grid(grid, row);
	}
	int64_t grand_total2 = evaluate_equations2(grid, row);

	printf("Grand Total (Part 1): %ld\n", grand_total);
	printf("Grand Total (Part 2): %ld\n", grand_total2);
}

