%{
#include <stdio.h>

#include "gramm_y.h"
#include "error.h"



void yyerror(char *str);

int ln = 1;
char msg[100];
%}

delim		[ \t]
ws			{delim}+
digit		[0-9]
upper		[A-Z]
lower		[a-z]
uline		"_"
comp		"=="|"!="|"<"|"<="|">"|">="
symbol		[\(\)\[\]\+\-\*\/\,]
num			[+\-]?{digit}+(\.{digit}+)?([eE][+\-]?{digit}+)?
a_spec		"."{digit}+
term		({lower}|{uline})({upper}|{lower}|{digit}|{uline})*|{symbol}
nterm		{upper}({upper}|{lower}|{digit}|{uline})*
const		"const"\[{term}:{num}:{num}:{num}\]

%%

{ws}		{}

{comp}		{ return(COMP); }

{const}		{ return(CONST); }

{a_spec}	{ return(A_SPEC); }

{term}		{ return(TERM); }
{nterm}		{ return(NTERM); }

"\n"		{ ++ln; }
"#"			{ return('#'); }
"|"			{ return('|'); }
"->"		{ return(YELDS); }
";"			{ return(';'); }
"{"			{ return('{'); }
"}"			{ return('}'); }

. {
	sprintf(msg, "unrecognized character: %s\0", yytext);
	yyerror(msg);
}

%%

int err;



void yyerror(char *str) {

        fprintf(stderr, "%s in line %d.\n", str, ln);
        err = 1;
}


void load_gramm(char *fname) {

	if ((yyin = fopen(fname, "r")) == NULL) eerror(OPEN_FILE, fname);
	yyparse();
	if (err == 1) eerror(GRAMM_FILE, fname);
}
