#include <stdio.h>
#include <stdlib.h>

#include "util/vector.h"

/*
    Long consists of 64 bits so having 64 of them
    means we can use this as a bit-wise grid on
    which we can run our ant automaton
*/
unsigned long grid[64];

void print_grid();

int get_bit(Vector v);
void tog_bit(Vector v);

int main()
{

    printf("Initializing...\n");

    // Initializing the grid to be all 0s
    for(int i = 0; i < 64; i++)
        grid[i] = 0L;

    printf("Done!\n");

    print_grid();

}

void print_grid()
{
    Vector v;
    for(v.j = 0; v.j < 64; v.j++)
    {
        for(v.i = 0; v.i < 64; v.i++)
            printf("%c", get_bit(v) ? '#' : ' ');
        printf("\n");
    }
}

int get_bit(Vector v)
{
    return (grid[v.j] & (1UL << v.i)) != 0;
}

void tog_bit(Vector v)
{
    grid[v.j] ^= (1UL << v.i);
}

// // KEEPING FOR FUTURE REFERENCE
// // WILL BE REMOVED
// void digest(FILE* f) {

//     /*
//         One block is enough data to digest into the grid
//         once. Block contains 128 bytes, 2 bytes per Ant.
//     */
//     const unsigned int BLOCK_SIZE = 128;
//     const unsigned int BLOCK_QTY = 2048;

//     /*
//         Buffer will contain an integer multiple of blocks
//     */
//     const unsigned int BUFFER_SIZE = BLOCK_SIZE * BLOCK_QTY; // 256KB

//     //     GET LENGTH OF FILE
//     fseek(f, 0L, SEEK_END);
//     // Look for an alternative
//     const unsigned int len = ftell(f) / BUFFER_SIZE + 1;
//     unsigned int cur = 0;
//     fseek(f, 0L, SEEK_SET);

//     // Create and init buffer
//     unsigned char buffer[BUFFER_SIZE];
//     for(int i = 0; i < BUFFER_SIZE; i++)
//         buffer[i] = 0;

//     while(fread(buffer, 1, BUFFER_SIZE, f)) {
//         // Read into buffer

//         // Divide buffer into blocks for ease
//         for(int offset = 0; offset < BUFFER_SIZE; offset += BLOCK_SIZE) {
//             unsigned char* block = &buffer[offset];

//             // Process individual block
//             for(int i = 0; i < 64; i++) {
//                 Ant* ant = &ants[i];

//                 // Digest left-byte into init pos
//                 int shift = block[i*2] % 64;
//                 Vector v;
//                 v.i = shift % 8;
//                 v.j = shift / 8;

//                 // Pad ant's square
//                 vec_add(&ant->pos, &v);
//                 if(ant->pos.i >= (i % 8) * 8 + 8)
//                     ant->pos.i -= 8;

//                 if(ant->pos.j >= (i / 8) * 8 + 8)
//                     ant->pos.j -= 8;

//                 // Digest right-byte into init vel
//                 int rot = block[i*2 + 1] % 4;
//                 for(int j = 0; j < rot; j++)
//                     rotate_cw(&ant->vel);

//             }

//         }

//         // Have more verbose output for larger files
//         cur++;
//         printf("Digested Block [%d/%d]\n", cur, len);
//     }

// }