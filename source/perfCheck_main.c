#include "perfCheck.h"
//#include " your header "
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int i, n;
    time_t result;
    
    if(argc > 1)
        n = atoi(argv[1]);
    else
        n = 100;

    printf("%s: %ld\n", perfCheck( /* your function */, n));

    return 0;
}
