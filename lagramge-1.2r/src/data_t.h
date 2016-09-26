#ifndef _DATA_TYPE_
#define _DATA_TYPE_



typedef struct {
	char *name;
	double *values;
} Var;

typedef struct {
	int nvars, len;
	Var *vars;

	int nsegments;
	int *segments;
} Data;
#endif
