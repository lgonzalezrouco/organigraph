#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Metadata {
	char *tag;
	union {
		int numValue;
		char *stringValue;
	};
} Metadata;

typedef struct Employee {
	Metadata *metadata;
	size_t metadataCount;
	struct Employee **children;
	size_t childrenCount;
} Employee;

#endif