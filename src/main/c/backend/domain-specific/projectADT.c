#include "projectADT.h"

struct projectCDT {
    size_t size;
    TEmployee root;
	char name[256];
};

projectADT newProject() {
    projectADT project = malloc(sizeof(struct projectCDT));
    project->root = NULL;
    project->size = 0;
    return project;
}

