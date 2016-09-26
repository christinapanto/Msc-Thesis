#include "ptree_t.h"
#include "gramm_t.h"



extern void print_ptree(PTree *t, int cf);
extern void draw_ptree(PTree *t, int cf);

extern void print_ptree_model(FILE *f, PTree *t);

extern int length(PTree *t);

extern void init_ptree(PTree *t, Atom *a);

extern void preorder_lr(PTree *t);
extern void postorder_lr(PTree *t);

extern void improve_ptnode(PTree *t, PTNode *n);
extern void improve_ptnode_b(PTree *t, PTNode *n);
extern void terminate_ptnode(PTree *t, PTNode *n);

extern void remove_ptnode(PTree *t, PTNode *n);

extern void cp_ptree(PTree *dt, PTree *st);

extern int eq_ptrees(PTree *t1, PTree *t2);

extern int distance(PTree *t1, PTree *t2);
