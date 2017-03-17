#include <stdio.h>
#include <stdlib.h>

#include "framework.h"
#include "print_data.h"

void print_data(float (*K)[SIZE], float q)
{
	printf("start write to file\n");
    	FILE *f = fopen("swartling_new.txt", "w");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	/* print integers and floats to file*/
	int row;
	int col;
	
	for(row=0;row<SIZE;++row){
		for(col=0;col<SIZE;++col){
			fprintf(f, "%f, ", q * K[row][col]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
	printf("end write to file\n");
}
