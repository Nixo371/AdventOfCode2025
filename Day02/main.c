#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

range* parse_ranges(char* line, int* total_range_count) {
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

int main(int argc, char *argv[]) {
	char* file_name = "input";
	if (argc > 1) {
		file_name = argv[1];
	}
	FILE* input = fopen(file_name, "r");

	char* line = get_next_line(input);

	int range_count;
	range* ranges = parse_ranges(line, &range_count);

	int64_t invalid_id_sum = 0;
	for (size_t i = 0; i < range_count; i++) {
		for (int64_t id = ranges[i].start; id < ranges[i].end; id++) {
			if (is_invalid_id(id)) {
				// printf("Invalid ID: %ld\n", id);
				invalid_id_sum += id;
			}
		}
	}

	printf("Invalid ID sum: %ld\n", invalid_id_sum);
}
