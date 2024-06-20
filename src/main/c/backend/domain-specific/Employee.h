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
	struct Employee **bosses;
	struct Employee **children;
	size_t childrenCount;
	size_t childrenSize;
	size_t bossesCount;
	size_t bossesSize;
	char *employeeId;
} Employee;

typedef struct EmployeeList{
	Employee **employees;
	size_t employeesCount;
	size_t employeesSize;
	char *id;
} EmployeeList;


#endif