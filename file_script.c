/* Name: Eric Lin
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TWO 2
#define THREE 3
#define OFFSET 49
#define BUF_SIZE 50
#define UNDERSCORE 95

int counter, discarded;

void replaceSpaces(char* original_name, FILE* output_log){

	// Initialize all our variables and allocate space on the heap
	int i, j, first, count, period = 0;
	int result;
	int len = strlen(original_name);
	
	char* argument = original_name;
	char* file = malloc(THREE*len);
	char* original = malloc(len);

	FILE *test;
	
	// Copy over relevant characters and rename ' ' to '_'
	for ( i = 0; i < len - 1; i++){
		file[i] = argument[i];
		original[i] = original_name[i];
	}
	original[len - 1] = '\0';
	for (i = 0; i < len - 1; i++){
		if (file[i] == ' '){
			file[i] = '_';
		}
		if (file[i] == '.'){
			if ( i > period){
				period = i;
			}
		}
	}
	file[--len] = '\0';

	// Loop to make sure we don't overwrite a file with existing name	
	while(1){
		if (count == 10){
			fprintf(output_log, "Unable to rename file: %s. Please rename manually\n", file);
			discarded++;
			counter++;
			free(file);
			free(original);
			return;
		}	
		test = fopen(file, "r");

		// If there is a file already named, then rename it again
		if(test != NULL){
			fclose(test);
			fprintf(output_log, "Entry %d: %s is taken. Appending...\n", 
				counter, file);

			// Check if this is the first time re-renaming it
			if ( first == 0){
				for (j = TWO*len - period - THREE; j >= period + TWO; j--){
					file[j] = file[j - TWO];
				}
				file[period] = (char)(UNDERSCORE);
				file[TWO*len - period - TWO] = '\0';
				first++;
			}
			file[period + 1] = (char)(count++ + OFFSET);
		}
		else{
			break;
		}
	}

	result = rename(original, file);
			
	// Log all of the changes we made
	if (result == 0){
		fprintf(output_log, "Entry %d: Renamed file %s to %s\n", 
		counter++, original, file);
	}
	
	// Free our malloced char arrays	
	free(file);
	free(original);

	return;
}

int main(int argc, char** argv){

	char result;

	// Warn user of risks associated with this 
	printf("This will rename all the files in this directory and all its"
		" subdirectories. It is recommended to not run this script in the"
		" home or root directory as it can edit important config files."
		" Please read the README for a more detailed"
		" explanation. Do you wish to continue? (Y/N)\n");

	
	// Keep looping until user gives confirmation
	while(1){	
		
		scanf("%s", &result);

		if (result == 'N' || result == 'n'){
			return 0;
		}
		if (result == 'Y' || result == 'y'){

			// Open our log file
			FILE *input_log, *output_log;
			char buf[BUF_SIZE];
			counter = 1;
			discarded = 0;

			input_log = fopen("log.txt", "r+");
			output_log = fopen("output_log.txt", "w+");
	
			// Check if fopen was successful for our files
			if (!input_log){
				return 1;
			}

			if (!output_log){
				return 1;
			}

			fprintf(output_log, "Beginning to rename...\n");
			fprintf(output_log, "\n");

			// Replace spaces for all of our files
			while (fgets(buf, BUF_SIZE, input_log) != NULL){
				replaceSpaces(buf, output_log);
				memset(buf, 0, BUF_SIZE);
			}
	
			// Done renaming. Print to log file and return
			fprintf(output_log, "\nDone renaming. Total files renamed"
				": %d\n", counter - 1 - discarded);
			fclose(input_log);
			return 0;
		}
		printf("Invalid input. Do you wish to proceed? (Y/N)\n");
	}
	return 0;
}
