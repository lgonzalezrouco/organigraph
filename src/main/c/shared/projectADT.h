#ifndef TREE_ADT_H
#define TREE_ADT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct projectCDT *projectADT;

typedef struct employee {
    void ** metadata;
    struct node ** children;
    size_t childrenCount;
} Employee;

typedef Employee * TEmployee;

projectADT newProject();

int addEmployee(projectADT project, char ** metadata, TEmployee * childrens);

int removeEmployee(projectADT project, TEmployee employee);

int addChild(projectADT project, TEmployee employee, char ** metadata, TEmployee * children);

TEmployee * getChildren(projectADT project, TEmployee employee);

TEmployee * getChildrenAndSelf(projectADT project, TEmployee employee);

// TODO: define if we do getSiblings

char * generateJsonFromRoot(projectADT project);

char * generateJsonFromEmployee(projectADT project, TEmployee employee);

void freeEmployee(TEmployee employee);

void freeProject(projectADT project);

#endif
