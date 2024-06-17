#ifndef PROJECT_ADT_H
#define PROJECT_ADT_H

#include <stdio.h>
#include <stdlib.h>

#include "Employee.h"

typedef struct projectCDT *projectADT;

typedef Employee *TEmployee;

projectADT newProject();

void setName(projectADT project, char *name);

TEmployee getEmployee(projectADT project, char *employeeId);

int addChild(projectADT project, TEmployee boss, TEmployee employee);

int addEmployee(projectADT project, TEmployee employee);

int removeEmployee(projectADT project, TEmployee employee);

int replaceEmployee(projectADT project, TEmployee old, TEmployee new);

TEmployee *getChildren(projectADT project, TEmployee employee);

TEmployee *getChildrenAndSelf(projectADT project, TEmployee employee);

char *generateJsonFromRoot(projectADT project);

char *generateJsonFromEmployee(projectADT project, TEmployee employee);

void freeEmployee(TEmployee employee);

void freeProject(projectADT project);

#endif
