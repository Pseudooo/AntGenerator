
#include "Ant.h"
#include "Vector.h"

#include <stdio.h>

typedef struct {
    Vector pos;
    Vector vel;
} Ant;

unsigned long grid[64];
Ant ants[16];

void tick();
int get_bit(Vector v);
void tog_bit(Vector v);

/*
    Clears the grid to be all zeros
*/
void init_grid()
{
    for(int i = 0; i < 64; i++)
        grid[i] = 0ul;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            Ant a = {{j, i}, {1, 0}};
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