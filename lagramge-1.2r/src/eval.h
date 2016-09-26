#include "globals.h"



extern int fit_ptree(int first_flag);


extern void madrj_DE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());

extern void madrj_DE_NE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());

extern void madrj_OEE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());

extern void madrj_OIE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());


extern void rhols_E(fortran_int *need, double *f, fortran_int *n, fortran_int *nf, double *xn, double *r, double *rp, fortran_int *ui, double *ur, double *w);

extern void rhols_I(fortran_int *need, double *f, fortran_int *n, fortran_int *nf, double *xn, double *r, double *rp, fortran_int *ui, double *ur, double *w);
