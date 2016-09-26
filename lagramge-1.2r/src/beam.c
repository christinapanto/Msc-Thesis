#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "beam_t.h"
#include "ptree.h"
#include "globals.h"



void print_beam(Beam b) {

	BElement *be;


	for (be = b.elements + b.nelements - 1; be >= b.elements; --be) {
		if (be->ptree.mse == 1e100) continue;

		printf("%s =", eqn_head);
		print_ptree(&(be->ptree), 1);

		printf(" {MSE = %g, MDL = %g}\n", be->ptree.mse, be->ptree.mdl);
	}
}


int sign(double x) {

	return((x == 0.0) ? 0 : ((x < 0.0) ? -1 : 1));
}

int cmp_errors(const void *p1, const void *p2) {
	
	int mse_diff = sign(((BElement *)p2)->ptree.mse - ((BElement *)p1)->ptree.mse);
	int mdl_diff = sign(((BElement *)p2)->ptree.mdl - ((BElement *)p1)->ptree.mdl);

	if (search_heuristic == 0)
		return ((mse_diff != 0) ? mse_diff : mdl_diff);
	else
		return ((mdl_diff != 0) ? mdl_diff : mse_diff);
		
}


void cp_to_beam(PTree *t) {


	cp_ptree(&(beam.elements[0].ptree), t);
	beam.elements[0].successors_flag = 0;

	qsort(beam.elements, beam.nelements, sizeof(BElement), cmp_errors);
}


void cp_from_beam(PTree *t, BElement *be) {

	cp_ptree(t, &(be->ptree));
}


BElement *in_beam(PTree *t) {

	int i;
	BElement *be;


	for (i = 0, be = beam.elements; i < beam.nelements; ++i, ++be) {
		if (be->ptree.mse == 1e100) continue;

		if (eq_ptrees(t, &(be->ptree)) == 1) return(be);
	}
	return((BElement *) NULL);
}


/*
 * makes a copy of a beam
 * needed at the beginning of each beam search iteration
 */
void save_beam(void) {

	int i;
	BElement *sbe, *dbe;


	for (i = 0, sbe = beam.elements, dbe = old_beam.elements; i < beam.nelements; ++i, ++sbe, ++dbe) {
		if (sbe->ptree.mse == 1e+100) continue;

		cp_ptree(&(dbe->ptree), &(sbe->ptree));
		dbe->successors_flag = sbe->successors_flag;

		sbe->successors_flag = 1;
	}
}



/*
void cp_in_beam_storage(BElement *be) {

	int i;

	(beam.elements - 1)->deriv.len = be->deriv.len;
	for (i = 0; i < deriv.len; ++i)
		cp_ds(be->deriv.steps + i, (beam.elements - 1)->deriv.steps + i);
}
*/
