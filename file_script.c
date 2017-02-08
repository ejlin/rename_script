/* Name: Eric Lin
 * Date: February 1st, 2016
 * Description: Program to rename all files in current directory and
 * 				subdirectory to replace spaces with underscores. 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSET 49
#define SIZE 50

int counter, discarded;

void replaceSpaces(char* original_name, FILE* output_log){

	// Initialize all our variables and allocate space on the heap
	int i, j, result;
	int period = 0, periodBool = 0, count = 0, first = 0; 
	int len = strlen(original_name);
	
	char* argument = original_name;
	char file[3*len];
	char original[len];

	FILE *test;
	
	// Copy over relevant characters 
	for ( i = 0; i < len - 1; i++){
		file[i] = argument[i];
		original[i] = original_name[i];
	}

	// Replace ' ' with '_' and also keep track of file extension
	original[len - 1] = '\0';
	for (i = 0; i < len - 1; i++){
		if (file[i] == ' '){
			file[i] = '_';
		}
		if (file[i] == '.'){
			periodBool++;
		}
		if (file[i] == '.' && periodBool == 2){
			period = i;
		}
	}
	file[--len] = '\0';

	// Loop to make sure we don't overwrite a file with existing name	
	while(1){
		if (count == 10){
			fprintf(output_log, "Unable to rename file: '%s'. Please "
				"rename manually\n", original_name);
			discarded++;
			counter++;
			return;
		}	
		test = fopen(file, "r");

		// If there is a file already named, then rename it again
		if(test != NULL){
			fclose(test);
			fprintf(output_log, "Entry %d: '%s' is taken.\n", 
				counter, file);

			// Check if this is the first time re-renaming it
			if ( first == 0){
				for (j = len + 1; j >= period; j--){
					file[j] = file[j - 2];
				}
			
				// Append to the end of the rename
				file[period] = '_';
				file[len + 2] = '\0';
				first = 1;
			}
			file[period + 1] = (char)(count++ + OFFSET);
		}
		else{
			break;
		}
	}

	result = rename(original, file);
	
	// Check rename was successful and log all of the changes we made
	if (result == 0){
		fprintf(output_log, "Entry %d: Renamed file '%s' to '%s'\n", 
		counter++, original, file);
		fprintf(output_log, "\n");
	}
	
	return;
}

int main(int argc, char** argv){

	char out_log[SIZE];
	char buf[SIZE];

	printf("Name a file to write the output log to: \n");
	scanf("%s", &out_log);
	
	printf("Writing to '%s':\n", out_log);
		
	// Declare and initialize variables
	FILE *input_log, *output_log;
			
	counter = 1;
	discarded = 0;

	// Open our files
	input_log = fopen(argv[1], "r+");
	output_log = fopen(out_log, "w+");
	
	// Check if fopen was successful for our files
	if (!input_log || !output_log){
		return 1;
	}

	fprintf(output_log, "Beginning to rename...\n");
	fprintf(output_log, "\n");

	// Replace spaces for all of our files
	while (fgets(buf, SIZE, input_log) != NULL){
		replaceSpaces(buf, output_log);
//		memset(buf, 0, SIZE);
	}
	
	// Done renaming. Print to log file and return
	fprintf(output_log, "\nDone renaming. Total files renamed"
		": %d\n", counter - discarded - 1);
	fclose(input_log);

	return 0;
}
