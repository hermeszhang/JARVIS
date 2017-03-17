#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "xcorr.h"
#include "schur.h"
#include "reflect.h"


#define N_ELEMENTS 15
#define SIZE 10


int main(void) {
	/*
	float A[2][2];
	float B[2][2];
	float C[2][2];
	int i,j;
	for(i = 0; i < 2; i++) {
		for(j = 0; j < 2; j++) {
			A[i][j] = j + i +1;
			B[i][j] = 2*j + i + 2;
		}
	}
	matrix_mult(A, B, C);
	print_matrix(A, 2, 2);
	print_matrix(B, 2, 2);
	print_matrix(C, 2, 2);
	*/
	//Code to test Schur algorithm	
	float x[N_ELEMENTS] = {0.5377, 1.9952, -1.6603, 0.3641, 0.4280, -1.1793, -0.7874, 0.1064, 3.6103, 3.8525, -0.1941, 2.9767, 1.6184, 0.4225, 0.8415};
	int i;
	float ref_coeff[SIZE];
	float R[SIZE];
	schur(x, ref_coeff, N_ELEMENTS, SIZE);
	//xcorr(x, R, N_ELEMENTS, 10);
	for(i = 0; i < SIZE; i++) {
		printf("RC[%d] = %f\n", i, ref_coeff[i]);
	}
	/*
	//Code to test compression
	float** mat = make_matrix(10, 200);
	FILE* matrix = fopen("testmat.txt", "r");
	float c;
	int i,j = 0;
	//Parse matrix
	while(1) {
		c = fgetc(matrix);
		//printf("c = %f\n", c-'0');
		if(feof(matrix)) {
			break;
		}
		if (c == ',') {
			j++;
		} else if (c == '\n') {
			i++;
			j = 0;
		} else {
			mat[i][j] = c-'0';
		}
	}
	fclose(matrix);
	//print_matrix(mat, 10, 200);
	float ** compr_mat; 
	compr_mat = refl_mat(mat, 200);
	print_matrix(compr_mat, 10, 10);
	free_mat(mat, 10);
	free_mat(compr_mat, 10);
	*/
}
