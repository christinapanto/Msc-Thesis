#include "gramm_t.h"
#include "ptree_t.h"
#include "data_t.h"
#include "beam_t.h"

/* on 64-bit machines fortran_int should be int */
#define fortran_int long int



extern Data data;

extern Gramm gramm;
extern PTree first_ptree, ptree;
extern Beam beam, old_beam;

extern int max_length;
extern char *eqn_head;


/*
 * nonlinear regression functions
 */
extern void (*nr_madrj)(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)());

extern void (*nr_rhols)(fortran_int *need, double *f, fortran_int *n, fortran_int *nf, double *xn, double *r, double *rp, fortran_int *ui, double *ur, double *w);


/*
 * Lagramge's parameters
 */

extern char *gramm_fname;
extern int max_depth;

extern int search_strategy;
extern int beam_width;

extern int search_heuristic;

extern int verbose_flag;

extern int test_comm_flag;

extern int stopping_criterion;
extern int time_limit;
extern double treshold;

extern int ndvars;
extern int *dvars;

extern double time_step;
extern int tvar_index;

extern char *model_fname;

extern int multistart;
