#include <math.h>


int _EQ(double *y, double **x, double **dx, int offset, int len, int n) {

	int i;

	for (i = offset; i < offset + len; ++i)
		y[i] = x[0][i];
	return(1);
}


int _Linear_1(double *y, double **x, double **dx, int offset, int len, int n) {

	int i;

	for (i = offset; i < offset + len; ++i)
		y[i] =  x[0][i];
	return(1);
}


int _Linear_2(double *y, double **x, double **dx, int offset, int len, int n) {

	int i;

	for (i = offset; i < offset + len; ++i)
		y[i] =  x[0][i] + x[2][i];
	return(1);
}


int _Term_1(double *y, double **x, double **dx, int offset, int len, int n) {

	int i;

	for (i = offset; i < offset + len; ++i)
		y[i] =  x[0][i];
	return(1);
}


int _Term_2(double *y, double **x, double **dx, int offset, int len, int n) {

	int i;

	for (i = offset; i < offset + len; ++i)
		y[i] =  x[0][i] * x[2][i];
	return(1);
}


int (*f[])(double *, double **, double **, int, int, int) = {_EQ, _Linear_1, _Linear_2, _Term_1, _Term_2};
