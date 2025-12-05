#include <locale.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

typedef struct s_range {
	int64_t start;
	int64_t end;
} range;

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

void print_range(range r) {
	printf("%'ld - %'ld", r.start, r.end);
}

void print_ranges(range* ranges, int range_count) {
	for (int i = 0; i < range_count; i++) {
		print_range(ranges[i]);
		printf("\n");
	}
}

range parse_range(char* line) {
	int64_t start = -1;
	int64_t end = -1;

	size_t start_length = 0;
	size_t end_length = 0;
	
	for (int i = 0; line[i] != '-'; i++) {
		start_length += 1;
	}
	char* start_str = strndup(line, start_length);
	start = atol(start_str);

	for (int i = start_length + 1; line[i] != '\0'; i++) {
		end_length += 1;
	}
	char* end_str = strndup(line + start_length + 1, end_length);
	end = atol(end_str);

	free(start_str);
	free(end_str);

	range r;
	r.start = start;
	r.end = end;

	return (r);
}

// returns which range it's in
// -1 if in none
int is_fresh(range* ranges, int range_count, int64_t id) {
	for (int i = 0; i < range_count; i++) {
		if (id >= ranges[i].start && id <= ranges[i].end) {
			return (i);
		}
	}

	return (-1);
}

int64_t get_total_fresh_ingredients(range* ranges, int range_count) {
	int range;
	for (int i = 0; i < range_count; i++) {
		range = is_fresh(ranges, i, ranges[i].start);
		// while the start of a range is inside a previous range
		while (range != -1) {
			if (DEBUG) {
				printf("Range %d\n(", i);
				print_range(ranges[i]);
				printf(") overlaps with range %d\n(", range);
				print_range(ranges[range]);
				printf(")\nAdjusting\n(");
				print_range(ranges[i]);
				printf(") to\n(");
			}

			ranges[i].start = ranges[range].end + 1;
			if (DEBUG) {
				print_range(ranges[i]);
				printf(")\n");
			}

			// Total overlap, set range to "null"
			if (ranges[i].start > ranges[i].end) {
				ranges[i].start = -1;
				ranges[i].end = -1;

				if (DEBUG) {
					printf("Total Overlap!\n");
				}
				break;
			}

			range = is_fresh(ranges, i, ranges[i].start);
		}

		if (ranges[i].start == -1 && ranges[i].end == -1) {
			if (DEBUG) {
				printf("------\n");
			}
			continue;
		}

		range = is_fresh(ranges, i, ranges[i].end);
		// while the end of a range is inside a previous range
		while (range != -1) {
			if (DEBUG) {
				printf("Range %d\n(", i);
				print_range(ranges[i]);
				printf(") overlaps with range %d\n(", range);
				print_range(ranges[range]);
				printf(")\nAdjusting\n(");
				print_range(ranges[i]);
				printf(") to\n(");
			}

			ranges[i].end = ranges[range].start - 1;
			if (DEBUG) {
				print_range(ranges[i]);
				printf(")\n");
			}

			// Total overlap, set range to "null"
			if (ranges[i].end < ranges[i].start) {
				ranges[i].start = -1;
				ranges[i].end = -1;

				break;
			}

			range = is_fresh(ranges, i, ranges[i].end);
		}


		// Detect having a smaller range completely enclosed inside, and killing it
		for (int j = 0; j < i; j++) {
			if (ranges[i].start < ranges[j].start && ranges[i].end > ranges[j].end) {
				if (DEBUG) {
					printf("Range %d\n(", i);
					print_range(ranges[i]);
					printf(") completely encloses range %d\n(", j);
					print_range(ranges[j]);
					printf(")\nKilling range %d\n", j);
				}

				ranges[j].start = -1;
				ranges[j].end = -1;
			}
		}

		if (DEBUG) {
			printf("------\n");
		}
	}

	if (DEBUG) {
		printf("New Ranges:\n");
		print_ranges(ranges, range_count);
	}

	int64_t total_fresh_ingredients = 0;
	for (int i = 0; i < range_count; i++) {
		if (ranges[i].start == -1 && ranges[i].end == -1) {
			continue;
		}

		int64_t ingredients = (ranges[i].end - ranges[i].start) + 1;
		if (DEBUG) {
			printf("Range (");
			print_range(ranges[i]);
			printf("): %ld ingredients\n", ingredients);
		}
		total_fresh_ingredients += ingredients;
	}

	return (total_fresh_ingredients);
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	char* file_name = "input";
	if (argc > 1) {
		file_name = argv[1];
	}
	FILE* input = fopen(file_name, "r");
	if (input == NULL) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	int range_count = 0;
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		if (line[0] == '\0') {
			free(line);
			break;
		}
		range_count += 1;
		free(line);
	}
	fseek(input, 0, SEEK_SET);

	int range_i = 0;
	range* ranges = (range *) malloc(range_count * sizeof(range));
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		if (line[0] == '\0') {
			free(line);
			break;
		}
		ranges[range_i] = parse_range(line);
		range_i += 1;

		free(line);
	}

	int fresh_ingredients = 0;
	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		int64_t id = atol(line);
		
		if (is_fresh(ranges, range_count, id) != -1) {
			fresh_ingredients += 1;
		}

		free(line);
	}

	int64_t total_fresh_ingredients = get_total_fresh_ingredients(ranges, range_count);
	printf("Fresh Ingredients (Part 1): %d\n", fresh_ingredients);
	printf("Total Fresh Ingredients (Part 2): %ld\n", total_fresh_ingredients);
}

