/* Name: Eric Lin
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLASSIFIED ./ .

int counter;

void replaceSpaces(char* original_name, FILE* output_log){

	// Initialize all our variables and allocate space on the heap
	int i = 0;
	int j = 0;
	int first = 0;
	int count = 0;
	int period = 0;
	int result;
	int len = strlen(original_name);
	
	char* argument = original_name;
	char* file = malloc(3*len);
	char* original = malloc(len);

	
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
	file[len-1] = '\0';
	len--;
	FILE *test;
	
	while(1){	
		test = fopen(file, "r");

		if(test != NULL){
			fclose(test);
			fprintf(output_log, "Entry %d: %s is taken. Appending...\n", 
				counter, file);
			if ( first == 0){
			for (j = len + (len - period - 3); j >= period + 2; j--){
				file[j] = file[j-2];
			}
			
		/*	for (j = period + 2; j < len + (len - period - 3); j++){
				file[j] = file[j-2];
			}
		*/
			file[period] = (char)(95);
			file[len + (len - period - 3) + 1] = '\0';
			first++;
			}
			file[period + 1] = (char)(count++ + 49);
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

	printf("This will rename all the files in this directory and all its"
		" subdirectories. It is recommended to not run this script in the"
		" home or root directory as it can edit important config files."
		" Please read the README for a more detailed"
		" explanation. Do you wish to continue? (Y/N)\n");

	
	while(1){	
		
		scanf("%s", &result);

		if (result == 'N' || result == 'n'){
			return 0;
		}
		if (result == 'Y' || result == 'y'){

			// Open our log file
			FILE *input_log, *output_log;
			char buf[50];
			counter = 1;

			input_log = fopen("log.txt", "r+");
			output_log = fopen("output_log.txt", "w+");
	
			// Check if fopen was successful
			if (!input_log){
				return 1;
			}

			if (!output_log){
				return 1;
			}

			fprintf(output_log, "Beginning to rename...\n");
			fprintf(output_log, "\n");

			// Replace spaces for all of our files
			while (fgets(buf, 50, input_log) != NULL){
				replaceSpaces(buf, output_log);
				memset(buf, 0, 50);
			}
	
			fprintf(output_log, "\nDone renaming. Total files renamed"
				": %d\n", counter - 1);
			fclose(input_log);
			return 0;
		}
		printf("Invalid input. Do you wish to proceed? (Y/N)\n");
	}
	return 0;
}
