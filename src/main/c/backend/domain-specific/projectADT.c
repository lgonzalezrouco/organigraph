#include "projectADT.h"

struct projectCDT {
    size_t size;
    TEmployee root;
    char name[256];
};

static TEmployee getEmployeeRec(TEmployee employee, char *employeeId);

projectADT newProject() {
    projectADT project = malloc(sizeof(struct projectCDT));
    project->root = NULL;
    project->size = 0;
    return project;
}

void setName(projectADT project, char *name) {
    strcpy(project->name, name);
}

static TEmployee getEmployeeRec(TEmployee employee, char *employeeId) {
    if (employee == NULL)
        return NULL;

    if (strcmp(employee->employeeId, employeeId) == 0)
    
    for (size_t i = 0; i < employee->childrenCount; i++) {
        if (strcmp(employee->children[i]->employeeId, employeeId) == 0)
            return employee->children[i];
        
        TEmployee result = getEmployeeRec(employee->children[i], employeeId);
        if (result != NULL)
            return result;
    }

    return NULL;
}

TEmployee getEmployee(projectADT project, char *employeeId) {
    return getEmployeeRec(project->root, employeeId);
}

int addChild(projectADT project, TEmployee boss, TEmployee employee){
    if (boss == NULL)
        return 0;

    boss->children = realloc(boss->children, (boss->childrenSize + 1) * sizeof(TEmployee));
    boss->children[boss->childrenSize] = employee;
    boss->childrenCount++;
    boss->childrenSize++;
    project->size++;
    return 1;
}

int addEmployee(projectADT project, TEmployee employee) {
    if (project->root == NULL) {
        project->root = employee;
        project->size++;
        return 1;
    }

    return addChild(project, project->root, employee);
}

static int removeEmployeeRec(TEmployee employee, TEmployee employeeToRemove) {
    if (employee == NULL)
        return 0;

    for (size_t i = 0; i < employee->childrenSize; i++) {
        if (employee->children[i] != NULL && employee->children[i] == employeeToRemove) {
            employee->children[i] = NULL;
            employee->childrenCount--;
            return 1;
        }

        if (removeEmployeeRec(employee->children[i], employeeToRemove)) {
            return 1;
        }
    }

    return 0;
}

int removeEmployee(projectADT project, TEmployee employee) {
    if (project->root == NULL)
        return 0;

    if (project->root == employee) {
        // freeEmployee(project->root); // TODO: employee should remains in the generator state  
        project->root = NULL;
        project->size--;
        return 1;
    }

    int result = removeEmployeeRec(project->root, employee);

    if (result)
        project->size--;

    return result;
}

