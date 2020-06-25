#include <stdio.h>
#include <stdlib.h>

#include "util/Vector.h"
#include "util/Ant.h"

int main()
{

    // Currently just  prints 256 16digit hex values
    // that are generated

    init_grid();

    const int n = 256;
    unsigned long* arr = malloc(n * sizeof(unsigned long));
    generate(arr, n * sizeof(unsigned long));

    for(int i = 0; i < n; i++)
        printf("Index %03d: %016lx\n", i, arr[i]);

}