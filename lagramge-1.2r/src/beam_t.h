#ifndef _BEAM_TYPE_
#define _BEAM_TYPE_

#include "ptree_t.h"



typedef struct str_belement {
	/*
	 * Parse tree of the expression
	 * on the right hand of the equation
	 */
	PTree ptree;

	/*
	 * Are successors of this element already generated?
	 */
	int successors_flag;

} BElement;



typedef struct str_beam {
	/*
	 * array of the elements in the beam
	 */
	int nelements;
	BElement *elements;

} Beam;

#endif
