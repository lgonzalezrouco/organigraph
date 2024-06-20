#include "Generator.h"

#include <stdbool.h>


/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;

typedef struct GeneratorState {
    TEmployee *employees;
    TEmployeeList * lists;
    size_t sizeLists;
    size_t sizeEmployees;
} GeneratorState;

static GeneratorState *state;

FILE *file;

void initializeGeneratorModule() {
    _logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/** PRIVATE FUNCTIONS */

static void _generateProgram(Program *program);
static void _generateExpressions(Expressions *expressions);
static void _generateExpression(Expression *expression);
static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression);
static void _generateProperties(TEmployee employee, Properties *properties);
static void _generateAttributes(TEmployee employee, Attributes *attributes);
static void _generateAttribute(TEmployee employee, Attribute *attribute);
static void _generateEmployeeExpression(EmployeeExpression *employeeExpression);
static void _generateRemoveExpression(RemoveExpression *removeExpression);
static void _generateReplaceExpression(ReplaceExpression *replaceExpression);
static void _generateAssignExpression(AssignExpression *assignExpression);
static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression);
static void addBoss(TEmployee employee, TEmployee boss);
static void addChild(TEmployee employee, TEmployee child);
static void removeEmployee(TEmployee employee);
static void removeBosses(TEmployee employee);
static void removeChildren(TEmployee employee);
static void replaceEmployee(TEmployee old, TEmployee new);
static void replaceBosses(TEmployee old, TEmployee new);
static void replaceChildren(TEmployee old, TEmployee new);
static TEmployee getRoot(TEmployee employee);
static TEmployee *searchEmployeesInState(Properties *properties);
static TEmployee *searchEmployees(TEmployee root, Properties *properties);
static void searchEmployeesRec(TEmployee employee, Properties *properties, TEmployee *employees, int *size);
static bool hasProperties(TEmployee employee, Properties *properties);
static bool hasAttribute(TEmployee employee, Attribute *attribute);
static bool isPresent(TEmployee employee, TEmployee *employees);
static TEmployee *getChildren(TEmployee employee, size_t *size);
static TEmployee *getSelfAndChildren(TEmployee employee, size_t *size);
static TEmployee *getSiblings(TEmployee employee, size_t *size);
static size_t concatenateEmployees(TEmployee *employees1, TEmployee *employees2, size_t size1, size_t size2);
static TEmployee getEmployeeFromState(char *employeeId);

static TEmployee newEmployee(char *employeeId, Properties *properties);
static void _generatePrologue(void);
static void _generateEpilogue();
static char *_indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char *const format, ...);


static void addBoss(TEmployee employee, TEmployee boss) {
    employee->bosses = (TEmployee *) realloc(employee->bosses, employee->bossesSize + 1);
    employee->bosses[employee->bossesCount] = boss;
    employee->bossesCount++;
    employee->bossesSize++;
}

static void addChild(TEmployee employee, TEmployee child) {
    employee->children = (TEmployee *) realloc(employee->children, employee->childrenSize + 1);
    employee->children[employee->childrenCount] = child;
    employee->childrenSize++;
    employee->childrenCount++;
}

static void _generateRemoveExpression(RemoveExpression *removeExpression) {
    TEmployee employee = getEmployeeFromState(removeExpression->idToRemove);
    if (employee == NULL)
        logError(_logger, "No se encontro el empleado con id %s", removeExpression->idToRemove);
    else
        removeEmployee(employee);
}

static void removeEmployee(TEmployee employee) {
    if (employee == NULL) {
        logError(_logger, "No se puede remover un empleado que no existe");
        return;
    }
    removeBosses(employee);
    removeChildren(employee);
}

static void removeBosses(TEmployee employee) {
    for (size_t i = 0; i < employee->bossesSize; i++) {
        TEmployee boss = employee->bosses[i];
        if (boss != NULL) {
            for (size_t j = 0; j < boss->childrenSize; j++) {
                if (boss->children[j] == employee) {
                    boss->children[j] = NULL;
                    boss->childrenCount--;
                }
            }
            employee->bosses[i] = NULL;
            employee->bossesCount--;
        }
    }
}

static void removeChildren(TEmployee employee) {
    for (size_t i = 0; i < employee->childrenSize; i++) {
        TEmployee child = employee->children[i];
        if (child != NULL) {
            for (size_t j = 0; j < child->bossesSize; j++) {
                if (child->bosses[j] == employee) {
                    child->bosses[j] = NULL;
                    child->bossesCount--;
                    if (child->bossesCount == 0)
                        removeEmployee(child);
                }
            }
            employee->children[i] = NULL;
            employee->childrenCount--;
        }
    }
}

static void replaceEmployee(TEmployee old, TEmployee new) {
    if (old != NULL && new != NULL) {
        replaceBosses(old, new);
        replaceChildren(old, new);
    }
}

static void replaceBosses(TEmployee old, TEmployee new) {
    for (size_t i = 0; i < old->bossesSize; i++) {
        TEmployee boss = old->bosses[i];
        if (boss != NULL) {
            for (size_t j = 0; j < boss->childrenSize; j++) {
                if (boss->children[j] == old)
                    boss->children[j] = new;
            }
            addBoss(new, boss);
            old->bosses[i] = NULL;
            old->bossesCount--;
        }
    }
}

static void replaceChildren(TEmployee old, TEmployee new) {
    for (size_t i = 0; i < old->childrenSize; i++) {
        TEmployee child = old->children[i];
        if (child != NULL) {
            for (size_t j = 0; j < child->bossesSize; j++) {
                if (child->bosses[j] == old)
                    child->bosses[j] = new;
            }
            addChild(new, child);
            old->children[i] = NULL;
            old->childrenCount--;
        }
    }
}

static TEmployee getRoot(TEmployee employee) {
    if (employee->bossesCount == 0)
        return employee;

    for (int i = 0; i < employee->bossesCount; i++) {
        if (employee->bosses[i] != NULL)
            return getRoot(employee->bosses[i]);
    }

    return NULL;
}

// Quiero buscar empleados que cumplan con las propiedades en su metadata
static TEmployee *searchEmployees(TEmployee root, Properties *properties) {
    TEmployee *employees = (TEmployee *) malloc(sizeof(TEmployee));
    if (employees == NULL) {
        logError(_logger, "No hay mas memoria disponible");
        return NULL;
    }
    if (root == NULL) {
        logError(_logger, "No se puede buscar empleados si no hay un empleado raiz");
        return NULL;
    }
    int size = 0;
    searchEmployeesRec(root, properties, employees, &size);
    return employees;

}

static void searchEmployeesRec(TEmployee employee, Properties *properties, TEmployee *employees, int *size) {
    if (isPresent(employee, employees))
        return;

    if (hasProperties(employee, properties)) {
        employees = (TEmployee *) realloc(employees, *size + 1);
        if (employees == NULL) {
            logError(_logger, "No hay mas memoria disponible");
            return;
        }
        employees[*size] = employee;
        (*size)++;
    }

    for (size_t i = 0; i < employee->childrenCount; i++) {
        searchEmployeesRec(employee->children[i], properties, employees, size);
    }

}

static bool hasProperties(TEmployee employee, Properties *properties) {
    for (size_t i = 0; i < properties->attributes->count; i++) {
        if (!hasAttribute(employee, properties->attributes->attributes[i])) {
            return false;
        }
    }
    return true;
}

static bool hasAttribute(TEmployee employee, Attribute *attribute) {
    for (size_t i = 0; i < employee->metadataCount; i++) {
        if (strcmp(employee->metadata[i].tag, attribute->tag) == 0) {
            if (employee->metadata[i].metadataType == METADATA_INTEGER) {
                if (employee->metadata[i].numValue == attribute->numValue) {
                    return true;
                }
            } else if (employee->metadata[i].metadataType == METADATA_STRING) {
                if (strcmp(employee->metadata[i].stringValue, attribute->stringValue) == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

static bool isPresent(TEmployee employee, TEmployee *employees) {
    if (employees == NULL || employee == NULL)
        return false;

    for (int i = 0; employees[i] != NULL; i++) {
        if (employees[i] == employee)
            return true;
    }
    return false;
}

static TEmployee *getChildren(TEmployee employee, size_t *size) {
    TEmployee *children = (TEmployee *) malloc(sizeof(TEmployee));
    if (children == NULL) {
        logError(_logger, "No hay mas memoria disponible");
        return NULL;
    }
    int count = 0;
    for (size_t i = 0; i < employee->childrenSize; i++) {
        if (employee->children[i] != NULL) {
            children = (TEmployee *) realloc(children, count + 1);
            if (children == NULL) {
                logError(_logger, "No hay mas memoria disponible");
                return NULL;
            }
            children[count] = employee->children[i];
            count++;
        }
    }

    *size = count;
    return children;
}

static TEmployee *getSelfAndChildren(TEmployee employee, size_t *size) {
    TEmployee *children = (TEmployee *) malloc(sizeof(employee->children) + sizeof(employee));
    *size = 0;
    for (size_t i = 0; i < employee->childrenSize; i++) {
        if (employee->children[i] != NULL && !isPresent(employee->children[i], children)) {
            children[*size] = employee->children[i];
            *size++;
        }

    }
    children[*size] = employee;
    *size++;
    return children;
}

static TEmployee *getSiblings(TEmployee employee, size_t *size) {
    TEmployee *siblings;
    *size = 0;
    for (int i = 0; i < employee->bossesSize; i++) {
        TEmployee boss = employee->bosses[i];
        *size = concatenateEmployees(siblings, getChildren(boss, size), *size, boss->childrenSize);
    }
    return siblings;
}

static size_t concatenateEmployees(TEmployee *employees1, TEmployee *employees2, size_t size1, size_t size2) {
    int j = 0;
    for (size_t i = 0; i < size2; i++) {
        if (employees2[i] != NULL && !isPresent(employees2[i], employees1)) {
            employees1 = (TEmployee *) realloc(employees1, size1 + j + 1);
            if (employees1 == NULL) {
                logError(_logger, "No hay mas memoria disponible");
                return 0;
            }
            employees1[size1 + j] = employees2[i];
            j++;
        }
    }
    return size1 + j;
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program *program) {
    state = (GeneratorState *) calloc(1, sizeof(GeneratorState));

    if (state == NULL) {
        logError(_logger, "No hay mas memoria disponible");
        return;
    }

    _generateExpressions(program->expressions);
}

static void _generateExpressions(Expressions *expressions) {
    for (int i = 0; i < expressions->count; i++) {
        _generateExpression(expressions->expressions[i]);
    }
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(Expression *expression) {
    switch (expression->type) {
        case VARIABLE_EMPLOYEE_EXPRESSION:
            _generateVariableEmployeeExpression(expression->variableEmployeeExpression);
            break;
        case EMPLOYEE_EXPRESSION:
            _generateEmployeeExpression(expression->employeeExpression);
            break;
        case REMOVE_EXPRESSION:
            _generateRemoveExpression(expression->removeExpression);
            break;
        case REPLACE_EXPRESSION:
            _generateReplaceExpression(expression->replaceExpression);
            break;
        case ASSIGN_EXPRESSION:
            _generateAssignExpression(expression->assignExpression);
            break;
        case RELATIONSHIP_EXPRESSION:
            _generateRelationshipExpression(expression->relationshipExpression);
            break;
        default:
            logError(_logger, "The specified expression type is unknown: %d", expression->type);
            break;
    }
}

static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression) {
    newEmployee(variableEmployeeExpression->employeeId, variableEmployeeExpression->properties);
}

static void _generateProperties(TEmployee employee, Properties *properties) {
    _generateAttributes(employee, properties->attributes);
}

static void _generateAttributes(TEmployee employee, Attributes *attributes) {
    for (int i = 0; i < attributes->count; i++) {
        _generateAttribute(employee, attributes->attributes[i]);
    }
}

static void _generateAttribute(TEmployee employee, Attribute *attribute) {
    employee->metadata = (Metadata *) realloc(employee->metadata, employee->metadataCount + 1);
    if (employee->metadata == NULL) {
        logError(_logger, "No hay mas memoria disponible");
        return;
    }

    employee->metadata[employee->metadataCount].tag = attribute->tag;
    switch (attribute->attributeType) {
        case ATTRIBUTE_INTEGER:
            employee->metadata[employee->metadataCount].metadataType = METADATA_INTEGER;
            employee->metadata[employee->metadataCount].numValue = attribute->numValue;
            break;
        case ATTRIBUTE_STRING:
            employee->metadata[employee->metadataCount].metadataType = METADATA_STRING;
            employee->metadata[employee->metadataCount].stringValue = attribute->stringValue;
            break;
    }
    employee->metadataCount++;
}

static void _generateEmployeeExpression(EmployeeExpression *employeeExpression) {
    TEmployee employee = getEmployeeFromState(employeeExpression->employeeId);

    if (employee == NULL)
        employee = newEmployee(employeeExpression->employeeId, employeeExpression->properties);
    else
        _generateProperties(employee, employeeExpression->properties);

    if (employeeExpression->hierarchy != NULL) {
        switch (employeeExpression->hierarchy->list->listType) {
            case LIST_PROPERTIES: //search que trae los jefes
                TEmployee root = getRoot(employee);
                if (root == NULL) {
                    logError(_logger, "No se puede buscar empleados si no hay un empleado raiz");
                    return;
                }
                TEmployee *employees = searchEmployees(root, employeeExpression->hierarchy->list->properties);
                if (employees == NULL) {
                    logError(_logger, "No se encontraron empleados con las propiedades especificadas");
                    return;
                }
                for (int i = 0; employees[i] != NULL; i++) {
                    addBoss(employee, employees[i]);
                    addChild(employees[i], employee);
                }
                break;
            case LIST_ELEMENTS:
                for (int i = 0; i < employeeExpression->hierarchy->list->elements->count; i++) {
                    TEmployee employeeAux = getEmployeeFromState(employeeExpression->hierarchy->list->elements->ids[i]);
                    if (employeeAux == NULL) {
                        logError(_logger, "No se encontro el empleado con id %s", employeeExpression->hierarchy->list->elements->ids[i]);
                    } else {
                        addBoss(employee, employeeAux);
                        addChild(employeeAux, employee);
                    }
                }
                break;
                /*
                case LIST_EMPLOYEE: //Revisar si vamos a sacar el assign
                    TEmployee boss = getEmployee(project, employeeExpression->hierarchy->list->employeeId);
                    addChild(project, boss, employee);
                    break;
                */
        }
    }
}

static void _generateReplaceExpression(ReplaceExpression *replaceExpression) {
    TEmployee old = getEmployeeFromState(replaceExpression->idToReplace);
    if (old == NULL) {
        logError(_logger, "No se encontro el empleado con id %s", replaceExpression->idToReplace);
        return;
    }

    TEmployee new;
    if (replaceExpression->properties != NULL) {
        new = newEmployee(replaceExpression->idToReplaceWith, replaceExpression->properties);
        if (new == NULL) {
            logError(_logger, "No se pudo crear el empleado con id %s", replaceExpression->idToReplaceWith);
            return;
        }
    } else {
        new = getEmployeeFromState(replaceExpression->idToReplaceWith);
        if (new == NULL) {
            logError(_logger, "No se encontro el empleado con id %s", replaceExpression->idToReplaceWith);
            return;
        }
    }

    replaceEmployee(old, new);
}

//SEGURO LO SACAMOS
static void _generateAssignExpression(AssignExpression *assignExpression) {
    // TODO: check if will be used
}

static TEmployee *searchEmployeesInState(Properties *properties) {
    TEmployee *employees = NULL;

    for (int size = 0; size < state->sizeEmployees; size++) {
        if (hasProperties(state->employees[size], properties)) {
            employees = (TEmployee *) realloc(employees, size + 1);
            if (employees == NULL) {
                logError(_logger, "No hay mas memoria disponible");
                return NULL;
            }
            employees[size] = state->employees[size];
            size++;
        }
    }
    return employees;
}

static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression) {
    TEmployee *employees = NULL;
    size_t size = 0;

    switch (relationshipExpression->list->listType) {
        case LIST_PROPERTIES:
            employees = searchEmployeesInState(relationshipExpression->list->properties);
            if (employees == NULL) {
                logError(_logger, "No se encontraron empleados con las propiedades especificadas");
                return;
            }
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < relationshipExpression->list->elements->count; i++) {
                TEmployee employeeAux = getEmployeeFromState(relationshipExpression->list->elements->ids[i]);
                size = concatenateEmployees(employees, &employeeAux, size, 1);
            }
            break;
        case LIST_EMPLOYEE:

            break;
    }

    TEmployee * employeesInRelationship = NULL;
    size = 0;

    switch (relationshipExpression->relationship->relationshipType) {
        case RELATIONSHIP_CHILD_AND_SELF:
            for(int i = 0; employees[i] != NULL; i++) {
                size = concatenateEmployees(employeesInRelationship, &employees[i], size, 1);
                size = concatenateEmployees(employeesInRelationship, employees[i]->children, size,
                                            employees[i]->childrenSize);
            }
            break;
        case RELATIONSHIP_CHILD:
            for(int i = 0; employees[i] != NULL; i++)
                size = concatenateEmployees(employeesInRelationship, employees[i]->children, size, employees[i]->childrenSize);
            break;

        case RELATIONSHIP_SIBLING:
            for(int i = 0; employees[i] != NULL; i++) {
                for (int j = 0; employees[i]->bossesSize; j++)
                    size = concatenateEmployees(employeesInRelationship, employees[i]->bosses[j]->children, size,
                                                employees[i]->bosses[j]->childrenSize);
            }
            break;
    }

    TEmployee * bosses = NULL;
    size_t bossesSize = 0;

    switch (relationshipExpression->hierarchy->list->listType) {
        case LIST_PROPERTIES:
            bosses = searchEmployeesInState(relationshipExpression->hierarchy->list->properties);
            if (bosses == NULL) {
                logError(_logger, "No se encontraron empleados con las propiedades especificadas");
                return;
            }
            break;
        case LIST_EMPLOYEE:
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < relationshipExpression->hierarchy->list->elements->count; i++) {
                TEmployee employeeAux = getEmployeeFromState(relationshipExpression->hierarchy->list->elements->ids[i]);
                bossesSize = concatenateEmployees(bosses, &employeeAux, bossesSize, 1);
            }
            break;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; bosses[j] != NULL; j++) {
            addBoss(employeesInRelationship[i], bosses[j]);
            addChild(bosses[j], employeesInRelationship[i]);
        }
    }
}

//Destruye al nodo empleado
static void fireEmployee(TEmployee employee){
	if(employee!=NULL){
	removeEmployee(employee);
		for(int i=0;i<state->sizeEmployees;i++){
			if(state->employees[i] == employee){
				state->employees[i] = NULL;
			}
		}
		free(employee);
	}
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
    fprintf(file, "%s", "{ \"nodes\": [");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue() {
    fprintf(file, "%s", "] }\n");
}

/**
 * Generates an indentation string for the specified level.
 */
static char *_indentation(const unsigned int level) {
    return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char *const format, ...) {
    va_list arguments;
    va_start(arguments, format);
    char *indentation = _indentation(indentationLevel);
    char *effectiveFormat = concatenate(2, indentation, format);
    vfprintf(stdout, effectiveFormat, arguments);
    free(effectiveFormat);
    free(indentation);
    va_end(arguments);
}

static TEmployee newEmployee(char *employeeId, Properties *properties) {
    if (employeeId == NULL) {
        logError(_logger, "No se puede crear un empleado sin id");
        return NULL;
    } else if (getEmployeeFromState(employeeId) != NULL) {
        logError(_logger, "El empleado ya existe");
        return NULL;
    }
    state->employees = (TEmployee *) realloc(state->employees, state->sizeEmployees + 1);

    if (state->employees == NULL) {
        logError(_logger, "No hay mas memoria disponible");
        return NULL;
    }

    state->sizeEmployees++;

    TEmployee employee = (TEmployee) malloc(sizeof(struct Employee));
    employee->employeeId = employeeId;
    employee->metadata = NULL;
    employee->metadataCount = 0;
    employee->children = NULL;
    employee->childrenCount = 0;
    employee->bosses = NULL;
    employee->bossesCount = 0;
    _generateProperties(employee, properties);

    state->employees[state->sizeEmployees - 1] = employee;

    return employee;
}

static TEmployee getEmployeeFromState(char *employeeId) {
    for (int i = 0; i < state->sizeEmployees; i++) {
        if (strcmp(state->employees[i]->employeeId, employeeId) == 0) {
            return state->employees[i];
        }
    }

    return NULL;
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState *compilerState) {
    logDebugging(_logger, "Generating final output...");
    file = fopen("project.json", "w");
    _generatePrologue();
    _generateProgram(compilerState->abstractSyntaxtTree);
    _generateEpilogue();
    logDebugging(_logger, "Generation is done.");
}
