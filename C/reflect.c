#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reflect.h"
#include "schur.h"
#include "xcorr.h"
#include "framework.h"


void reflect(int N, float (*mat)[MAX_BLOCK_SIZE], float (*res)[SIZE])
{
	/* 
	Calculates average matrix of input.
	 Returns a 10x10 matrix and takes a 10XN matrix as input.
	*/
	N = N-3; //Because Johan and Johannes write messy code
	int i, j;
	int b = N/SIZE; //Number of columns in each compressed column
	int last = N%SIZE; // Number of columns in last compressed column
	float scale = 1.0/b;
	float scale_last = 1.0/(b+1);

	//Run compression algorithm
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE-last; j++) {
			res[i][j] = scale*sum(&mat[i][j*b], b);
		}
	}

	//Last column (if present):
	if(last) {
		for(i = 0; i < SIZE; i++) {
			int step_size = SIZE - last;
			for(j = SIZE - last; j < SIZE; j++) {
				if(j == step_size) {
					res[i][j] = scale_last*sum(&mat[i][j*(b)], b+1);
				} else {
					res[i][j] = scale_last*sum(&mat[i][j*(b + 1) - step_size], b+1);
				}
			}
		}
	}
}
