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

	equations equations = {0};
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		parse_line(&equations, line);

		free(line);
	}

	if (DEBUG) {
		print_equations(&equations);
	}

	int64_t grand_total = 0;
	for (size_t i = 0; i < equations.equation_count; i++) {
		int64_t value = evaluate_equation(equations.equations[i]);
		grand_total += value;
	}

	printf("Grand Total (Part 1): %ld\n", grand_total);
}

