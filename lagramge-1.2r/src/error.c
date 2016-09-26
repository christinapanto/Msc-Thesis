#include <stdio.h>
#include <stdlib.h>

#include "error.h"



char *errors[] = {
	"Usage: %s\n [-T]: report search space complexity only\n [-V]: verbose on\n [-g grammar]: CFG specification\n [-d max_depth]: maximal parse tree depth\n [-v var]: dependent variable\n [-s search_strategy]: exhaustive (default), beam, bestfirst\n [-b beam_width]: beam width (default: 25)\n [-h search_heuristic]: mse (default), mdl\n [-c stopping_criterion]: none (default), timelimit, treshold\n [-e error_treshold]: used by treshold stopping criterion\n [-l time_limit]: (in minutes) used by timelimit stopping criterion\n [-i time_step] time step for discovery of ODEs\n [-t time_var]: time variable for discovery of ODEs\n [-o file_name]: write models for simulation\n [-m number_of_restarts (default 0)]: multiple invocations of the fit procedure\n [-r seed]: set seed for random number generator \ninput_file(s): one or more measuremnt tables\n",
	"Usage: %s gramm_file \n",
	"Can not open file %s\n",
	"Error in data input file %s\n",
	"Error in grammar input file %s\n",
	"Out of memory in allocating %s\n",
	"Illegal variable name %s\n",
	"Illegal search strategy %s\n",
	"Illegal beam width %s\n",
	"Illegal parse tree depth %s\n",
	"Illegal search heuristic %s\n",
	"Illegal stopping criterion %s\n",
	"Usage: %s [-e equation_number] model_file input_file(s)\n",
	"Illegal multistart parameter %s\n",
	"Illegal maximal distance parameter %s\n"
};


void eerror(Error e, char *s) {

	fprintf(stderr, errors[e], s);
	exit(e + 1);
}
