#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define DEBUG 0

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

	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		printf("%s\n", line);

		free(line);
	}
}

