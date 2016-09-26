#include <malloc/malloc.h>


void *realloc_array(void *array, int n, int alloc_por, int size) {

        if (n == 0) return(malloc(size * alloc_por));
        if (n % alloc_por == 0) return(realloc(array, size * (n + alloc_por)));
        return(array);
}
