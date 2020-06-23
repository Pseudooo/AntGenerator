#include "Vector.h"

void rotate_cw(Vector* v) {

    // Rotate the vector clockwise
    int tmp = -1 * v -> i;
    v -> i = v -> j;
    v -> j = tmp;

}

void rotate_acw(Vector* v) {

    // Rotate the vector anti-clockwise
    int tmp = v -> i;
    v -> i = -1 * v -> j;
    v -> j = tmp;

}

void vec_add(Vector* v1, Vector* v2) {

    // Set v1 = v1 + v2
    v1->i += v2->i;
    v1->j += v2->j;

}
