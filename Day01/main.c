#include <stdio.h>
#include <stdlib.h>

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

	int number = 50; // Where the dial starts
	int times_landed = 0;
	int times_passed = 0;

	char buffer[10]; // Max 3 digit number
	while (fgets(buffer, 10, input)) {
		int clicks = atoi(buffer + 1);

		if (buffer[0] == 'L') {
			number = turn(number, -clicks, &times_landed, &times_passed);
		}
		else if (buffer[0] == 'R') {
			number = turn(number, clicks, &times_landed, &times_passed);
		}
	}

	printf("The Part 1 password is %d\n", times_landed);
	printf("The Part 2 password is %d\n", times_landed + times_passed);
}
