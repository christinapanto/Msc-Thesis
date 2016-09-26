%{

#include <stdio.h>
#include <string.h>
#include <malloc/malloc.h>
#include <dlfcn.h>

#include "globals.h"
#include "realloc.h"
#include "error.h"

extern char *yytext;

int derivatives_flag;


void init_gramm(void);
void done_gramm(void);

Atom *add_atom(char *name, int type);
void add_prod(void);

void add_constrained_atom(char *atom_spec);
void constraint(void);
void constraint_type(char *comp);

int rslen, nc, nexpressions;
Atom *ls, *rs[65536];

int constraint_flag = 0;
char *constrained_atom;
int nconstrained_atoms;
int constrained_atoms[65536];
%}

%start	Gramm

%token	CONST COMP TERM NTERM YELDS A_SPEC

%%

Gramm:
	{ init_gramm(); }
	Productions
	{ done_gramm(); }
;

Productions:
	Production
;
Productions:
	Productions
	Production
;

Production:
	NTERM
	{ ls = add_atom(yytext, NTERM_T); }
	YELDS
	RightSides
	';'
;

RightSides:
	RightSide
;
RightSides:
	RightSides
	'|'
	RightSide
;

RightSide:
	{
		nc = rslen = 0;
		nexpressions = 1;
	}
	Expressions
	{ add_prod(); }
	ListOfConstraints
;

Expressions:
	Atoms
;
Expressions:
	Expressions
	'#'
	{
		++nexpressions;
		if (constraint_flag == 0) rs[rslen++] = add_atom(yytext, TERM_T);
	}
	Atoms
;

Atoms:
	Atom
;
Atoms:
	Atoms
	Atom
;

Atom:
	NTERM
	{ if (constraint_flag == 0) rs[rslen++] = add_atom(yytext, NTERM_T); }
;
Atom:
	TERM
	{ if (constraint_flag == 0) rs[rslen++] = add_atom(yytext, TERM_T); }
;
Atom:
	COMP
	{ if (constraint_flag == 0) rs[rslen++] = add_atom(yytext, TERM_T); }
;
Atom:
	CONST
	{ if (constraint_flag == 0) rs[rslen++] = add_atom(yytext, CONST_T); }
;

ListOfConstraints:
;
ListOfConstraints:
	'{'
	Constraints
	'}'
;

Constraints:
;
Constraints:
	Constraints
	Constraint
;

Constraint:
	{ constraint_flag = 1; }
	AtomSpecification
	{ constraint(); }
	COMP
	{
		constraint_type(yytext);
		constraint_flag = 2;
	}
	AtomSpecification
	{
		constraint();
		constraint_flag = 0;

		++gramm.prods[gramm.nprods - 1].nconstraints;
	}
	';'
;

AtomSpecification:
	Atom
	{ constrained_atom = strdup(yytext); }
	A_SPEC
	{ add_constrained_atom(yytext); }
;

%%



void init_gramm(void) {

	gramm.atoms = (Atom *) malloc(65536 * sizeof(Atom));
}


Atom *add_atom(char *name, int type) {

	int i;
	Atom *a;
	char buffer[8192];


	if (type == CONST_T) ++nc;

	/* const[name:lower:init:upper] */
	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a)
		if (!strcmp(a->name, name)) return(a);

	if ((a->type = type) == CONST_T) sscanf(name + 6, "%[^:]:%lg:%lg:%lg", buffer, &(a->lower), &(a->init), &(a->upper));
	if (strncmp(name, "variable_", 9) == 0) a->type = VAR_T;

	a->name = strdup(name);

	++gramm.natoms;
	return(a);
}


void add_prod(void) {

	int i;


	gramm.prods = (Prod *) realloc_array(gramm.prods, gramm.nprods, 65536, sizeof(Prod));

	gramm.prods[gramm.nprods].ls = ls;
	gramm.prods[gramm.nprods].rslen = rslen;
	gramm.prods[gramm.nprods].nc = nc;

	gramm.prods[gramm.nprods].rs = (Atom **) malloc(rslen * sizeof(Atom *));
	for (i = 0; i < rslen; ++i) gramm.prods[gramm.nprods].rs[i] = rs[i];

	gramm.prods[gramm.nprods].nexpressions = nexpressions;
	gramm.prods[gramm.nprods].nconstraints = 0;

	++gramm.nprods;
}

void add_constrained_atom(char *atom_spec) {

	int i;
	int atom_specification;


	if (!strcmp(atom_spec, "")) {
		atom_specification = 1;
	} else {
		sscanf(atom_spec, ".%d", &atom_specification);
	}

	for (i = 0; i < rslen; ++i) {
		if (strcmp(constrained_atom, rs[i]->name)) continue;
		if (--atom_specification == 0) break;
	}
	if (i < rslen)
		constrained_atoms[nconstrained_atoms++] = i;
	else {
		fprintf(stderr, "Invalid atom specification (in constraint): %s%s\n", constrained_atom, atom_spec);
		exit(1);
	}

}

#define CS	gramm.prods[gramm.nprods - 1].constraints
#define	CI	gramm.prods[gramm.nprods - 1].nconstraints
#define	CONSTRAINT	CS[CI]

void constraint(void) {

	int i;


	if (constraint_flag == 1) {
		CS = (Constraint *) realloc_array(CS, CI, 8192, sizeof(Constraint));

		CONSTRAINT.nls = nconstrained_atoms;
		CONSTRAINT.ls = (int *) malloc(CONSTRAINT.nls * sizeof(int));
		for (i = 0; i < CONSTRAINT.nls; ++i)
			CONSTRAINT.ls[i] = constrained_atoms[i];
	} else {
		CONSTRAINT.nrs = nconstrained_atoms;
		CONSTRAINT.rs = (int *) malloc(CONSTRAINT.nrs * sizeof(int));
		for (i = 0; i < CONSTRAINT.nrs; ++i)
			CONSTRAINT.rs[i] = constrained_atoms[i];
	}
	nconstrained_atoms = 0;
}

void constraint_type(char *comp) {

	if (!strcmp(comp, "=="))
		CONSTRAINT.type = C_E;
	else if (!strcmp(comp, "!="))
		CONSTRAINT.type = C_NE;
	else if (!strcmp(comp, "<"))
		CONSTRAINT.type = C_L;
	else if (!strcmp(comp, "<="))
		CONSTRAINT.type = C_LE;
	else if (!strcmp(comp, ">"))
		CONSTRAINT.type = C_G;
	else if (!strcmp(comp, ">="))
		CONSTRAINT.type = C_GE;
}

void done_gramm(void) {

	int i, j, k, fn;
	Atom *a, *a_V;
	Prod *p;
	Var *v;
	char *error, buffer[1024];
	void *dlh;
	int (**f)(double *, double **, double **, int, int, int);


	sprintf(buffer, "%s.lib", gramm_fname);
	if (!(dlh = dlopen(buffer, RTLD_LAZY))) {
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	f = dlsym(dlh, "f");
	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(1);
	}

	for (j = 0, p = gramm.prods, fn = 1; j < gramm.nprods; ++j, ++p)
		p->f = f[fn++];

	/* incorporate variables into grammar */
	a_V = add_atom("V", NTERM_T);
	for (i = 0, v = data.vars; i < data.nvars; ++i, ++v) {
		rs[0] = add_atom(v->name, VAR_T);
		rs[0]->values = v->values;
		rslen = 1;

		if (time_step == 0.0) {
			for (j = 0; j < ndvars; ++j)
				if (dvars[j] == i) break;
			if (j < ndvars) continue;
		}
		if ((time_step == -1.0) && (tvar_index == i)) continue;

		ls = a_V;
		add_prod();

		p->f = f[0];
		p->nc = 0;

		p++;
	}

	/* claen and tidy */
	for (i = 0, a = gramm.atoms; i < gramm.natoms; ++i, ++a) {
		if ((a->type == TERM_T) || (a->type == CONST_T)) {
			a->nprods = 0;
			continue;
		}

		if (a->type == VAR_T) {
			if (strncmp(a->name, "variable_", 9) == 0) {
				for (j = 0, v = data.vars; j < data.nvars; ++j, ++v)
					if (strcmp(a->name + 9, v->name) == 0) break;
				if (j == data.nvars) eerror(VAR_NAME, a->name);
				a->values = v->values;
			}
			continue;
		}

		for (j = a->nprods = 0, p = gramm.prods; j < gramm.nprods; ++j, ++p)
			if (!strcmp(p->ls->name, a->name)) ++a->nprods;
		a->prods = (Prod **) malloc(a->nprods * sizeof(Prod *));

		for (j = k = 0, p = gramm.prods; j < gramm.nprods; ++j, ++p)
			if (!strcmp(p->ls->name, a->name)) a->prods[k++] = p;
	}
}
