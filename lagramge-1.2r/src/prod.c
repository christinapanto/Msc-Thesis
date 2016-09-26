#include <stdio.h>

#include "gramm_t.h"



int rs_nlen(Prod *p) {

	int i, rsnlen;

	for (i = rsnlen = 0; i < p->rslen; ++i)
		if (p->rs[i]->type == NTERM_T) ++rsnlen;
	return(rsnlen);
}

int cmp_prods(const void *p1, const void *p2) {

	int dh = (*((Prod **)p1))->h - (*((Prod **)p2))->h;
	int dl = (*((Prod **)p1))->rslen - (*((Prod **)p2))->rslen;

	return((dh != 0) ? dh : dl);
}


void printf_prod(Prod *p) {

	int i;

	for (i = 0; i < p->rslen; ++i) printf(" %s", p->rs[i]->name);
}
