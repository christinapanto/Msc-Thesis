#ifndef _GRAMM_TYPE_
#define _GRAMM_TYPE_



typedef enum {TERM_T, NTERM_T, CONST_T, VAR_T} Atom_t;

typedef struct str_atom {
	/*
	 * atom can be terminal, nonterminal, constant or domain variable
	 */
	Atom_t type;
	
	/*
	 * atom name
	 */
	char *name;

	/*
	 * productions with atom on the left side
	 */
	int nprods;
	struct str_prod **prods;

	/*
	 * the height of the atom:
	 * minimal height of a parse tree with atom in the root and
         * terminals in leaves
	 */
	int h;

	/*
	 * number of parse trees with atom in root and height =/<= h
	 * size: max_dpeth
	 */
	double *n, *N;
	
	/*
	 * max and min length of derivated expression,
         * max derivation length and max number of "const" atoms
	 * in the parse trees with atom in root and height h
	 * size: max_dpeth
	 */
	int *max_len, *min_len, *mdlen, *mnc;

	/*
	 * values of the atom (used for domain variables only)
	 * size: data.len
	 */
	double *values;

	/*
	 * bounds (used for constants only)
	 */
	double lower, init, upper;
} Atom;



typedef enum {C_E, C_NE, C_L, C_LE, C_G, C_GE} Constraint_t;

typedef struct str_constaint {
	/*
	 * type of the constraint
	 */
	Constraint_t type;

	/*
	 * pair of arrays of constrained indexes
	 */
	int nls, *ls, nrs, *rs;
} Constraint;



typedef struct str_prod {
	/*
	 * atom on the left side of the production
	 */
	Atom *ls;

	/*
	 * atoms on the right side of the production
	 */
	int rslen;
	Atom **rs;
	
	/*
	 * number of atoms "const" on the right side of the production
	 */
	int nc;

	/*
	 * pointer to function explaining the semantics of the production
	 */
	int (*f)(double *, double **, double **, int, int, int);

	/*
	 * a height of the production:
	 * minimal height of parse tree with atom in root, terminals in leaves
	 * and production used in root
	 */
	int h;

	/*
	 * a length of the production:
	 * minimal hength of the derived expression using parse tree
	 * with atom in root, terminals in leaves
	 * and production used in root
	 */
	int l;

	/*
	 * production can generate more than one expression
	 * they are separated using '#'
	 * for now, this is limited to the productions of the start symbol
	 */
	int nexpressions;

	/*
	 * array of constraints
	 */
	int nconstraints;
	Constraint *constraints;
} Prod;



typedef struct str_gramm {
	int natoms;
	Atom *atoms;
	
	int nprods;
	Prod *prods;
	
	int max_plen;
} Gramm;
#endif
