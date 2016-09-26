#include <stdio.h>
#include <stdlib.h>

#include "error.h"


extern void load_gramm(char *fname);



int main(int argc, char **argv) {


	if (argc != 2) eerror(USAGE_RG, argv[0]);
	load_gramm(argv[1]);

	exit(0);
}
