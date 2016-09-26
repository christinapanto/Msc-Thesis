#include <stdio.h>
#include <stdlib.h>
#include <malloc/malloc.h>

#include "globals.h"
#include "error.h"
#include "prod.h"



/*
 * calculates a height of the production=p:
 * minimal height of a parse tree with p used in root, terminals in leaves
 */
int prod_h(Prod *p) {

	int i, max_h;

	for (i = 0, max_h = -1; i < p->rslen; ++i) {
		if (p->rs[i]->h == -1) return(-1);
		if (p->rs[i]->h > max_h) max_h = p->rs[i]->h;
	}
	return(p->h = max_h + 1);
}

/*
 * calculates a height of the atom=a:
 * minimal height of parse tree with root a and terminals in leaves
 * level prevents infinite recursions
 */
int atom_h(Atom *a, int level) {

	int i, min_h;

	for (i = 0, min_h = 10000; i < a->nprods; ++i) {
		if (a->prods[i]->h == level) return(a->h = level);
		if (a->prods[i]->h < min_h) min_h = a->prods[i]->h;
	}
	if (min_h == 10000) return(a->h = -1); else return(a->h = min_h);
}

/*
 * calculates the height of all atoms and productions in CFG
 * sorts the productions of all atoms in ascending height order
 */
void height(void) {

	int i, level, n;
	Prod *p;
	Atom *a;


	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a)
		if (a->type != NTERM_T) a->h = 0; else a->h = -1;
	for (i = 0, p = gramm.prods; i < gramm.nprods; ++i, ++p) p->h = -1;

	for (level = n = 1; n == 1; ++level) {
		for (i = n = 0, p = gramm.prods; i < gramm.nprods; ++i, ++p) {
			if (p->h != -1) continue;
			if (prod_h(p) != -1) n = 1;
		}

		for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a) {
			if (a->h != -1) continue;
			if (atom_h(a, level) != -1) n = 1;
		}
	}

	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a)
		qsort(a->prods, a->nprods, sizeof(Prod *), cmp_prods);
}



/*
 * Allocate memory for data structures
 */
#define ALLOC(array, len, type, ptype, errstr) \
	if ((array = ptype malloc((len) * sizeof(type))) == NULL) \
		eerror(MEMORY, errstr)

void memalloc_ptree(PTree *p, int max_plen, int max_nc, char *s) {

	int i, j;

	ALLOC(p->nodes, p->max_nnodes, PTNode, (PTNode *), s);
	ALLOC(p->preorder_lr, p->max_nnodes, PTNode *, (PTNode **), s);
	ALLOC(p->postorder_lr, p->max_nnodes, PTNode *, (PTNode **), s);
	for (i = 0; i < p->max_nnodes; ++i) {
		ALLOC(p->nodes[i].children, max_plen, PTNode *, (PTNode **), s);
		for (j = 0; j < max_plen; ++j) p->nodes[i].children[j] = (PTNode *) NULL;
		ALLOC(p->nodes[i].cl_flags, max_plen, int, (int *), s);
		ALLOC(p->nodes[i].values, data.len, double, (double *), s);
	}
	ALLOC(p->c, max_nc, double, (double *), s);
	ALLOC(p->yhat, ndvars * data.len, double, (double *), s);

	p->nnodes = 0;
	p->mse = p->mdl = 1e+100;
}

void memalloc_beam(Beam *b, int max_plen, int max_nc) {

	int i;
	BElement *be;

	ALLOC(b->elements, b->nelements, BElement, (BElement *), "beam");
	for (i = 0, be = b->elements; i < b->nelements; ++i, ++be) {
		be->ptree.max_nnodes = ptree.max_nnodes;
		memalloc_ptree(&(be->ptree), max_plen, max_nc, "beam - parse trees");
		be->successors_flag = 0;
	}
}


void memalloc(int bflag) {

	int i, max_plen, max_dlen, max_wlen, max_nc;


	max_wlen = gramm.atoms->max_len[max_depth - 1];
	max_dlen = gramm.atoms->mdlen[max_depth - 1] + 1;
	max_nc = gramm.atoms->mnc[max_depth - 1];

	first_ptree.max_nnodes = ptree.max_nnodes = 10 * (max_wlen + max_dlen);

	for (max_plen = i = 0; i < gramm.nprods; ++i)
		if (gramm.prods[i].rslen > max_plen) max_plen = gramm.prods[i].rslen;
	gramm.max_plen = max_plen;

	memalloc_ptree(&ptree, max_plen, max_nc, "main parse tree");
	memalloc_ptree(&first_ptree, max_plen, max_nc, "first (initial) parse tree");

	if (bflag != 0) {
		memalloc_beam(&beam, max_plen, max_nc);
		memalloc_beam(&old_beam, max_plen, max_nc);
	} else {
		/* reserve the possition -1 in beam for tmp. storage */
		++beam.nelements;
		memalloc_beam(&beam, max_plen, max_nc);
		--beam.nelements;
		++beam.elements;
	}
}

/*
 * Initialize first derivation step
 */
void init(int bflag) {

	height();
	memalloc(bflag);
}
