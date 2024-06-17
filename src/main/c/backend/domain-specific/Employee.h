#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum MetadataType { METADATA_STRING, METADATA_INTEGER } MetadataType;

typedef struct Metadata {
	char *tag;
	union {
		int numValue;
		char *stringValue;
	};
	MetadataType metadataType;
} Metadata;

typedef struct Employee {
	Metadata *metadata;
	size_t metadataCount;
	struct Employee **children;
	size_t childrenCount;
	size_t childrenSize;
	char *employeeId;
} Employee;

#endif