#include <stdio.h>

#include "util/vector.h"

// Setup 64^2 binary grid
unsigned long grid[64];

// Ant structure
typedef struct {
    Vector pos;
    Vector vel;
} Ant;

int get_bit(Vector* v);
void tog_bit(Vector* v);

int main() {  

    printf("Executing...");
    
    // init grid
    for(int i = 0; i < 64; i++)
        grid[i] = 0L;

    // ...

}

int get_bit(Vector* v) {
    return (grid[v->j] & (1L << v->i)) != 0;
}

void tog_bit(Vector* v) {
    grid[v->j] ^= 1L << v->i;
}