#include <stdio.h>
#include <stdlib.h>

#include "util/vector.h"

// Ant structure
typedef struct {
    Vector pos;
    Vector vel;
} Ant;

int get_bit(Vector* v);
void tog_bit(Vector* v);

void digest(FILE* f);
void tick();

void hash_file(char* f_name);

unsigned char* extract();

// Setup 64^2 binary grid
unsigned long grid[64];

Ant ants[64];

int main(int argc, char* argv[]) {  

    // init grid
    for(int i = 0; i < 64; i++)
        grid[i] = 0L;

    // init ants
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int idx = i + j*8; // R² => R

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

    // Check correct arguments have been provided
    if(argc != 2) {
        printf("Invalid Arguments!\n");
        printf("Please provide a file!\n");
        return 0;
    }

    // Perform 1024 ticks
    for(int i = 0; i < 8192; i++)
        tick();

    unsigned char* hash = extract();

    hash_file(argv[1]);

    printf("Key: ");
    for(int i = 0; i < 15; i++) {
        printf("%02X:", hash[i]);
    }
    printf("%02X\n", hash[15]);

    free(hash);

    return 0;

}

void hash_file(char* f_name) {

     // Check the file is ok for reading
    FILE* f;
    if((f = fopen(f_name, "rb")) == NULL) {
        fclose(f);
        printf("File Error!\n");
        return;
    }

    printf("Digesting Data...\n");
    digest(f); fclose(f);

}

void digest(FILE* f) {

    /*
        One "block" of data is the amount of data (in bytes) needed
        to XOR into the grid once. Several blocks will be read into
        the buffer at once to reduce IO calls.
    */  
    const unsigned int BLOCK_SIZE = 64; // 8x8byte grid => 64 bytes needed
    const unsigned int BLOCK_QTY = 10;
    const unsigned int BUFFER_SIZE = BLOCK_SIZE * BLOCK_QTY;

    char* buffer[BUFFER_SIZE]; // init buffer
    for(int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 0;

    // Read into the buffer
    while(fread(buffer, 1, BUFFER_SIZE, f)) {

        // Process buffer into blocks for ease
        for(int offset = 0; offset < BUFFER_SIZE; offset += BLOCK_SIZE) {
            
            

        }

    }

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
 // 0A:64:61:6E:20:69:73:60:66:61:67:67:6F:74:0A:2A
unsigned char* extract() {

    // Initialize and extract grid into segs
    unsigned long segs[2];
    for(int i = 0; i < 2; i++)
        segs[i] = 0L;

    for(int i = 0; i < 64; i += 2)
        for(int j = 0; j < 2; j++)
            segs[j] ^= (i+j) % 3 == 0 ? grid[i+j] : ~grid[i+j];

    // initialize byte array
    unsigned char* hash = malloc(sizeof(char) * 16);
    if(hash == NULL) return NULL;

    // Convert segs into byte array
    for(int i = 7; i < 16; i += 8)
        hash[i] = segs[((i + 1) / 8) - 1];

    return hash;

}

// Get the bit located at position v
int get_bit(Vector* v) {
    return (grid[v->j] & (1L << v->i)) != 0;
}

// Toggle bit located at position v
void tog_bit(Vector* v) {
    grid[v->j] ^= 1L << v->i;
}