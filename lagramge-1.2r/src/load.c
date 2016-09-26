#include <stdio.h>
#include <string.h>
#include <malloc/malloc.h>

#include "globals.h"
#include "error.h"


void init_data(int nvars, int len) {

	int i;
	Var *v;


	data.nvars = nvars;
	data.len = 0;

	data.vars = (Var *) malloc(nvars * sizeof(Var));
	for (i = 0, v = data.vars; i < nvars; ++i, ++v)
		v->values = (double *) malloc(len * sizeof(double));
}


void load_error(char *err, int ln, char *fname) {

	fprintf(stderr, "%s in data line %d.\n", err, ln);
	eerror(DATA_FILE, fname);
}

void load_data(char *fname) {

	FILE *f;
	char name[256];
	int i, j, n, nvars = 0, len = 0;
	double x;
	Var *v;


	if ((f = fopen(fname, "r")) == NULL) eerror(OPEN_FILE, fname);

	while (fscanf(f, " %[^ ;\t\n]", name) == 1) ++nvars;
	fscanf(f, " ;");
	while ((n = fscanf(f, " %lg", &x)) == 1) {
		if (n != 1) load_error("Wrong number of values", len + 1, fname);
		for (i = 1; i < nvars; ++i)
			if (fscanf(f, " %lg", &x) != 1)
				load_error("Wrong number of values", len + 1, fname);
		if (fscanf(f, " ;") != 0) load_error("Missing ;", len + 1, fname);
		++len;
	}
	if (data.nsegments == -1)
		init_data(nvars, len);
	else {
		if (data.nvars != nvars)
			load_error("Incompatible file", 1, fname);
		for (i = 0, v = data.vars; i < nvars; ++i, ++v)
			v->values = (double *) realloc(v->values, (data.len + len) * sizeof(double));

		if (data.nsegments == 0)
			data.segments = (int *) malloc(100 * sizeof(int));
		else if (data.nsegments % 100 == 0)
			data.segments = (int *) realloc(data.segments, data.nsegments * sizeof(int));

		data.segments[data.nsegments] = data.len;
	}

	f = freopen(fname, "r", f);
	for (i = 0, v = data.vars; i < nvars; ++i, ++v) {
		fscanf(f, " %[^ ;\t\n]", name);
		if (data.nsegments == -1)
			v->name = strdup(name);
		else
			if (strcmp(v->name, name))
				load_error("Incompatible file", 1, fname);
	}
	fscanf(f, " ;");
	for (j = 0; j < len; ++j) {
		for (i = 0, v = data.vars; i < nvars; ++i, ++v)
			fscanf(f, " %lg", v->values + data.len + j);
		fscanf(f, " ;");
	}

	data.len += len;
	++data.nsegments;
}
