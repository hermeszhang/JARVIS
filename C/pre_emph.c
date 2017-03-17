float pre_emph(float* x, int N, float last_data, float* y)
{	
	//x data, N size of data vector, last_data last data in previous data block
	y[0] = x[0] - last_data;
	int i;
	for(i=1; i<N; i++){
		y[i] = x[i] - x[i-1];
	}
	return x[N-1]; 
}
