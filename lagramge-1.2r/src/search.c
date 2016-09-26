#include <stdio.h>
#include <string.h>
#include <malloc/malloc.h>

#include "globals.h"
#include "ptree.h"
#include "eval.h"
#include "beam.h"


/*
 * prints out the equation
 */
void print_equation(int index, char *head, int cf) {


	printf("%d: %s =", index, head);
	print_ptree(&ptree, cf);
	if (cf == 1) printf(" {MSE = %g, MDL = %g}", ptree.mse, ptree.mdl);
	printf("\n");
	fflush(stdout);
}



int improvable_node(PTNode *n) {


	if (n->atom->type != NTERM_T) return(0);
	if (n->prod_index + 1 == n->atom->nprods) return(0);
	if (n->h + n->atom->prods[n->prod_index + 1]->h >= max_depth) return(0);

	return(1);
}



/*
 * exhaustive search:
 * enumerate all parse trees with limited depth
 */
void exhaustive_search(PTree *t) {

	unsigned long int ntrees = 0;
	int i;


	while(1) {

		++ntrees;

		if (fit_ptree(0) == 1) cp_to_beam(t);
		if (verbose_flag) {
			print_equation(ntrees, eqn_head, 1);
			draw_ptree(t, 0);
		}

		preorder_lr(t);
		for (i = t->nnodes - 1; i >= 0; --i) {
			if (improvable_node(t->preorder_lr[i]) == 1) break;
			remove_ptnode(t, t->preorder_lr[i]);
		}
		if (i < 0) break;

		improve_ptnode(t, t->preorder_lr[i]);
	}
	printf("\n%lu parse trees evaluated\n", ntrees);
}


void beam_search(PTree *t) {

	unsigned long int ntrees = 0;
	int i, j, new_flag;
	BElement *be;


	cp_to_beam(&first_ptree);

	do {
		if (verbose_flag) {
			printf("Current beam:\n");
			print_beam(beam);
			printf("\n");
		}
		save_beam();

		new_flag = 0;
		for (i = 0, be = old_beam.elements; i < old_beam.nelements; ++i, ++be) {
			if (be->ptree.mse == 1e+100) continue;
			if (be->successors_flag == 1) continue;

			for (j = 0; j < be->ptree.nnodes; ++j) {
				if (improvable_node(be->ptree.preorder_lr[j]) == 0) continue;
				cp_from_beam(t, be);

				improve_ptnode_b(t, t->preorder_lr[j]);

				if (in_beam(t) != NULL) continue;

				++ntrees;
				if (fit_ptree(0) == 1) {
					cp_to_beam(t);
					new_flag = 1;
				}
			}
		}
	} while(new_flag == 1);

	printf("\n%lu parse trees evaluated\n", ntrees);
}


void best_first_search(void) {
}



void search(void) {


	init_ptree(&ptree, gramm.atoms);
	terminate_ptnode(&ptree, ptree.nodes);

	fit_ptree(1);
	cp_ptree(&first_ptree, &ptree);

	max_length = gramm.atoms->max_len[max_depth - 1];

	switch(search_strategy) {
		case 0: exhaustive_search(&ptree); break;
		case 1: beam_search(&ptree); break;
		case 2: best_first_search();
	}
}
