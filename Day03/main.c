#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/utils.h"

int64_t get_maximum_joltage(int* batteries, int battery_count, int digits) {
	int64_t max_joltage = 0;
	size_t previous_battery = -1;

	for (int digit = 0; digit < digits; digit++) {
		int max_single_joltage = -1;
		size_t last_possible_digit = battery_count - (digits - digit) + 1;
		for (size_t i = previous_battery + 1; i < last_possible_digit; i++) {
			if (batteries[i] > max_single_joltage) {
				max_single_joltage = batteries[i];
				previous_battery = i;
			}
		}

		max_joltage = (max_joltage * 10) + max_single_joltage;
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

	int64_t total_joltage = 0;
	int64_t total_joltage2 = 0;
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		int battery_count;
		int* batteries = parse_batteries(line, &battery_count);

		int64_t maximum_joltage = get_maximum_joltage(batteries, battery_count, 2);
		int64_t maximum_joltage2 = get_maximum_joltage(batteries, battery_count, 12);

		total_joltage += maximum_joltage;
		total_joltage2 += maximum_joltage2;

		free(batteries);
		free(line);
	}

	printf("Total Joltage Part 1: %ld\n", total_joltage);
	printf("Total Joltage Part 2: %ld\n", total_joltage2);
}
