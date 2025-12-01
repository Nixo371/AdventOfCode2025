#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* input = fopen("input", "r");

	int number = 50; // Where the dial starts
	int password = 0;

	char buffer[10]; // Max 3 digit number
	while (fgets(buffer, 10, input)) {
		int clicks = atoi(buffer + 1);

		if (clicks < 10) {
			buffer[2] = '\0';
		}
		if (clicks < 100) {
			buffer[3] = '\0';
		}
		if (clicks < 1000) {
			buffer[4] = '\0';
		}
		buffer[5] = '\0';
		// printf("%s\n", buffer);

		if (buffer[0] == 'L') {
			number -= clicks;
			while (number < 0) {
				number += 100;
			}
		}
		else if (buffer[0] == 'R') {
			number += clicks;
			while (number > 99) {
				number -= 100;
			}
		}

		if (number == 0) {
			password += 1;
		}

	}

	printf("The password is %d\n", password);
}
