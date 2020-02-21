#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int i;
    int j;
} Vector;

void rotate_cw(Vector* vector);
void rotate_acw(Vector* vector);

#endif 