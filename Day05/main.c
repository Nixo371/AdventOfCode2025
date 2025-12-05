#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	printf("%ld - %ld", r.start, r.end);
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

int is_fresh(range* ranges, int range_count, int64_t id) {
	for (int i = 0; i < range_count; i++) {
		if (id >= ranges[i].start && id <= ranges[i].end) {
			return (1);
		}
	}

	return (0);
}

int main(int argc, char *argv[]) {
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
		
		if (is_fresh(ranges, range_count, id)) {
			fresh_ingredients += 1;
		}

		free(line);
	}

	printf("Fresh Ingredients: %d\n", fresh_ingredients);
}

