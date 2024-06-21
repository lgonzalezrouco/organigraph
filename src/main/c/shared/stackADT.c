#include "stackADT.h"
#include <stdlib.h>
#include <assert.h>
#define BLOCK 20

struct stackCDT {
    elemType * v;    // los elementos
    size_t dim; // cantidad de elementos en el stack
    size_t size; // cantidad de espacios reservados
};

stackADT newStack(void) {
    return calloc(1, sizeof(struct stackCDT));
}

int isEmpty(stackADT stack) {
    return stack->dim == 0;
}

void push(stackADT stack, elemType elem) {
    if ( stack->size == stack->dim) {
        stack->size += BLOCK;
        stack->v = realloc(stack->v, stack->size * sizeof(stack->v[0]));
    }
    stack->v[stack->dim++] = elem;
}

elemType pop(stackADT stack){
    assert (!isEmpty(stack));
    return stack->v[--stack->dim];
}

void freeStack(stackADT stack) {
    free(stack->v);
    free(stack);
}
