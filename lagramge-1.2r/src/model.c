#include <stdio.h>

#include "error.h"
#include "globals.h"
#include "ptree.h"


/*
 * print all the models in the beam
 */
void print_models(char *model_fname, Beam b) {

	FILE *fm;
	int i;
	BElement *be;


	fm = fopen(model_fname, "w");

	fprintf(fm, "%s", gramm_fname);
	fprintf(fm, " %d", max_depth);
	fprintf(fm , " %d", ndvars);
	for (i = 0; i < ndvars; ++i)
		fprintf(fm, " %s", data.vars[dvars[i]].name);
	fprintf(fm, " %g", time_step);
	if (time_step == -1.0) fprintf(fm, " %s", data.vars[tvar_index].name);
	fprintf(fm, "\n");

	for (be = b.elements + b.nelements - 1; be >= b.elements; --be) {
		if (be->ptree.mse == 1e+100) break;

		print_ptree_model(fm, &(be->ptree));
		fprintf(fm, "-1\n");
	}
	fclose(fm);
}


/*
void restore_model(char *model_fname) {

	FILE *fm;

	
	if ((fm = fopen(model_fname, "r")) == NULL) eerror(OPEN_FILE, model_fname);

	init_ptree(&ptree, gramm.atoms);
	restore_ptree_model(fm, &ptree, ptree.nodes);

	fclose(fm);
}
*/
