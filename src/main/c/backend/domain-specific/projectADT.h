#ifndef PROJECT_ADT_H
#define PROJECT_ADT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct projectCDT *projectADT;

typedef struct Employee {
	void **metadata;
	struct Employee **children;
	size_t childrenCount;
} Employee;

typedef Employee *TEmployee;

projectADT newProject();

int addEmployee(projectADT project, char **metadata, TEmployee *children);

int removeEmployee(projectADT project, TEmployee employee);

int replaceEmployee(projectADT project, TEmployee old, TEmployee new);

int addChild(projectADT project, TEmployee *employees, char **metadata, TEmployee *children);

TEmployee *getChildren(projectADT project, TEmployee employee);

TEmployee *getChildrenAndSelf(projectADT project, TEmployee employee);

void setName(projectADT project, char *name);

char *generateJsonFromRoot(projectADT project);

char *generateJsonFromEmployee(projectADT project, TEmployee employee);

void freeEmployee(TEmployee employee);

void freeProject(projectADT project);

#endif
