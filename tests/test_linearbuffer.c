/* thirdparty */
#include <cutest.h>

/* local private */
#include "linearbuffer.h"


#define BUFFSIZE  512


void
test_linearbuffer() {
    char backend[BUFFSIZE];
    struct linearbuffer b;

    linearbuffer_init(&b, backend, BUFFSIZE);
    eqint(BUFFSIZE, b.size);
}


int
main() {
    test_linearbuffer();
    return EXIT_SUCCESS;
}
