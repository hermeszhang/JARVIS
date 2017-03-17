#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "xcorr.h"
#include "schur.h"

void schur(float* data, float* ref_coeff, int N, int n)
{
	/*
	Function to calculate reflection coefficients using the Schur algorithm. 
	Assumes data is provided in blocks of size N and that no buffering is 
	needed. The function returns n reflection coefficients.
	*/
	int i, j, k, l;		//Loop counters
	float VG[2][11] = {0}; 	//Declaration of matrix mult
	float R[n+1];		//Vector to contain autocorrelation
	//Autocorrelations of the data sequence
	xcorr(data, R, N, n+1);
	
	//Reflection coefficients

	// Initialize G matrix
	float G[2][11];
	G[0][0] = 0.0;
	G[1][0] = 0.0;

	for(i = 1; i < n+1; i++) {
		G[0][i] = R[i];
	}
	for(i = 0; i < n; i++) {
		G[1][i+1] = R[i];
	}
	//print_matrix(G, 2, 11);
	//Initialize Schur algorithm
	ref_coeff[0] = -G[0][1]/G[1][1];
	//print_matrix(G, 2, 11);
	float V[2][2] = {0};
	V[0][0] = 1;
	V[1][1] = 1;
	//Run algorithm
	for(i = 1; i < n; i++) {
		//Generate and initialize V matrix
		V[0][1]	= ref_coeff[i-1];
		V[1][0] = ref_coeff[i-1];
		//Calculate V*G
		//print_matrix(V, 2, 2);
		matrix_mult(V, G, VG);
		//Generate new G matrix
		G[1][0] = 0.0;
		for(j = 0; j <= n; j++){
			//float tmp1 = VG[0][j];
			G[0][j] = VG[0][j];
		}
		for(j = 1; j <= n; j++){
			//float tmp2 = VG[1][j-1];
			G[1][j] = VG[1][j-1];
		}
		ref_coeff[i] = -G[0][i+1]/G[1][i+1];
		
		//Reset VG matrix
		for(k = 0; k < 2; k++) {
			for(l = 0; l < 11; l++) {
				VG[k][l] = 0.0;
			}
		}
	}
}

void matrix_mult(float(*A)[2], float(*B)[11], float(*res)[11])
//void matrix_mult(float(*A)[2], float(*B)[2], float(*res)[2])
{
	/* 
	Function that performs matrix multiplication AB. a is the number 
	of rows in A and b is the number of columns i B. The dimension of the
	resulting matrix is a X b. Assumes dimension match.
	*/
	int rows = 2;
	int cols = 11;
	int i,j,k;
//	print_matrix(A, 2, 2);
//	print_matrix(B, 2, 11);
	for(i = 0; i < rows; i++) {
		for(j = 0; j < rows; j++) {
			for(k = 0; k < cols; k++) {
				//res[i*rows + k] += A[i*rows + j]*B[j*rows +k];
				res[i][k] += A[i][j]*B[j][k];
			}
		}
	}
}

void print_matrix(int m, int n, int cols, float(*mat)[cols])
//void print_matrix(float(*mat)[2], int m, int n)
{
	int i,j;
	printf("\n");
	for(i = 0; i < m; i++){
		for(j = 0; j<n; j++){
			printf("%f\t", mat[i][j]);
		}
		printf("\n");
	}
}
