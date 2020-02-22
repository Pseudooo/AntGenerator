#include <stdio.h>

#include "util/vector.h"

// Ant structure
typedef struct {
    Vector pos;
    Vector vel;
} Ant;

int get_bit(Vector* v);
void tog_bit(Vector* v);

void tick();

// Setup 64^2 binary grid
unsigned long grid[64];

Ant ants[64];

int main(int argc, char* argv[]) {  

    printf("Initializing...\n");
    
    // init grid
    for(int i = 0; i < 64; i++)
        grid[i] = 0L;

    // init ants
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int idx = i + j*8;

            /*
                All ants are initialized to the 0,0 bit
                of their respective 8x8 block and also
                looking "up" (1,0)
            */

            Vector v;
            v.i = 1;
            v.j = 0;
            ants[idx].vel = v;

            v.i = i*8;
            v.j = j*8;
            ants[idx].pos = v;

        }
    }

    // TODO Allow for digest data

}

void tick() {

    // Perform a tick for each ant
    for(int i = 0; i < 64; i++) {
        // pos & vel respectively
        Vector* r = &ants[i].pos;
        Vector* v = &ants[i].vel;

        if(get_bit(r)) { // cellular rule
            rotate_cw(v);
        }else{
            rotate_acw(v);
        }

        // Toggle cur bit and move
        tog_bit(r);
        vec_add(r, v);

        // Handle wrapping if ant leaves grid bounds
        if(r->i >= 64) {        
            r->i = 0;
        }else if(r->i < 0) {
            r->i = 63;
        }

        if(r->j >= 64) {
            r->j = 0;
        }else if(r->j < 0) {
            r->j = 63;
        }

    }

}

// Get the bit located at position v
int get_bit(Vector* v) {
    return (grid[v->j] & (1L << v->i)) != 0;
}

// Toggle bit located at position v
void tog_bit(Vector* v) {
    grid[v->j] ^= 1L << v->i;
}