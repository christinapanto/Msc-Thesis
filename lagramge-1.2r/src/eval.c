#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc/malloc.h>
#include <math.h>

#include "globals.h"
#include "beam.h"
#include "ptree.h"



/*
 * evaluates the node in the parse tree for the whole data set
 */
void eval_ptnode(PTree *t, PTNode *n, int ne) {

	double **args;
	int i;


	if (n->atom->type == NTERM_T) {
		args = (double **) malloc(n->prod->rslen * sizeof(double *));

		for (i = 0; i < n->prod->rslen; ++i) {
			if (n->cl_flags[i] != 1) eval_ptnode(t, n->children[i], ne);
			args[i] = n->children[i]->values;
		}
		(void) (*n->prod->f)(n->values, args, NULL, 0, data.len, ne);

		free(args);
		return;
	}
	if (n->atom->type == VAR_T) {
		for (i = 0; i < data.len; ++i)
			n->values[i] = n->atom->values[i];
	}
	if (n->atom->type == CONST_T) {
		for (i = 0; i < data.len; ++i)
			n->values[i] = t->c[t->c_index];
		++t->c_index;
	}
}

/*
 * evaluates the parse tree
 */
void eval_ptree(PTree *t, int ne) {

	t->c_index = 0;
	eval_ptnode(t, t->nodes, ne);
}


/*
 * calculates the integral of the double array
 */
void integral(double *ix, double *x, double *startx) {
	
	double sum;
	int i, segment, slimit;

	for (segment = -1, i = 0; segment < data.nsegments; ++segment) {
		slimit = (segment == data.nsegments - 1) ? data.len : data.segments[segment + 1];
		sum = ix[i] = (startx == NULL) ? 0.0 : startx[i];
		for (++i; i < slimit; ++i)
			ix[i] = (sum += (x[i - 1] + x[i]) * time_step / 2);
	}
}


/*
 * calculates the integral of the double array
 * non-equdistant time points
 */
void integral_NE(double *ix, double *t, double *x, double *startx) {
	
	double sum;
	int i, segment, slimit;

	for (segment = -1, i = 0; segment < data.nsegments; ++segment) {
		slimit = (segment == data.nsegments - 1) ? data.len : data.segments[segment + 1];
		sum = ix[i] = (startx == NULL) ? 0.0 : startx[i];
		for (++i; i < slimit; ++i)
			ix[i] = (sum += (x[i - 1] + x[i]) * (t[i] - t[i - 1]) / 2);
	}
}


/*
 * calculate predicted values as needed by nonlinear regression package
 * for differential equation - equidistant time point
 *  !!! quasi-simulate the differential equation (calculate integrals)
 */
void madrj_DE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)()) {

	int ne;

	for (ne = 0; ne < ndvars; ++ne) {
		eval_ptree(&ptree, ne);
		integral(r + ne * data.len, ptree.nodes->values, data.vars[dvars[ne]].values);
	}
}

/*
 * for differential equation - nonequidistant time points
 */
void madrj_DE_NE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)()) {

	int ne;

	for (ne = 0; ne < ndvars; ++ne) {
		eval_ptree(&ptree, ne);
		integral_NE(r + ne * data.len, data.vars[tvar_index].values, ptree.nodes->values, data.vars[dvars[ne]].values);
	}
}


/*
 * calculate predicted values as needed by nonlinear regression package
 * for ordinary explicit equation
 */
void madrj_OEE(fortran_int *n, fortran_int *p, double *x, fortran_int *nf, fortran_int *need, double *r, double *rp, fortran_int *ui, double *ur, int (*uf)()) {

	int j;
	int ne;

	for (ne = 0; ne < ndvars; ++ne) {
		eval_ptree(&ptree, ne);
		for (j = 0; j < data.len; ++j) r[j + ne * data.len] = ptree.nodes->values[j];
	}
}


/*
 * calculate SSE as needed for nonlinear regression package
 * explicit equation - differential and ordinary
 */
void rhols_E(fortran_int *need, double *f, fortran_int *n, fortran_int *nf, double *xn, double *r, double *rp, fortran_int *ui, double *ur, double *w) {

	int j;
	double dist, sum, *y;
	int ne;

	if (need[0] == 1) {
		for (ne = 0, sum = 0.0; ne < ndvars; ++ne) {
			y = data.vars[dvars[ne]].values;
			for (j = 0; j < data.len; ++j) {
				dist = r[j + ne * data.len] - y[j];
				sum += dist * dist;
			}
		}
		*f = 0.5 * sum;
		if (isnan(*f) || isinf(*f)) *nf = 0;
	} else {
		for (ne = 0; ne < ndvars; ++ne) {
			y = data.vars[dvars[ne]].values;
			for (j = 0; j < data.len; ++j) {
				r[j + ne * data.len] = r[j + ne * data.len] - y[j];
				rp[j + ne * data.len] = (double) 1.0;
			}
		}
	}
}



void count_constants(PTree *t, PTNode *n) {

	int i;


	if (n->atom->type == CONST_T) t->c_index++;
	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			if (n->cl_flags[i] != 1) count_constants(t, n->children[i]);
	}
}

void initialize_constants(PTree *t, PTNode *n, double *nr_bounds) {

	int i;


	if (n->atom->type == CONST_T) {
		nr_bounds[2 * t->c_index] = n->atom->lower;
		nr_bounds[2 * t->c_index + 1] = n->atom->upper;

		if (multistart == 0)
			t->c[t->c_index++] = n->atom->init;
		else
			t->c[t->c_index++] = n->atom->lower + (n->atom->upper - n->atom->lower) * ((double) rand()) / ((double) RAND_MAX);
	}
	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			if (n->cl_flags[i] != 1) initialize_constants(t, n->children[i], nr_bounds);
	}
}

void R_load_constants(PTree *t, PTNode *n, double *c) {

	int i;


	if (n->atom->type == CONST_T) {
		t->c[t->c_index] = c[t->c_index];
		t->c_index++;
	}
	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			if (n->cl_flags[i] != 1) R_load_constants(t, n->children[i], c);
	}
}

void load_constants(PTree *t, double *c) {

	t->c_index = 0;
	R_load_constants(t, t->nodes, c);
}

void R_save_constants(PTree *t, PTNode *n, double *c) {

	int i;


	if (n->atom->type == CONST_T) {
		c[t->c_index] = t->c[t->c_index];
		t->c_index++;
	}
	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			if (n->cl_flags[i] != 1) R_save_constants(t, n->children[i], c);
	}
}

void save_constants(PTree *t, double *c) {

	t->c_index = 0;
	R_save_constants(t, t->nodes, c);
}



/*
 * envoke nonlinear fit
 */
void nonlinear_fit(void) {

	extern void dglfb_();
	double *nr_bounds, *nr_v;
	fortran_int *nr_iv;
	fortran_int nr_lv, nr_liv;

	int nc, data_len, i;
	double dist, mse;
	int ne;


	/*
	 * count how many constants are there
	 */
	ptree.c_index = 0;
	count_constants(&ptree, ptree.nodes);
	nc = ptree.c_index;

	data_len = data.len * ndvars;

	/*
	 * alloc arrays for nonlinear fit procedure
	 */
	nr_bounds = (double *) malloc(2 * nc * sizeof(double));
	nr_lv = data_len * (nc + 6) + 4 * nc + nc * (2 * nc + 20) + 105;
	nr_v = (double *) calloc(nr_lv, sizeof(double));
	nr_liv =  5 * nc + 82;
	nr_iv = (fortran_int *) calloc(nr_liv, sizeof(fortran_int));

	/*
	 * initialize contstants and contstraints on their values
	 */
	ptree.c_index = 0;
	initialize_constants(&ptree, ptree.nodes, nr_bounds);
	ptree.mse = 0.0;

	/*
	 * default settings of the nonlinear fit procedure
	 */
	nr_iv[0] = nr_iv[1] = 0;

	/*
	 * envoke fit
	 */
	dglfb_(&(data_len), &nc, &nc, ptree.c, nr_bounds, nr_rhols, NULL, NULL, nr_iv, &nr_liv, &nr_lv, nr_v, nr_madrj, NULL, NULL, NULL);

	/*
	 * evaluate ptree with optimal parameter values
	 */
	nr_madrj((fortran_int *) &(data_len), (fortran_int *) &nc, ptree.c, NULL, NULL, ptree.yhat, NULL, NULL, NULL, NULL);

	/*
	 * calculate discrepance between simulated and given value
	 */
	mse = 0.0;
	for (ne = 0; ne < ndvars; ++ne) {
		for (i = 0; i < data.len; ++i) {
			dist = ptree.yhat[i + ne * data.len] - data.vars[dvars[ne]].values[i];
			mse += dist * dist;
		}
	}
	mse = mse / data.len;
	ptree.mse += mse;

	/*
	 * free allocated space
	 */
	free((void *) nr_bounds);
	free((void *) nr_v);
	free((void *) nr_iv);
}


void fit(int first_flag) {

	int i, tmp;
	double min_mse;
	double c[1024]; /* to be allocated in init in the next release */

	if (multistart > 0) {
		tmp = multistart;

		multistart = 0;
		nonlinear_fit();
		multistart = tmp;

		min_mse = ptree.mse;
		save_constants(&ptree, c);

		for (i = 0; i < multistart; ++i) {
			nonlinear_fit();
			if (ptree.mse < min_mse) {
				min_mse = ptree.mse;
				save_constants(&ptree, c);
			}
		}

		if (ptree.mse > min_mse) {
			load_constants(&ptree, c);
			ptree.mse = min_mse;
		}

	} else nonlinear_fit();

	ptree.mdl = ptree.mse + ((first_flag == 0) ? first_ptree.mse : ptree.mse) * length(&ptree) / (10.0 * max_length);
}



/*
 * fit the constants of the parse tree
 */
int fit_ptree(int first_flag) {

	fit(first_flag);

	if (search_heuristic == 0)
		return((ptree.mse < beam.elements[0].ptree.mse) ? 1 : 0);
	else
		return((ptree.mdl < beam.elements[0].ptree.mdl) ? 1 : 0);
}
