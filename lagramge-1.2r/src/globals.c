#include <stdio.h>

#include "gramm_t.h"
#include "ptree_t.h"
#include "data_t.h"
#include "beam_t.h"
#include "globals.h"



Data data;

Gramm gramm;
PTree first_ptree, ptree;
Beam beam, old_beam;

int max_length;
char *eqn_head;


/*
 * nonlinear regression functions
 */
void (*nr_madrj)(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());

void (*nr_rhols)(fortran_int *need, double *f, fortran_int *n, fortran_int *nf, double *xn, double *r, double *rp, fortran_int *ui, double *ur, double *w);


/*
 * Lagramge's parameters
 */
char *gramm_fname = NULL;
int max_depth = 5;

int search_strategy;
int beam_width = 25;

int search_heuristic;

int verbose_flag;

int test_comm_flag;

int stopping_criterion;
int time_limit = 10;
double treshold = 1e-5;

int ndvars = 0;
int *dvars;

double time_step = 0.0;
int tvar_index = -1;

char *model_fname = NULL;

int multistart = 0;
