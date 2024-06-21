#ifndef _STACKADT_H
#define _STACKADT_H

#include <strings.h>

typedef struct stackCDT * stackADT;

typedef int elemType;

stackADT newStack(void);
int isEmpty(stackADT stack);

void push(stackADT stack, elemType elem);
elemType pop(stackADT stack);
void freeStack(stackADT stack);

#endif