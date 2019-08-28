#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    // Start by experimenting with params to the
    // script. We know many things. . . 
    fprintf(stdout, "Arguments to Script: %d\n", argc);
    // See if we can iterate over the arguments
    int i = 0;
    for (i = 0; i < argc; ++i) {
        fprintf(stdout, "argv[%d]=%s\n", i, argv[i]);
    }
}
