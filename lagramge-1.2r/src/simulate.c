#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "load.h"
#include "globals.h"
#include "count.h"
#include "eval.h"
#include "init.h"
#include "error.h"
#include "beam.h"
#include "prod.h"
#include "model.h"

extern void load_gramm(char *fname);


extern double **xargs, **dp, *c, **cp;
int dp_free;



/*
 * prints out derivated expression
 */
void printf_term(DStep *ds) {

	int i, j;
	DAtom *da;


	for (i = j = 0, da = ds->datoms; i < ds->len; ++i, ++da)
		if (da->atom->type == CONST_T)
			printf("%g ", c[j++]);
		else
			printf("%s ", da->atom->name);
}


void print_gramm(void) {

	Atom *a;
	int i, j;

	printf("Grammar:\n");
	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a) {
		if (a->nprods == 0) continue;
		if (a->name[0] == 'V' && (a->name[1] == '_' || a->name[1] == 0)) continue;

		printf("%s ->", a->name);
		for (j = 0; j < a->nprods; ++j) {
			printf_prod(a->prods[j]);
			printf("%s", j < a->nprods - 1 ? " |" : ";\n");
		}
	}
	printf("\n");
}


/*
 * evaluates the non-teriminal expanded in the previous derivation step
 * calculates the function values only (without derivatives)
 */
void eval_dstep(DStep *ds, int offset, int len) {

	int j;
	DStep *pds = ds - 1;
	DAtom *da;


	for (da = ds->datoms; da < ds->rs; ++da)
		da->pred->values = da->values;

	for (j = 0, da = ds->rs; j < ds->rslen; ++j, ++da)
		xargs[j] = da->values;
	pds->ls->values = dp[dp_free];

	if ((*pds->prod->f)(&(pds->ls->values), xargs, NULL, offset, len))
		++dp_free;

	for (da = ds->datoms + ds->len - 1; da >= ds->rs + ds->rslen; --da)
		da->pred->values = da->values;
}


/*
 * evaluates the parse tree
 */
void eval_derivation_tree(int nc) {

	int i, j;
	DStep *ds, *dstep = deriv.steps + deriv.len - 1;
	DAtom *da;

	for (i = 0, da = dstep->datoms; i < nc; ++da) {
		if (da->atom->type == CONST_T) continue;

		cp[i] = da->values = dp[i++];
	}
	
	dp_free = nc;
	for (i = 0; i < nc; ++i)
		for (j = 0; j < data.len; ++j) cp[i][j] = c[i];

	for (ds = dstep; ds > deriv.steps; --ds)
		eval_dstep(ds, 0, data.len);
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





int main(int argc, char **argv) {

	FILE *fm;
	char buffer[256], *dvar_name, *tvar_name = NULL, ch;
	int i, npis, *pis, nc, errflag = 0, neqn = 0, eqn = 1, error_flag = 0;
	double *yhat, diff, error_mse;


	while ((ch = getopt(argc, argv, "Ee:")) != -1)
		switch(ch) {
			case 'E':
				error_flag = 1;
				break;

			case 'e':
				eqn = atoi(optarg);
				break;

			case '?':
				++errflag;
		}
	if (errflag || (optind == argc) || (optind == argc - 1))
		eerror(USAGE_S, argv[0]);

	model_fname = argv[optind];
	if ((fm = fopen(model_fname, "r")) == NULL)
		eerror(OPEN_FILE, model_fname);

	fscanf(fm, "%[^ ]", buffer);
	gramm_fname = strdup(buffer);

	fscanf(fm, " %d", &max_depth);

	fscanf(fm, " %[^ ]", buffer);
	dvar_name = strdup(buffer);

	fscanf(fm, " %lg", &time_step);
	if (time_step == -1.0) {
		fscanf(fm, " %[^ \n]", buffer);
		tvar_name = strdup(buffer);
	}

	data.nsegments = -1;
	printf("Data files:");
	for (++optind; optind < argc; ++optind) {
		load_data(argv[optind]);
		printf(" %s", argv[optind]);
	}
	printf("\n");

	printf("Variables: %s", data.vars[0].name);
	for (i = 1; i < data.nvars; ++i) printf(" %s", data.vars[i].name);
	printf("\n");
	printf("Data length: %d\n\n", data.len);

	for (i = 0; i < data.nvars; ++i)
		if (!strcmp(data.vars[i].name, dvar_name)) break;
	if (i == data.nvars) eerror(VAR_NAME, dvar_name);
	dvar_index = i;
	
	if (time_step == -1.0) {
		for (i = 0; i < data.nvars; ++i)
			if (!strcmp(data.vars[i].name, tvar_name)) break;
		if (i == data.nvars) eerror(VAR_NAME, tvar_name);
		tvar_index = i;
	}
	
	printf("Grammar definition file: %s\n", gramm_fname);
	load_gramm(gramm_fname);
	print_gramm();

	printf("Equation type: ");
	if (time_step == 0.0)
		printf("ordinary implicit (%s)\n", dvar_name);
	else if (time_step > 0.0)
		printf("differential (%s, h = %g)\n", dvar_name, time_step);
	else
		printf("differential (%s, time_variable = %s)\n", dvar_name, tvar_name);
	fflush(stdout);

	count();
	init(0);

	while(fscanf(fm, "%d", &npis) != EOF) {

		pis = (int *) malloc(npis * sizeof(int));
		for (i = 0; i < npis; ++i) fscanf(fm, " %d", pis + i);

		fscanf(fm, "%d", &nc);
		for (i = 0; i < nc; ++i) fscanf(fm, " %lg", c + i);

		if (++neqn != eqn) continue;

		model_restore(npis, pis);
		printf("Equation: ");
		printf("%s%s = ", dvar_name, (time_step == 0.0) ? "" : "'");
		printf_term(deriv.steps + deriv.len - 1);
		printf("\n\n");


		if (error_flag == 1)
			printf("Predicted, given values and error:\n");
		else
			printf("Predicted values:\n");

#define YHAT deriv.steps->datoms->values
#define Y data.vars[dvar_index].values
#define T data.vars[tvar_index].values

		eval_derivation_tree(nc);
		if (time_step == 0.0) yhat = YHAT;
		else {
			yhat = (double *) malloc(data.len * sizeof(double));
			if (time_step > 0.0)
				integral(yhat, YHAT, Y);
			else integral_NE(yhat, T, YHAT, Y);
		}

		for (i = 0, error_mse = 0.0; i < data.len; ++i) {
			diff = yhat[i] - Y[i];
			error_mse += diff * diff;

			printf("%g", yhat[i]);
			if (error_flag == 1) printf(" %g %g", Y[i], diff);
			printf("\n");
		}
		printf("\nMean squred error: %g\n", error_mse / data.len);
		free((void *) pis);
		free((void *) yhat);

		break;
	}

	fclose(fm);
	exit(0);
}
