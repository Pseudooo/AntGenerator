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

unsigned char* extract();

// Setup 64^2 binary grid
unsigned long grid[64];

Ant ants[64];

int main(int argc, char* argv[]) {  

    // Check correct arguments have been provided
    if(argc != 2) {
        printf("Invalid Arguments!\n");
        printf("Please provide a file!\n");
        return 0;
    }

    // Check the file is ok for reading
    FILE* f;
    if((f = fopen(argv[1], "rb")) == NULL) {
        fclose(f);
        printf("File Error!\n");
        return 0;
    }

    printf("Initializing State...\n");

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

    printf("Digesting Data...\n");
    digest(f); fclose(f);

    // Perform 1024 ticks
    for(int i = 0; i < 8192; i++)
        tick();

    unsigned char* hash = extract();

    printf("Key: ");
    for(int i = 0; i < 15; i++) {
        printf("%02X:", hash[i]);
    }
    printf("%02X\n", hash[15]);

    free(hash);

    return 0;

}

void digest(FILE* f) {

    /*
        One block is enough data to digest into the grid
        once. Block contains 128 bytes, 2 bytes per Ant.
    */
    const unsigned int BLOCK_SIZE = 128;
    const unsigned int BLOCK_QTY = 2048;

    /*
        Buffer will contain an integer multiple of blocks
    */
    const unsigned int BUFFER_SIZE = BLOCK_SIZE * BLOCK_QTY; // 256KB

    //     GET LENGTH OF FILE
    fseek(f, 0L, SEEK_END);
    // Look for an alternative
    const unsigned int len = ftell(f) / BUFFER_SIZE + 1;
    unsigned int cur = 0;
    fseek(f, 0L, SEEK_SET);

    // Create and init buffer
    unsigned char buffer[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 0;

    while(fread(buffer, 1, BUFFER_SIZE, f)) {
        // Read into buffer

        // Divide buffer into blocks for ease
        for(int offset = 0; offset < BUFFER_SIZE; offset += BLOCK_SIZE) {
            unsigned char* block = &buffer[offset];

            // Process individual block
            for(int i = 0; i < 64; i++) {
                Ant* ant = &ants[i];

                // Digest left-byte into init pos
                int shift = block[i*2] % 64;
                Vector v;
                v.i = shift % 8;
                v.j = shift / 8;

                // Pad ant's square
                vec_add(&ant->pos, &v);
                if(ant->pos.i >= (i % 8) * 8 + 8)
                    ant->pos.i -= 8;

                if(ant->pos.j >= (i / 8) * 8 + 8)
                    ant->pos.j -= 8;

                // Digest right-byte into init vel
                int rot = block[i*2 + 1] % 4;
                for(int j = 0; j < rot; j++)
                    rotate_cw(&ant->vel);

            }

        }

        // Have more verbose output for larger files
        cur++;
        printf("Digested Block [%d/%d]\n", cur, len);
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