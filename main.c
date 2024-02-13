#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* read_file(char* filename) {
	char* content;
	FILE* file;
	char ch;
	int file_size;

	// Open file
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("File '%s' not found \n", filename);
		return NULL;
	}

	// Determine file size
	fseek(file, 0, SEEK_END); // Move file pointer to end
	file_size = ftell(file);   // Get current position, which is file size
	rewind(file);             // Move file pointer back to the beginning

	// Allocate memory for content
	content = (char*) malloc((file_size + 1) * sizeof(char)); // +1 for null terminator

	// Read file content into string
	int i = 0;
	while ((ch = fgetc(file)) != EOF) {
		content[i++] = ch;
	}
	content[i] = '\0'; // Null-terminate the string


	fclose(file);
	return content;
}

void interpret(char* code) {
	// Memory with zeroes (30k bytes)
	char memory[30001] = { 0 };

	// Set pointer to point at the left-most cell
	char* ptr = memory;
	//    ^ *ptr = Cell Value / ptr = Cell index


	// Read all characters of file
	while(*code != '\0') {
		// printf("%c", *code);
		switch (*code) {
			case '>': // Go to next cell
				ptr++;
				break;
			case '<': // Go to previous cell
				// Check if current ptr is equal to first index in memory
				if(ptr == &memory[0]) {
					printf("ERROR: Try to go back on first index\n");
					exit(1);
				}

				ptr--;
				break;



			case '+': // Increment cell
				++*ptr;
				break;
			case '-': // Decrement cell
				--*ptr;
				break;



			case '[': // Start loop
				// Nothing to loop
				if(*ptr != 0) {
					break;
				}

				int loops = 1; // Keep track of nested loops
				while(loops > 0 && *code != '\0') {
					code++; // Advance character
					
					if(*code == '[')
						loops++; // More loops to do

					else if(*code == ']') {
						loops--;
					}
				}
				break;

			case ']': // End loop
				// printf(" - Loop done\n");

				// If zero, go back to the start of the loop
				if(*ptr == 0) {
					break;
				}

				int iloops = 1; // Inside loops
				while(iloops > 0) {
					code--;

					if(*code == '[')
						iloops--; // One loop done

					else if(*code == ']')
						iloops++;
				}
				break;



			case '.': // Print char
				putchar(*ptr); // Print single character
				break;
			case ',': // Store char
				*ptr = getchar(); // Get char from user
				break;

			// Ignore any other character
			default:
				break;
		}

		code++; // Next character
	}

	putchar('\n');
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Incorrect usage, provide a filename\n");
		return 1;
	}

	char* code = read_file(argv[1]);
	if(code == NULL) {
		return 1;
	}

	interpret(code);

	return 0;
}

/*
++++++++++
++++++++++
++++++++++
++++++++++
++++++++++
++++++++++
+++++
.
*/


// Process of Multiplication
/*
 * Print 'A':
- Start cell: [ 0, 0, 0 ]

Code: +++ +++ [ > +++++ +++++ + <- ]
      ^ Add 6 in first cell then add 11 to the next cell, back to the first and decrease

- Before loop starts: [ 6, 0, 0 ]

- Loop 1: [ 5, 11, 0 ]
- Loop 2: [ 4, 22, 0 ]
- Loop 3: [ 3, 33, 0 ]
- Loop 4: [ 2, 44, 0 ]
- Loop 5: [ 1, 55, 0 ]
- Loop 6: [ 0, 66, 0 ]
            ^ Pointer ends here

The first cell that is checked if the loop should end

Code: >-.
      ^ Go to the next cell, where have numbers, and decrease 1 to get 65, and print

- Memory: [ 0, 66, 0 ]
- On print: 'A'

- 6 x 11 = 66 - 1 = 65 = 'A'

This can be done with any pair of numbers
*/
