#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"

typedef struct s_range {
	int64_t start;
	int64_t end;
} range;

size_t get_number_digit_count(int64_t number) {
	size_t digit_count = 1;
	while (number / 10) {
		number /= 10;
		digit_count += 1;
	}

	return (digit_count);
}

int* get_factors(int number, size_t* factor_count) {
	*factor_count = 1; // 1 and itself
	size_t factor_size = 4;
	
	int* factors = (int *) malloc(factor_size * sizeof(int));
	factors[0] = 1;
	for (int i = 2; i <= number / 2; i++) {
		if (number % i == 0) {
			if (*factor_count >= factor_size) {
				factor_size *= 2;
				factors = realloc(factors, factor_size);
			}
			factors[*factor_count] = i;
			*factor_count += 1;
		}
	}
	factors = realloc(factors, *factor_count + 1);
	factors[*factor_count] = number;
	*factor_count += 1;

	return (factors);
}

int is_invalid_id(int64_t id) {
	size_t digit_count = get_number_digit_count(id);

	// Odd numbers can't be invalid
	if (digit_count % 2 == 1) {
		return (0);
	}

	int64_t modulo = 1;
	while (digit_count > 0) {
		modulo *= 10;
		digit_count -= 2;
	}
	modulo += 1;

	if (id % modulo == 0) {
		return (1);
	}
	return (0);
}

int is_invalid_id2(int64_t id) {
	size_t digit_count = get_number_digit_count(id);
	if (digit_count <= 1) {
		return (0);
	}
	
	// Check all factors and generate all possible modulos
	// Ex: 565656
	// Digit Count = 6
	// Factors: 1, 2, 3, 6
	// Modulos: 001001 (2), 010101 (3)

	size_t factor_count;
	int* factors = get_factors(digit_count, &factor_count);

	// ignore last (id)
	for (size_t i = 1; i < factor_count; i++) {
		int factor = factors[i];
		int64_t modulo = 1;
		// multiply by 10 [number / factor] times; then add 1
		// repeat [factor - 1] times
		for (int repeats = 0; repeats < (factor - 1); repeats++) {
			for (size_t spacing = 0; spacing < (digit_count / factor); spacing++) {
				modulo *= 10;
			}
			modulo += 1;
		}

		// printf("[%ld]\tModulo for %d is %ld\n", id, factor, modulo);
		if (id % modulo == 0) {
			return (1);
		}
	}

	return (0);
}

range* parse_ranges(char* line, size_t* total_range_count) {
	*total_range_count = 0;
	size_t range_count = 0;
	for (size_t i = 0; line[i] != '\0'; i++) {
		if (line[i] == '-') {
			*total_range_count += 1;
		}
	}

	range* ranges = (range *) malloc(*total_range_count * sizeof(range));

	size_t i = 0;
	size_t number_start;
	size_t number_length;

	char* number_str;
	while (range_count < *total_range_count) {
		number_start = i;
		number_length = 0;

		// Get first number of range
		while (line[i] != '-') {
			number_length += 1;
			i += 1;
		}

		number_str = strndup(line + number_start, number_length);
		ranges[range_count].start = atol(number_str);
		i += 1;

		number_start = i;
		number_length = 0;
		// Get second number of range
		while (line[i] != ',' && line[i] != '\0') {
			number_length += 1;
			i += 1;
		}

		number_str = strndup(line + number_start, number_length);
		ranges[range_count].end = atol(number_str);
		i += 1;

		range_count += 1;
	}

	return (ranges);
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

	char* line = get_next_line(input);

	size_t range_count;
	range* ranges = parse_ranges(line, &range_count);

	int64_t invalid_id_sum = 0;
	int64_t invalid_id_sum2 = 0;
	for (size_t i = 0; i < range_count; i++) {
		for (int64_t id = ranges[i].start; id <= ranges[i].end; id++) {
			if (is_invalid_id(id)) {
				// printf("Invalid ID: %ld\n", id);
				invalid_id_sum += id;
			}
			if (is_invalid_id2(id)) {
				// printf("Invalid ID: %ld\n", id);
				// printf("%ld - Invalid\n", id);
				invalid_id_sum2 += id;
			}
		}
	}

	printf("Invalid ID sum: %ld\n", invalid_id_sum);
	printf("Invalid ID sum 2: %ld\n", invalid_id_sum2);
}
