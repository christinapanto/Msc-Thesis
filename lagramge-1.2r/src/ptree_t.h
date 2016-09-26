#ifndef _PARSE_TYPE_
#define _PARSE_TYPE_

#include "gramm_t.h"



typedef struct str_pnode {

	int free;

	/*
	 * Syntax of the parse tree (data and topology):
	 *  - pointer to the atom description
	 *  - distance from the root node
	 *  - pointer to the parent node
	 *  - production used to expand current node
	 *  - array of pointers to the children nodes
	 */
	Atom *atom;
	int h;

	struct str_pnode *parent;
	int child_index;

	int prod_index;
	Prod *prod;

	struct str_pnode **children;

	int refinement_index;

	int index_lr;

	/*
	 * Semantics of the parse tree:
	 *  - pointer to the array of the node evaluations
	 */
	double *values;

	/*
	 * Equality constraints:
	 *  - list of cross-links flags: 1 would mean that
	 *    the corresponding child is only link to another
	 *    parse-tree
	 */
	int *cl_flags;

} PTNode;


typedef struct str_ptree {

	/*
	 * maximal number of nodes in the tree
	 */
	int max_nnodes;

	/*
	 * array of nodes in the parse tree
	 */
	PTNode *nodes;

	/*
	 * array of pointers to internal nodes in the parse tree
	 * in order from left to right
	 */
	int nnodes;
	PTNode **preorder_lr;
	PTNode **postorder_lr;

	/*
	 * constant parameter values (after the tree has been evaluated)
	 * and c_index is a variable needed during the evaluation
	 */
	double *c;
	int c_index;
	
	/*
	 * result of the evaluation and errors
	 */
	double *yhat, mse, mdl;

} PTree;

#endif
