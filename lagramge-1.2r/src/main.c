#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/param.h>
#include <signal.h>
#include <malloc/malloc.h>
#include <unistd.h>
#include <ctype.h>

#include "load.h"
#include "globals.h"
#include "count.h"
#include "search.h"
#include "eval.h"
#include "init.h"
#include "error.h"
#include "beam.h"
#include "prod.h"
#include "ptree.h"
#include "model.h"
#include "realloc.h"


extern void load_gramm(char *fname);

extern char *optarg;
extern int optind;
extern int derivatives_flag;

struct tms time_buffer;
clock_t btime, etime;



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


void print_results(int ignore) {


	printf("\n\nBest equations:\n");
	print_beam(beam);
	printf("\n");

	etime = times(&time_buffer);
	etime = (etime == 0) ? time_buffer.tms_utime : etime;
	printf("Time elapsed: %g [s]\n", (double) (etime - btime) / sysconf(_SC_CLK_TCK));

	if (model_fname != NULL) print_models(model_fname, beam);

	exit(0);
}


int main(int argc, char **argv) {

	char c, *dvar_names[1024], *tvar_name = NULL, buffer[1024];
	int i, j, t_flag = 0, errflag = 0;


	btime = times(&time_buffer);
	btime = (btime == 0) ? time_buffer.tms_utime : btime;
	while ((c = getopt(argc, argv, "TCVg:d:v:s:b:h:f:c:e:l:i:t:o:m:r:")) != -1)
		switch (c) {
			case 'g':
				gramm_fname = (char *) malloc((strlen(HOME_DIR) + strlen("/lib/") + strlen(optarg) + 1) * sizeof(char));
				sprintf(gramm_fname, "%s/lib/%s", HOME_DIR, optarg);
				break;

			case 'v':
				dvars = (int *) realloc_array(dvars, ndvars, 50, sizeof(int));
				if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
					dvars[ndvars] = atoi(optarg);
					if (dvars[ndvars] < 0) eerror(VAR_NAME, optarg);
				}
				else {
					dvars[ndvars] = -1;
					dvar_names[ndvars] = strdup(optarg);
				}
				++ndvars;
				break;

			case 't':
				if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
					tvar_index = atoi(optarg);
					if (tvar_index < 0) eerror(VAR_NAME, optarg);
				}
				else {
					tvar_index = -2;
					tvar_name = strdup(optarg);
				}
				time_step = -1.0;
				break;

			case 'i':
				time_step = atof(optarg);
				break;

			case 'e':
				treshold = atof(optarg);
				break;

			case 'l':
				time_limit = atoi(optarg);
				break;

			case 'c':
				if (strcmp(optarg, "none") == 0) stopping_criterion = 0;
				else if (strcmp(optarg, "timelimit") == 0) stopping_criterion = 1;
				else if (strcmp(optarg, "treshold") == 0) stopping_criterion = 2;
				else eerror(STOPPING, optarg);
				break;

			case 's':
				if (strcmp(optarg, "exhaustive") == 0) search_strategy = 0;
				else if (strcmp(optarg, "beam") == 0) search_strategy = 1;
				else if (strcmp(optarg, "bestfirst") == 0) search_strategy = 2;
				else eerror(STRATEGY, optarg);
				break;

			case 'b':
				beam_width = atoi(optarg);
				if (beam_width < 1) eerror(BEAM, optarg);
				break;

			case 'd':
				max_depth = atoi(optarg);
				if (max_depth < 1) eerror(DEPTH, optarg);
				break;

			case 'h':
				if (strcmp(optarg, "mse") == 0) search_heuristic = 0;
				else if (strcmp(optarg, "mdl") == 0) search_heuristic = 1;
				else eerror(HEURISTIC, optarg);
				break;

			case 'o':
				model_fname = strdup(optarg);
				break;

			case 'm':
				multistart = atoi(optarg);
				if (multistart < 0) eerror(MULTISTART, optarg);
				break;

			case 'r':
				srand(atoi(optarg));
				break;

			case 'C':
				test_comm_flag = 1;
				break;

			case 'V':
				verbose_flag = 1;
				break;

			case 'T':
				t_flag = 1;
				break;

			case '?': ++errflag;
		}
	if (errflag || (optind == argc)) eerror(USAGE_G, argv[0]);

	/* adjust and propagate parameters */
	++max_depth;
	beam.nelements = old_beam.nelements = beam_width;

	/* read the data set(s) */
	data.nsegments = -1;
	printf("Data files:");
	for ( ; optind < argc; ++optind) {
		load_data(argv[optind]);
		printf(" %s", argv[optind]);
	}
	printf("\n");

	/* print out data set(s) basic facts */
	printf("Variables: %s", data.vars[0].name);
	for (i = 1; i < data.nvars; ++i) printf(" %s", data.vars[i].name);
	printf("\n");
	printf("Data length: %d\n\n", data.len);

	/* check dependent variables */
	for (i = 0; i < ndvars; ++i) {
		if (dvars[i] != -1) continue;

		for (j = 0; j < data.nvars; ++j)
			if (!strcmp(data.vars[j].name, dvar_names[i])) break;
		if (j == data.nvars) eerror(VAR_NAME, dvar_names[i]);
		dvars[i] = j;
	}

	/* check time variable */
	if (tvar_index == -2) {
		for (i = 0; i < data.nvars; ++i)
			if (!strcmp(data.vars[i].name, tvar_name)) break;
		if (i == data.nvars) eerror(VAR_NAME, tvar_name);
		tvar_index = i;
	}
	else
		if (tvar_index >= data.nvars) eerror(VAR_NAME, "");

	/* load and print out grammar */
	if (gramm_fname == NULL) gramm_fname = "Default";
	printf("Grammar definition file: %s\n", gramm_fname);
	load_gramm(gramm_fname);
	print_gramm();

	/* check whether number of dependent variables is correct */
	if (ndvars > data.nvars) {
		fprintf(stderr, "Wrong number of dependent variables.\n");
		exit(1);
	}
	for (i = 0; i < gramm.atoms->nprods; ++i)
		if (ndvars != gramm.atoms->prods[i]->nexpressions) {
			fprintf(stderr, "Wrong number of dependent variables.\n");
			exit(1);
		}

	/* check equation type */
	if (time_step == 0.0)
		if (ndvars == 0) {
			eqn_head = strdup("0");
			printf("Equation type: ordinary implicit is not supported in this version of Lagramge.\n");
			exit(1);
		}
		else {
			strcpy(buffer, data.vars[dvars[0]].name);
			for (i = 1; i < ndvars; ++i) {
				strcat(buffer, " # ");
				strcat(buffer, data.vars[dvars[i]].name);
			}
			eqn_head = strdup(buffer);
			printf("Equation type: ordinary explicit (%s)\n", eqn_head);

			nr_madrj = madrj_OEE;
			nr_rhols = rhols_E;
		}
	else if (time_step != -1.0) {
		if (dvars == 0)
			eerror(VAR_NAME, " - differential equation can not be implicit");
		else {
			strcpy(buffer, data.vars[dvars[0]].name);
			for (i = 1; i < ndvars; ++i) {
				strcat(buffer, "' # ");
				strcat(buffer, data.vars[dvars[i]].name);
			}
			strcat(buffer, "'");
			eqn_head = strdup(buffer);
			printf("Equation type: differential (%s, h = %g)\n", eqn_head, time_step);

			nr_madrj = madrj_DE;
			nr_rhols = rhols_E;
		}
	} else {
		strcpy(buffer, data.vars[dvars[0]].name);
		for (i = 1; i < ndvars; ++i) {
			strcat(buffer, "' # ");
			strcat(buffer, data.vars[dvars[i]].name);
		}
		strcat(buffer, "'");
		eqn_head = strdup(buffer);
		printf("Equation type: differential (%s, time_variable = %s)\n", eqn_head, data.vars[tvar_index].name);

		nr_madrj = madrj_DE_NE;
		nr_rhols = rhols_E;
	}

	/* count number of possible equations/trees and print out summary */
	count();
	printf("Maximal parse tree depth: %d\n", max_depth - 1);
	print_count(); printf("\n");
	if (t_flag) exit(0);

	if (gramm.atoms->N[max_depth - 1] <= 0) {
		fprintf(stderr, "Given grammar does not have any parse trees with depth <= %d.\n", max_depth - 1);
		exit(0);
	}

	/* initialize (mainly memory allocation) */
	printf("\n");
	init(search_strategy == 1);

	/* print out search strategy */
	printf("Search strategy: ");
	switch(search_strategy) {
		case 0: printf("exhaustive\n"); break;
		case 1: printf("beam (beam_width = %d)\n", beam_width); break;
		case 2: printf("best_first (queue_length = %d)\n", beam_width);
	}

	/* chack and print out stopping criterion */
	if (stopping_criterion == 1) {
		if (time_limit <= 0) {
			printf("Warning: illegal time limit - using default.\n");
			time_limit = 5;
		}
		signal(SIGALRM, print_results);
		alarm((unsigned int) time_limit * 60);
	} else if ((stopping_criterion == 2) && (treshold < 0.0)) {
		printf("Warning illegal treshold - using default.\n");
		treshold = 1e-5;
	}
	printf("Stopping criterion: ");
	switch(stopping_criterion) {
		case 0: printf("none\n"); break;
		case 1: printf("time_limit (%d minutes)\n", time_limit); break;
		case 2: printf("treshold (%g)\n", treshold);
	}

	/* print out search heuristic */
	printf("Search heuristic: ");
	switch(search_heuristic) {
		case 0: printf("sum of squared errors\n"); break;
		case 1: printf("mdl\n");
	}
	printf("\n");

	/* print out multistart */
	printf("Restarts of parameter estimation methods: %d\n", multistart);

	/* print out verbose status */
	printf("Verbose: %s\n", verbose_flag ? "on" : "off");

	fflush(stdout);

	/* finally, invoke search procedure and print out results */
	signal(SIGINT, print_results);
	search();
	print_results(0);
	exit(0);
}
