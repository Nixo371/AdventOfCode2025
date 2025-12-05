#include <stdio.h>
#include <stdlib.h>

#include "../common/utils.h"

int normalize(int number) {
	while (number < 0) {
		number += 100;
	}
	return (number % 100);
}

int turn(int number, int clicks, int* times_landed, int* times_passed) {
	int new_number = number + clicks;

	// L
	if (clicks < 0) {
		if (new_number < 0 && (new_number % 100) != 0) {
			int passed = -(new_number / 100) + 1;
			if (number == 0) {
				passed -= 1;
			}
			*times_passed += passed;
		}
		else if (new_number == 0) {
			// nothing
		}
		else if (new_number > 0) {
			// nothing
		}
		else if (new_number < 0 && (new_number % 100) == 0) {
			int passed = -(new_number / 100);
			if (number == 0) {
				passed -= 1;
			}
			*times_passed += passed;
		}
	}
	// R
	else {
		if (new_number > 100 && (new_number % 100) != 0) {
			int passed = (new_number / 100);
			*times_passed += passed;
		}
		else if (new_number == 100) {
			// nothing
		}
		else if (new_number < 100) {
			// nothing
		}
		else if (new_number > 100 && (new_number % 100) == 0) {
			int passed = (new_number / 100) - 1;
			if (number == 0) {
				passed -= 1;
			}
			*times_passed += passed;
		}
	}

	if (normalize(new_number) == 0) {
		*times_landed += 1;
	}

	return (normalize(new_number));
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

	int number = 50; // Where the dial starts
	int times_landed = 0;
	int times_passed = 0;

	for (char* line = get_next_line(input); line != NULL; line = get_next_line(input)) {
		int clicks = atoi(line + 1);

		if (line[0] == 'L') {
			number = turn(number, -clicks, &times_landed, &times_passed);
		}
		else if (line[0] == 'R') {
			number = turn(number, clicks, &times_landed, &times_passed);
		}

		free(line);
	}

	printf("The Part 1 password is %d\n", times_landed);
	printf("The Part 2 password is %d\n", times_landed + times_passed);
}
