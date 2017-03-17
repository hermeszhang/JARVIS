#include "framework.h"

/*
Functions to perform circular shifting of coefficient vectors.
To avoid loss of data, we need to save old coefficients (in
case of late triggering).
*/ 

circ_buff(float coeff[], float old[], float older[], float oldest[])
{
	int n;
	for(n=0;n<SIZE;++n) { //circular coeff buffer
   			oldest[n] = older[n];
   			older[n] = old[n];
   			old[n] = coeff[n];
	}
}

fill_K(float (*K)[MAX_BLOCK_SIZE], float old[], float older[], float oldest[])
{
	int n;
	for(n=0;n<SIZE;++n) { //circular coeff buffer
   			K[n][0] = oldest[n];
   			K[n][1] = older[n];
   			K[n][2] = old[n];
	}
}
