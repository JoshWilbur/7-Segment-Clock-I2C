#include <stdio.h>
#include <stdlib.h>

int get_temp(void){
	// Initialize buffers and temperature
	char buffer[128];
	char command[] = "python3 weather.py";
	int temp_F = -99;

	// Open the command for reading, error check
	FILE *fp = popen(command, "r");
	if (fp == NULL){
		printf("Failed to run command\n");
        	return 1;
	}

	// Read output, convert buffer to int
	if (fgets(buffer, sizeof(buffer)-1, fp) != NULL) temp_F = atoi(buffer);

	// Close the file pointer
	fclose(fp);

	return temp_F;
}
