#include "projectADT.h"

struct projectCDT {
    size_t size;
    TEmployee root;
};

projectADT newProject() {
    projectADT project = malloc(sizeof(struct projectCDT));
    project->root = NULL;
    project->size = 0;
    return project;
}

