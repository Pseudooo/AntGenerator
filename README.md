
# AntHasher

AntHasher is a project I've made where I use the Cellular Automata, Langton's Ant, to generate a pseudo-random number for some given data.

 - [Langton's Ant](#Langtons-Ant)
 - [Using Langton's Ant](#Using-Langtons-Ant)

## Langton's Ant

[The wiki](https://en.wikipedia.org/wiki/Langton%27s_ant)

If we have a grid of black and white squares, the ant(s) can be on any of the squares and facing towards an adjacent square.

1. If an Ant is on a black Square it will turn clockwise, otherwise it will turn Clockwise
2. The Ant will then invert the colour of the Square it is stood on
3. The Ant will then step forward on space in the direction it is facing

## Using Langton's Ant

By nature the grid is a binary grid, as each space within the grid can have one of two states (1 or 0).

And due to the nature of the cellular automata we can see that if we were to start all the Ants in the same initial positions and run the automata for a constant amount of ticks we'll always get the same result. So, we can make a sort of hashing function from this if were to base the starting positions and/or amount of ticks on some data.

## Digesting Data

In my design I have a 64x64 binary grid, on which the _Ants_ roam around. Now to digest some data we need to somehow include it into the starting positions of the ants.

So I broke the 64x64 grid into 8x8 bit squares and each one had an Ant that started at the top left corner so in total 64 Ants are on the grid.

The data to be digested is then broken into _BLOCKS_, a single _BLOCK_ is 128 bytes. Each Ant on the grid digests 2 bytes of that data. The first byte is digested into its position within its own 8x8 square, the second byte determines how many clock-wise rotations it will undergo.

This process is repeated for all of the data in the file, if the buffer needs to be padded it will retain 0s for the first read, but retain the previous read's data otherwise.

## The Implementation

The binary grid is implemented as an array of longs.
```c
unsigned long grid[64];
```

The grid is initialized into all 0s
```c
for(int i = 0; i < 64; i++) 
    grid[i] = 0L;
```

The Rules for the ants are shown in the `tick` function. Both the position vector, `r`, and the velocity vector, `v`, have been defined like so:
```c
Vector* r = &ants[i].pos;
Vector* v = &ants[i].vel;
```

The Ant's rotation is determined:
```c
if(get_bit(r)) { // cellular rule
    rotate_cw(v);
}else{
    rotate_acw(v);
}
```

The ant then inverts the space its stood on and steps forward
```c
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
```