#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 2 "rgramm.y"
#include <stdio.h>
#include <string.h>

#include "error.h"

#ifdef _NOFLEX_
extern char yytext[];
#else
extern char *yytext;
#endif
extern FILE *yyin;

extern int ln;

extern void init_files(void);
extern void close_files(void);

extern void begin_funcdef(char *name, int ord);
extern void end_funcdef(void);

extern void atom_term(char *atom);
extern void atom_nterm(void);


int rsn, atom_index;
char *ls;

int function_flag;

int nfunctions = 0;
char *functions[1024];

int nexpressions = 0;
char expressions[1024][1024];

FILE *fc;

#line 55 "y.tab.c"
#define YYERRCODE 256
#define CONST 257
#define TERM 258
#define COMP 259
#define NTERM 260
#define YELDS 261
#define A_SPEC 262
const short yylhs[] = {                                        -1,
    2,    0,    1,    1,    4,    3,    5,    5,    8,   10,
    6,    7,   12,    7,   11,   11,   13,   13,   13,    9,
    9,   14,   14,   15,   16,   16,   17,   18,   18,
};
const short yylen[] = {                                         2,
    0,    2,    1,    2,    0,    5,    1,    3,    0,    0,
    4,    1,    0,    4,    1,    2,    1,    1,    1,    0,
    3,    1,    2,    4,    1,    2,    2,    0,    1,
};
const short yydefred[] = {                                      1,
    0,    0,    5,    0,    3,    0,    4,    9,    0,    7,
    0,    6,    9,   19,   18,   17,    0,    0,   15,    8,
   13,    0,   16,    0,    0,   11,    0,    0,    0,   22,
    0,   25,   29,   27,   21,   23,    0,   26,    0,   24,
};
const short yydgoto[] = {                                       1,
    4,    2,    5,    6,    9,   10,   17,   11,   26,   22,
   18,   24,   28,   29,   30,   31,   32,   34,
};
const short yysindex[] = {                                      0,
    0, -246,    0, -246,    0, -245,    0,    0,  -53,    0,
 -238,    0,    0,    0,    0,    0,  -17, -238,    0,    0,
    0, -100,    0, -238, -238,    0, -238, -237, -124,    0,
 -247,    0,    0,    0,    0,    0, -238,    0,  -55,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0,   27,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -56,  -33,    0,    0,
    0,  -52,    0,    0,    0,    0,  -30,  -59,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
    0,    0,   24,    0,    0,   17,    0,    0,    0,    0,
    7,    0,   -3,    0,    3,   -4,  -22,    0,
};
#define YYTABLESIZE 205
const short yytable[] = {                                      28,
   35,   12,   10,   40,   14,   12,   20,   19,   38,   14,
   15,   37,   16,    3,   23,    8,   38,   21,   14,   15,
   19,   16,   25,   23,   33,   12,    2,    7,   14,   20,
   27,   36,   39,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   10,   10,    0,    0,
   13,   20,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   12,
   12,    0,   14,   14,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,   15,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   28,   28,   28,
   28,   14,   15,    0,   16,
};
const short yycheck[] = {                                      59,
  125,   35,   59,   59,   35,   59,   59,   11,   31,  257,
  258,  259,  260,  260,   18,  261,   39,   35,  257,  258,
   24,  260,  123,   27,  262,   59,    0,    4,   59,   13,
   24,   29,   37,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  123,  124,   -1,   -1,
  124,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  123,
  124,   -1,  123,  124,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,   -1,  260,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,  259,
  260,  257,  258,   -1,  260,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 262
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,"'#'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'","'|'","'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CONST",
"TERM","COMP","NTERM","YELDS","A_SPEC",
};
const char * const yyrule[] = {
"$accept : Gramm",
"$$1 :",
"Gramm : $$1 Productions",
"Productions : Production",
"Productions : Productions Production",
"$$2 :",
"Production : NTERM $$2 YELDS RightSides ';'",
"RightSides : RightSide",
"RightSides : RightSides '|' RightSide",
"$$3 :",
"$$4 :",
"RightSide : $$3 Expressions $$4 ListOfConstraints",
"Expressions : Atoms",
"$$5 :",
"Expressions : Expressions '#' $$5 Atoms",
"Atoms : Atom",
"Atoms : Atoms Atom",
"Atom : NTERM",
"Atom : TERM",
"Atom : CONST",
"ListOfConstraints :",
"ListOfConstraints : '{' Constraints '}'",
"Constraints : Constraint",
"Constraints : Constraints Constraint",
"Constraint : AtomSpecifications COMP AtomSpecifications ';'",
"AtomSpecifications : AtomSpecification",
"AtomSpecifications : AtomSpecifications AtomSpecification",
"AtomSpecification : Atom Specification",
"Specification :",
"Specification : A_SPEC",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 171 "rgramm.y"

#define BUFF_LEN 1024


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
	fprintf(fc, "\ty = x[0];\n");
	fprintf(fc, "\treturn(0);\n");
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
#line 325 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 48 "rgramm.y"
{ init_files(); ls = strdup(""); }
break;
case 2:
#line 50 "rgramm.y"
{ close_files(); }
break;
case 5:
#line 63 "rgramm.y"
{
		if (strcmp(yytext, ls)) {
			ls = strdup(yytext);
			rsn = 0;
		}
	}
break;
case 9:
#line 84 "rgramm.y"
{
		atom_index = 0;
		strcpy(expressions[nexpressions = 0], "");
		function_flag  = 1;
	}
break;
case 10:
#line 90 "rgramm.y"
{ function_def(ls, ++rsn); }
break;
case 13:
#line 100 "rgramm.y"
{
		++atom_index;
		strcpy(expressions[++nexpressions], "");
	}
break;
case 17:
#line 117 "rgramm.y"
{ atom_nterm(); }
break;
case 18:
#line 121 "rgramm.y"
{ atom_term(yytext); }
break;
case 19:
#line 125 "rgramm.y"
{ atom_nterm(); }
break;
#line 568 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
