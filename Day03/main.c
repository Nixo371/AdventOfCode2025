#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int get_maximum_joltage(int* batteries, int battery_count) {
	size_t max_single_joltage_index = -1;
	int max_single_joltage = -1;
	int max_joltage = -1;

	// Find the largest number (except the last one)
	// This gives us the 10s place for our maximum joltage
	for (size_t i = 0; i < battery_count; i++) {
		if (batteries[i] > max_single_joltage && i != battery_count - 1) {
			max_single_joltage = batteries[i];
			max_single_joltage_index = i;
		}
	}

	max_joltage = max_single_joltage * 10;

	int starting_joltage = max_joltage;
	for (size_t i = max_single_joltage_index + 1; i < battery_count; i++) {
		if (starting_joltage + batteries[i] > max_joltage) {
			max_joltage = starting_joltage + batteries[i];
		}
	}

	return (max_joltage);
}

int* parse_batteries(char* line, int* battery_count) {
	int batteries_length = 100;
	*battery_count = 0;

	int* batteries = (int *) malloc(batteries_length * sizeof(int));
	for (size_t i = 0; line[i] != '\0'; i++) {
		if (*battery_count >= batteries_length) {
			batteries_length *= 2;
			batteries = realloc(batteries, batteries_length * sizeof(int));
		}
		batteries[*battery_count] = line[i] - '0';
		*battery_count += 1;
	}
	batteries = realloc(batteries, *battery_count * sizeof(int));

	return (batteries);
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

	int total_joltage = 0;
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		int battery_count;
		int* batteries = parse_batteries(line, &battery_count);

		int maximum_joltage = get_maximum_joltage(batteries, battery_count);

		total_joltage += maximum_joltage;

		free(batteries);
		free(line);
	}

	printf("Total Joltage: %d\n", total_joltage);
}
