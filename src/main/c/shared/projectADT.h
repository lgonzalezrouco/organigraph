#ifndef PROJECT_ADT_H
#define PROJECT_ADT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct projectCDT *projectADT;

typedef struct Employee {
    void ** metadata;
    struct Employee ** children;
    size_t childrenCount;
} Employee;

typedef Employee * TEmployee;

projectADT newProject();

int addEmployee(projectADT project, char ** metadata, TEmployee * childrens);

int removeEmployee(projectADT project, TEmployee employee);

int addChild(projectADT project, TEmployee * employees, char ** metadata, TEmployee * children);

TEmployee * getChildren(projectADT project, TEmployee employee);

TEmployee * getChildrenAndSelf(projectADT project, TEmployee employee);

// TODO: define if we do getSiblings

char * generateJsonFromRoot(projectADT project);

char * generateJsonFromEmployee(projectADT project, TEmployee employee);

void freeEmployee(TEmployee employee);

void freeProject(projectADT project);

#endif
