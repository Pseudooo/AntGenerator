#include <stdio.h>
#include "util/vector.h"

// Setup 64^2 binary grid
unsigned long grid[64];

// Ant structure
typedef struct {
    Vector pos;
    Vector vel;
} Ant;

int main() {    

    
    
}

int get_bit(Vector* v) {
    return grid[v->j] && 1 << v->i != 0;
}

void check_bit(Vector* v) {
    grid[v->j] ^= 1 << v->i
}