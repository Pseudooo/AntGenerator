
#include "Ant.h"
#include "Vector.h"

#include <stdio.h>
#include <string.h>

typedef unsigned long u64;
typedef struct {
    Vector pos;
    Vector vel;
} Ant;

u64 grid[64];
Ant ants[16];

void tick();
int get_bit(Vector v);
void tog_bit(Vector v);

/*
    Resets the grid to be all 0s and resets all ants
    back to their original positions.
*/
void init_grid()
{
    for(int i = 0; i < 64; i++)
        grid[i] = 0ul;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            const Ant a = {{16*j, 16*i}, {1, 0}};
            ants[i*4+j] = a;
        }
}

/*
    Will perform a given number of ticks on the grid
*/
void run_ticks(const int n)
{
    for(int i = 0; i < n; i++)
        tick();
}

/*
    Will run a singular tick for all ants on the grid
*/
void tick()
{

    for(int i = 0; i < 16; i++)
    {
        Ant* ant = &ants[i];

        // 1. Ant inverts current bit and moves forward
        tog_bit(ant->pos);
        vec_add(&ant->pos, &ant->vel);

        // 2. Make sure ant hasn't left the bounds of grid
        if(ant->pos.i < 0)
            ant->pos.i = 63;
        else if(ant->pos.i >= 64)
            ant->pos.i = 0;

        if(ant->pos.j < 0)
            ant->pos.j = 63;
        else if(ant->pos.j >= 64)
            ant->pos.j = 0;

        // 3. Check the state of the current tile and rotate
        if(get_bit(ant->pos))
            rotate_cw(&ant->vel);
        else
            rotate_acw(&ant->vel);
   }

}

/*
    Will use the Ant Automata to generate x "random"
    bytes
    Note: Will not reset the grid before/after execution
    so runs can be called sequentially to avoid large buffers
*/
void generate(void* dest, const u64 bytes)
{

    // NPE
    if(dest == NULL)
        return;

    // Make local pointer that we can dereference
    char* _dest = (char*) dest;

    // Zero grid and reset ants
    run_ticks(8192);

    const int blocks = bytes / 512;
    const int remainder = bytes % 512;

    // Head to keep track of how much has been written
    u64 head = 0ul;

    if(blocks > 0)
    {
        for(int i = 0; i < blocks; i++)
        {
            // Copy grid, update head, run automata 
            memcpy(dest+head, grid, 512);
            head += 512;
            run_ticks(8192);
        }
    }

    // Pad out remaining
    memcpy(dest+head, grid, remainder);

}

/*
    Will digest a given data source
*/
void digest(void* src, const u64 bytes)
{

    // Establish char type to allow deref
    unsigned char* _src = (unsigned char*) src;

    // Initialize bank with ant's current positions on grid
    // modulo 256
    unsigned char bank[16];
    for(int i = 0; i < 16; i++)
    {
        Vector v = ants[i].pos;
        bank[i] = v.i + 64*v.j;
    }

    // Digest the actual data stream into the ants positions
    for(u64 digested = 0ul; digested < bytes; digested += 16)
        for(int i = 0; i < 16; i++)
            // If we've reached the end pad with 0s
            bank[i] += digested + i < bytes ? _src[digested+i] : 0;

    // Convert the bank into positions for each individual ant
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            const int idx = i + 4*j;
            ants[idx].pos.i = 16*i + (bank[idx] % 8);
            ants[idx].pos.j = 16*j + (bank[idx] / 8);
        }

}

/*
    Helper functions that allow to manipulation
    of individual bits within the grid
*/
int get_bit(Vector v)
{
    return (grid[v.j] & (1UL << v.i)) != 0;
}

void tog_bit(Vector v)
{
    grid[v.j] ^= (1UL << v.i);
}
