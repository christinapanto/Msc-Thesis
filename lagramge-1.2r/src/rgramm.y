%{
#include <stdio.h>
#include <string.h>

#include "error.h"

extern char *yytext;

extern FILE *yyin;

extern int ln;

extern void init_files(void);
extern void close_files(void);

extern void begin_funcdef(char *name, int ord);
extern void end_funcdef(void);

extern void atom_term(char *atom);
extern void atom_nterm(void);

void function_def(char *ls, int ord);
int rsn, atom_index;
char *ls;

int function_flag;

int nfunctions = 0;
char *functions[65536];

int nexpressions = 0;
char expressions[1024][262144];

FILE *fc;

%}

%start	Gramm

%token	CONST TERM COMP NTERM YELDS A_SPEC

%%

Gramm:
	{ init_files(); ls = strdup(""); }
	Productions
	{ close_files(); }
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
	{
		if (strcmp(yytext, ls)) {
			ls = strdup(yytext);
			rsn = 0;
		}
	}
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
		atom_index = 0;
		strcpy(expressions[nexpressions = 0], "");
		function_flag  = 1;
	}
    Expressions
    { function_def(ls, ++rsn); }
	ListOfConstraints
;

Expressions:
	Atoms
;
Expressions:
	Expressions
	'#'
	{
		++atom_index;
		strcpy(expressions[++nexpressions], "");
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
	{ atom_nterm(); }
;
Atom:
	TERM
	{ atom_term(yytext); }
;
Atom:
	CONST
	{ atom_nterm(); }
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
	AtomSpecifications
	COMP
	AtomSpecifications
	';'
;

AtomSpecifications:
	AtomSpecification
;
AtomSpecifications:
	AtomSpecifications
	AtomSpecification
;

AtomSpecification:
	Atom
	Specification
;

Specification:
;
Specification:
	A_SPEC
;

%%

#define BUFF_LEN 65536


void init_files(void) {

	char line[BUFF_LEN];


	fc = fopen("functions.c", "w");

	if ((fgets(line, BUFF_LEN - 1, yyin) != NULL) && !strncmp(line, "%{", 2)) {
		while (fgets(line, BUFF_LEN - 1, yyin) != NULL) {
			if (!strncmp(line, "%}", 2)) break;
			fputs(line, fc);
		}
		fprintf(fc, "\n\n");
	}
	else rewind(yyin);

	fprintf(fc, "int _EQ(double *y, double **x, double **dx, int offset, int len, int n) {\n\n");
	fprintf(fc, "\tint i;\n\n");
	fprintf(fc, "\tfor (i = offset; i < offset + len; ++i)\n");
	fprintf(fc, "\t\ty[i] = x[0][i];\n");
	fprintf(fc, "\treturn(1);\n");
	fprintf(fc, "}\n\n\n");
}


void close_files(void) {

	int i;


	fprintf(fc, "int (*f[])(double *, double **, double **, int, int, int) = {_EQ");
	for (i = 0; i < nfunctions; ++i) fprintf(fc, ", %s", functions[i]);
	fprintf(fc, "};\n");

	fclose(fc);
}


void function_def(char *ls, int ord) {

	char *name;
	int n;


	name = (char *) malloc((strlen(ls) + 10) * sizeof(char));
	if (ord == 0)
		sprintf(name, "_%s", ls);
	else
		sprintf(name, "_%s_%d", ls, ord);
	functions[nfunctions++] = strdup(name);

	fprintf(fc, "int %s(double *y, double **x, double **dx, int offset, int len, int n) {\n\n", name);
	fprintf(fc, "\tint i;\n\n");

	if (nexpressions == 0) {
		fprintf(fc, "\tfor (i = offset; i < offset + len; ++i)\n");
		fprintf(fc, "\t\ty[i] = %s;\n", expressions[0]);
	} else {
		fprintf(fc, "\tswitch(n) {\n");
		for (n = 0; n <= nexpressions; ++n) {
			fprintf(fc, "\t\tcase %d:\n", n);
			fprintf(fc, "\t\tfor (i = offset; i < offset + len; ++i)\n");
			fprintf(fc, "\t\t\ty[i] = %s;\n", expressions[n]);
			fprintf(fc, "\t\tbreak;\n\n");
		}
		fprintf(fc, "\t}\n");
	}

	fprintf(fc, "\treturn(1);\n");
	fprintf(fc, "}\n\n\n");
	function_flag = 0;
}


void atom_term(char *atom) {

	if (function_flag == 0) return;

	if (strncmp(atom, "variable_", 9) != 0) {
		strcat(expressions[nexpressions], " ");
		strcat(expressions[nexpressions], atom);
		++atom_index;
	} else
		atom_nterm();
}


void atom_nterm(void) {

	char buffer[1024];


	if (function_flag == 0) return;

	sprintf(buffer, " x[%d][i]", atom_index++);
	strcat(expressions[nexpressions], buffer);
}
