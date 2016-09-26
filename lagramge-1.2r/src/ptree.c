#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc/malloc.h>

#include "globals.h"
#include "ptree.h"



/*
 * print parse tree in human readable form
 * call with pointer to the root node of the parse tree
 */
void R_mathml_ptree(PTree *t, PTNode *n, int cf) {

	int i;


	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			R_mathml_ptree(t, n->children[i], cf);
		return;
	}
	if (n->atom->type == CONST_T) {
		if (cf == 1)
			printf(" %g", n->values[0]);
		else
			printf(" const[_:%g:%g:%g]", n->atom->lower, n->atom->init, n->atom->upper);

		return;
	}
	printf(" %s", n->atom->name);
}

void mathml_ptree(PTree *t, int cf) {

	R_mathml_ptree(t, t->nodes, cf);
}


/*
 * print parse tree in MathML
 * call with pointer to the root node of the parse tree
 */
void R_print_ptree(PTree *t, PTNode *n, int cf) {

	int i;


	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			R_print_ptree(t, n->children[i], cf);
		return;
	}
	if (n->atom->type == CONST_T) {
		if (cf == 1)
			printf(" %g", n->values[0]);
		else
			printf(" const[_:%g:%g:%g]", n->atom->lower, n->atom->init, n->atom->upper);

		return;
	}
	printf(" %s", n->atom->name);
}

void print_ptree(PTree *t, int cf) {

	R_print_ptree(t, t->nodes, cf);
}


/*
 * generate input file for graphviz (dot): draw a parse tree
 * call with pointer to the root node of the parse tree
 */
void R_nodes_ptree(PTree *t, PTNode *n, int cf) {

	int i;

	if (n == (PTNode *) NULL) return;

	printf("\tnode_%p [label = \"", (void *) n);
	if ((cf == 0) || (n->atom->type != CONST_T))
		printf("%s", (n->atom->type == VAR_T) ? n->atom->name + 9 : n->atom->name);
	else {
		printf("%g", n->values[0]);
	}
	printf("\"];\n");

	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i) R_nodes_ptree(t, n->children[i], cf);
	}
}

void R_edges_ptree(PTree *t, PTNode *n) {

	int i;


	if (n == (PTNode *) NULL) return;

	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i) {
			printf("\tnode_%p -- node_%p%s;\n", (void *) n, (void *) n->children[i], ((i == 0) || (i == n->prod->rslen - 1)) ? "" : " [color = white]");
			R_edges_ptree(t, n->children[i]);
		}
	}
}

void draw_ptree(PTree *t, int cf) {

	printf("graph parse_tree {\n\n");
	printf("\tnodesep=0.01;\n\n");
	printf("\tnode [shape=\"plaintext\", font=\"Helvetica\", height=0.0, width= 0.0];\n\n");

	R_nodes_ptree(t, t->nodes, cf);
	printf("\n");

	R_edges_ptree(t, t->nodes);

	printf("}\n");
}


/*
 * print parse tree in machine readable form:
 * all we need to reconstruct the model later
 */
void R_print_ptree_model(FILE *f, PTree *t, PTNode *n) {

	int i;


	fprintf(f, " %d", n->prod_index);
	if (n->prod_index != -1) {
		for (i = 0; i <= n->prod->rslen; ++i)
			R_print_ptree_model(f, t, n->children[i]);
		return;
	}
	if (n->atom->type == CONST_T) {
		printf(" %g", n->values[0]);
	}
}

void print_ptree_model(FILE *f, PTree *t) {

	R_print_ptree_model(f, t, t->nodes);
}



/*
 * checks whether two parse trees are equal
 */
int R_eq_ptrees(PTNode *n1, PTNode *n2) {

	int i;


	if (n1->atom != n2->atom) return(0);
	if (n1->atom->type != NTERM_T) return(1);
	if (n1->prod != n2->prod) return(0);
	for (i = 0; i < n1->prod->rslen; ++i)
		if (R_eq_ptrees(n1->children[i], n2->children[i]) == 0) return(0);
	return(1);
}

int eq_ptrees(PTree *t1, PTree *t2) {

	return(R_eq_ptrees(t1->nodes, t2->nodes));
}



/*
 * length of the derived expression
 */
int R_length(PTree *t, PTNode *n) {

	int i, len = 0;


	if (n->atom->type != NTERM_T) return(1);
	for (i = 0; i < n->prod->rslen; ++i)
		len += R_length(t, n->children[i]);
	return(len);
}

int length(PTree *t) {

	return(R_length(t, t->nodes));
}



/*
 * allocates new parse tree node
 */
PTNode *new_node(PTree *t, PTNode *parent, Atom *a, int ci) {

	int i;
	PTNode *n;


	/* find first free node */
	for (i = 0, n = t->nodes; n->free == 0; ++i, ++n);

	n->free = 0;
	n->atom = a;
	n->h = parent->h + 1;
	n->parent = parent;
	n->child_index = ci;
	n->prod_index = 0;
	n->children[0] = (PTNode *) NULL;

	n->refinement_index = 0;

	return(n);
}


/*
 * initialize the root node of a parse tree with given nonterminal
 */
void init_ptree(PTree *t, Atom *a) {

	int i;
	PTNode *n;

	/* free all nodes */
	for (i = 0, n = t->nodes; i < t->max_nnodes; ++i, ++n) n->free = 1;

	t->nodes->free = 0;
	t->nodes->atom = a;
	t->nodes->h = 0;
	t->nodes->parent = (PTNode *) NULL;
	t->nodes->child_index = -1;
	t->nodes->prod_index = 0;

	t->nodes->refinement_index = 0;
}

/*
 * remove a node and the subtree
 */
void remove_ptnode(PTree *t, PTNode *n) {

	int i;


	if (n == (PTNode *) NULL) return;

	n->free = 1;

	if (n->parent != (PTNode*) NULL)
		if (n->parent->cl_flags[n->child_index] == 0)
			n->parent->children[n->child_index] = (PTNode *) NULL;

	if (n->atom->type != NTERM_T) return;

	for (i = 0; i < n->prod->rslen; ++i) remove_ptnode(t, n->children[i]);
}



/*
 * check whether the i-th symbol on the right side of
 * the production *p is equally cosntrained with some
 * symbol on the left. if it is return the index of
 * the symbol constraining it, otherwise return -1.
 */
#define MAX(a,b)		(((a) > (b)) ? (a) : (b))
#define MIN(a,b)		(((a) < (b)) ? (a) : (b))

int eq_constrained(Prod *p, int i) {

	int j;
	Constraint *c;

	for (j = 0, c = p->constraints; j < p->nconstraints; ++j, ++c) {
		if (c->type != C_E) continue;
		if (MAX(c->ls[0], c->rs[0]) == i) return(MIN(c->ls[0], c->rs[0]));
	}
	return(-1);
}



/*
 * terminate all nonterminal nodes in a parse tree
 */
void terminate_ptnode(PTree *t, PTNode *n) {

	int i, j;


	if (n->atom->type != NTERM_T) return;

	n->prod = n->atom->prods[n->prod_index];

	for (i = 0; i < n->prod->rslen; ++i) {
		if ((j = eq_constrained(n->prod, i)) != -1) {
			n->cl_flags[i] = 1;
			n->children[i] = n->children[j];
		} else {
			n->cl_flags[i] = 0;
			if (n->children[i] == (PTNode *) NULL)
				n->children[i] = new_node(t, n, n->prod->rs[i], i);
			terminate_ptnode(t, n->children[i]);
		}
	}
}


void terminate_ptnode_b(PTree *t, PTNode *n) {

	int i, j;


	if (n->atom->type != NTERM_T) return;

	n->prod = n->atom->prods[n->prod_index];

	if (n->children[0] == (PTNode *) NULL)
		for (i = 0; i < n->prod->rslen; ++i) {
			if ((j = eq_constrained(n->prod, i)) != -1) {
				n->cl_flags[i] = 1;
				n->children[i] = n->children[j];
			} else {
				n->cl_flags[i] = 0;
				n->children[i] = new_node(t, n, n->prod->rs[i], i);
				terminate_ptnode_b(t, n->children[i]);
			}
		}
	else
		for (i = 0; i < n->prod->rslen; ++i) {
			if ((j = eq_constrained(n->prod, i)) != -1) {
				n->cl_flags[i] = 1;
				n->children[i] = n->children[j];
			} else {
				n->cl_flags[i] = 0;
				terminate_ptnode_b(t, n->children[i]);
			}
		}
}



/*
 * build a list of all internal nodes in a parse tree
 */
void R_preorder_lr(PTree *t, PTNode *n) {

	int i;


	t->preorder_lr[t->nnodes++] = n;
	if (n->atom->type == CONST_T) n->values[0] = t->c[t->c_index++];

	if (n->atom->type == NTERM_T) {
		for (i = 0; i < n->prod->rslen; ++i)
			if (n->cl_flags[i] == 0) R_preorder_lr(t, n->children[i]);
	}
		
}

void preorder_lr(PTree *t) {

	t->nnodes = 0;
	t->c_index = 0;
	R_preorder_lr(t, t->nodes);
}


/*
 * improve the production at the node of the parse tree with
 * more complicated one and terminate all nontermial nodes
 */

void improve_ptnode(PTree *t, PTNode *n) {

	PTNode *p;
	int i;


	for (i = 0; i < n->prod->rslen; ++i) remove_ptnode(t, n->children[i]);
	n->free = 0;
	++n->prod_index;

	++n->refinement_index;
	for (p = n->parent; p != (PTNode *) NULL; p = p->parent) ++p->refinement_index;

	terminate_ptnode(t, t->nodes);
}

void improve_ptnode_b(PTree *t, PTNode *n) {

	PTNode *p;
	int i;


	for (i = 0; i < n->prod->rslen; ++i) remove_ptnode(t, n->children[i]);
	n->free = 0;
	++n->prod_index;

	++n->refinement_index;
	for (p = n->parent; p != (PTNode *) NULL; p = p->parent) ++p->refinement_index;

	terminate_ptnode_b(t, t->nodes);
}



/*
 * make a memory copy (dt) of the parse tree (st)
 */
void R_cp_ptree(PTree *dt, PTNode *dn, PTree *st, PTNode *sn) {

	int i, j;


	if (sn->atom->type == CONST_T) {
		dt->c[dt->c_index++] = st->c[st->c_index++];
		return;
	}
	if (sn->atom->type != NTERM_T) return;

	dn->prod_index = sn->prod_index;
	dn->prod = sn->prod;
	for (i = 0; i < dn->prod->rslen; ++i) {
		if ((j = eq_constrained(dn->prod, i)) != -1) {
			dn->children[i] = dn->children[j];
			dn->cl_flags[i] = 1;
		} else {
			dn->children[i] = new_node(dt, dn, dn->prod->rs[i], i);
			R_cp_ptree(dt, dn->children[i], st, sn->children[i]);
			dn->cl_flags[i] = 0;
		}
	}
}

void cp_ptree(PTree *dt, PTree *st) {

	init_ptree(dt, st->nodes->atom);

	dt->c_index = st->c_index = 0;
	R_cp_ptree(dt, dt->nodes, st, st->nodes);

	dt->nnodes = 0;
	preorder_lr(dt);

	dt->mse = st->mse;
	dt->mdl = st->mdl;
}
