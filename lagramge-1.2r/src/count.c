#include <stdio.h>
#include <string.h>
#include <malloc/malloc.h>

#include "globals.h"
#include "prod.h"



double number_of_trees(Atom *a, int h);


/*
 * Maximal number of parse trees with root=a and height<=h
 */
double Number_of_trees(Atom *a, int h) {

	int i;
	double s;

	if (a->N[h] != -1) return(a->N[h]);
	for (i = 0, s = 0.0; i <= h; ++i) s += number_of_trees(a, i);
	return(a->N[h] = s);
}

/*
 * Maximal number of parse trees with root=a and height=h
 */
double number_of_trees(Atom *a, int h) {

	int nterm_rs;
	int i, j;
	double s, p1, p2;



	if (a->n[h] != -1) return(a->n[h]);

	if (a->type != NTERM_T) {
		if (h == 0) return(a->n[h] = 1.0);
		return(a->n[h] = 0.0);
	}

	if (h == 0) return(a->n[h] = 0);
	if (h == 1) {
		for (i = nterm_rs = 0; i < a->nprods; ++i)
			if (rs_nlen(a->prods[i]) == 0) ++nterm_rs;
		return(a->n[h] = (double) nterm_rs);
	}

	for (i = 0, s = 0.0; i < a->nprods; ++i) {
		for (j = 0, p1 = p2 = 1.0; j < a->prods[i]->rslen; ++j) {
			if ((eq_constrained(a->prods[i], j)) != -1) continue;
			p1 *= Number_of_trees(a->prods[i]->rs[j], h - 1);
			p2 *= Number_of_trees(a->prods[i]->rs[j], h - 2);
		}
		s += (p1 - p2);
	}
	return(a->n[h] = s);
}


/*
 * Maximal length of a derivated expression
 * in parse tree with root=a and height=h
 */
int max_derivated_word_length(Atom *a, int h) {

	int i, j, s, m, max;

	if (a->max_len[h] != -1) return(a->max_len[h]);
	if (a->type != NTERM_T) return(a->max_len[h] = 1);

	if (h == 0) return(-1);

	for (i = 0, max = -1; i < a->nprods; ++i) {
		for (j = 0, s = 0; j < a->prods[i]->rslen; ++j) {
			m = max_derivated_word_length(a->prods[i]->rs[j], h-1);
			if (m == -1) break;
			s += m;
		}
		if (j < a->prods[i]->rslen) continue;
		if (s > max) max = s;
	}
	return(a->max_len[h] = max);
}

/*
 * Minimal length of derivated expression
 * in parse tree with root=a and height=h
 */
int min_derivated_word_length(Atom *a, int h) {

	int i, j, k, s, m, min, flag;

	if (a->min_len[h] != -1) return(a->min_len[h]);
	if (a->type != NTERM_T) return(a->min_len[h] = ((h == 0) ? 1 : -1));

	if (h == 0) return(-1);

	for (i = 0, min = 1000; i < a->nprods; ++i) {
		for (j = s = flag = 0; j < a->prods[i]->rslen; ++j) {
			m = min_derivated_word_length(a->prods[i]->rs[j], h-1);
			if (m == -1) {
				for (k = h - 2; k >= 0; --k) {
					if ((m = min_derivated_word_length(a->prods[i]->rs[j], k)) != -1) break;
				}
			} else flag = 1;
			if (m == -1) {
				flag = 0;
				break;
			}
			s += m;
		}
		if (flag == 0) continue;
		if (s < min) min = s;
	}
	return(a->min_len[h] = ((min == 1000) ? -1 : min));
}


/*
 * Maximal number of "const" in derivated word
 * in parse tree with root=a and height=h
 */
int max_constants_number(Atom *a, int h) {

	int i, j, s, m, max;

	if (a->mnc[h] != -1) return(a->mnc[h]);
	if (a->type != NTERM_T) {
		if (a->type == CONST_T)
			return(a->mnc[h] = 1);
		else
			return(a->mnc[h] = 0);
	}

	if (h == 0) return(-1);

	for (i = 0, max = -1; i < a->nprods; ++i) {
		for (j = 0, s = 0; j < a->prods[i]->rslen; ++j) {
			m = max_constants_number(a->prods[i]->rs[j], h - 1);
			if (m == -1) break;
			s += m;
		}
		if (j < a->prods[i]->rslen) continue;
		if (s > max) max = s;
	}
	return(a->mnc[h] = max);
}


/*
 * Maximal derivation length (number of internal nodes)
 * in parse tree with root=a and height=h
 */
int max_derivation_length(Atom *a, int h) {

	int i, j, s, m, max;

	if (a->mdlen[h] != -1) return(a->mdlen[h]);

	if (h == 0) return(-1);
	if (h == 1) {
		for (i = 0; i < a->nprods; ++i)
			if (rs_nlen(a->prods[i]) == 0) return(a->mdlen[h] = 1);
		return(a->n[h] = 0);
	}

	for (i = 0, max = -1; i < a->nprods; ++i) {
		for (j = 0, s = 0; j < a->prods[i]->rslen; ++j)
			if (a->prods[i]->rs[j]->type == NTERM_T) {
				m = max_derivation_length(a->prods[i]->rs[j], h - 1);
				if (m == -1) break;
				s += m;
			}
		if (j < a->prods[i]->rslen) continue;
		if (s > max) max = s;
	}
	return(a->mdlen[h] = max + 1);
}


/*
 * calculates all the statistics for the CFG
 * max_depth is the maximal height of the parse trees
 */
void count(void) {

	int i, j;
	Atom *a;


	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a) {
		a->n = (double *) malloc(max_depth * sizeof(double));
		a->N = (double *) malloc(max_depth * sizeof(double));

		a->max_len = (int *) malloc(max_depth * sizeof(int));
		a->min_len = (int *) malloc(max_depth * sizeof(int));
		a->mdlen = (int *) malloc(max_depth * sizeof(int));
		a->mnc = (int *) malloc(max_depth * sizeof(int));

		for (j = 0; j < max_depth; ++j) {
			a->n[j] = a->N[j] = -1.0;
			a->max_len[j] = a->min_len[j] = -1;
			a->mdlen[j] = a->mnc[j] = -1;
		}
	}

	for (i = 0; i < max_depth; ++i) {
		gramm.atoms->N[i] = Number_of_trees(gramm.atoms, i);
		gramm.atoms->max_len[i] = max_derivated_word_length(gramm.atoms, i);
		gramm.atoms->min_len[i] = min_derivated_word_length(gramm.atoms, i);
		gramm.atoms->mdlen[i] = max_derivation_length(gramm.atoms, i);
		gramm.atoms->mnc[i] = max_constants_number(gramm.atoms, i);
	}
}


void print_count(void) {

	int i, j;
	Atom *a;

/*	for (a = gramm.atoms, j = 0; j < gramm.natoms; ++a, ++j) { */
	for (a = gramm.atoms, j = 0; j < 1; ++a, ++j) {

		printf("Atom %s:\n", a->name);

		printf("%5s", "depth");
		printf("%12s", "#p.trees");
		printf("%12s", "max.len.");
		printf("%12s", "min.len.");
		printf("%12s", "#consts");
		printf("%12s", "der.len.");
		printf("\n");

		for (i = 0; i < max_depth; ++i) {

			printf("%5d", i);
			printf("%12g", a->N[i]);
			printf("%12d", a->max_len[i]);
			printf("%12d", a->min_len[i]);
			printf("%12d", a->mnc[i]);
			printf("%12d", a->mdlen[i]);
			printf("\n");
		}
	}
}
