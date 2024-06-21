#include "Generator.h"

#include <stdbool.h>


/* MODULE INTERNAL STATE */

const char indentationCharacter = ' ';
const char indentationSize = 4;
static Logger *logger = NULL;

typedef struct GeneratorState {
    TEmployee *employees;
    TEmployeeList *lists;
    size_t sizeLists;
    size_t sizeEmployees;
} GeneratorState;

static GeneratorState *state;

FILE *file;

void initializeGeneratorModule() {
    logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
    if (logger != NULL) {
        destroyLogger(logger);
    }
}

/** PRIVATE FUNCTIONS */

static void generateProgram(Program *program);
static void generateExpressions(Expressions *expressions);
static void generateExpression(Expression *expression);
static void generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression);
static void generateProperties(TEmployee employee, Properties *properties);
static void generateAttributes(TEmployee employee, Attributes *attributes);
static void generateAttribute(TEmployee employee, Attribute *attribute);
static void generateEmployeeExpression(EmployeeExpression *employeeExpression);
static void generateRemoveExpression(RemoveExpression *removeExpression);
static void generateReplaceExpression(ReplaceExpression *replaceExpression);
static void generateAssignExpression(AssignExpression *assignExpression);
static void generateRelationshipExpression(RelationshipExpression *relationshipExpression);

static void generatePrologue(void);
static void generateEpilogue();
static char *myIndentation(unsigned int indentationLevel);
static void output(unsigned int indentationLevel, const char *format, ...);

static void addBoss(TEmployee employee, TEmployee boss);
static void addChild(TEmployee employee, TEmployee child);

static void removeEmployee(TEmployee employee);
static void removeBosses(TEmployee employee);
static void removeChildren(TEmployee employee);

static void replaceEmployee(TEmployee old, TEmployee new);
static void replaceBosses(TEmployee old, TEmployee new);
static void replaceChildren(TEmployee old, TEmployee new);

static TEmployee getRoot(TEmployee employee);
static TEmployee getEmployeeFromState(char *employeeId);
static TEmployee newEmployee(char *employeeId, Properties *properties);

static TEmployee *searchEmployeesInState(Attributes *attributes);
static TEmployee *searchEmployees(TEmployee root, Attributes *attributes);

static bool hasProperties(TEmployee employee, Properties *properties);
static bool hasAttributes(TEmployee employee, Attributes *attributes);
static bool hasAttribute(TEmployee employee, Attribute *attribute);
static bool isPresent(TEmployee employee, TEmployee *employees);

static size_t concatenateEmployees(TEmployee *employees1, TEmployee *employees2, size_t size1, size_t size2);

static void fireEmployee(TEmployee employee);

static TEmployeeList getListFromState(char *id);
static TEmployeeList newList(char *id, TEmployee *employees);

static void addBoss(TEmployee employee, TEmployee boss) {
    TEmployee *temp = (TEmployee *) realloc(employee->bosses, (employee->bossesSize + 1) * sizeof(TEmployee));
    if (temp == NULL) {
        logError(logger, "No more memory available");
        // free(employee->bosses);
        return;
    }
    if(employee==NULL)
    {
        logError(logger, "No se puede agregar un jefe a un empleado nulo");
        return;
    }
    if(boss==NULL){
        logError(logger, "No se puede agregar un jefe nulo");
        return;
    }
    employee->bosses = temp;
    employee->bosses[employee->bossesCount] = boss;
    employee->bossesCount++;
    employee->bossesSize++;
}

static void addChild(TEmployee employee, TEmployee child) {
    TEmployee *temp = (TEmployee *) realloc(employee->children, (employee->childrenSize + 1) * sizeof(TEmployee));
    if (temp == NULL) {
        logError(logger, "No more memory available");
        // free(employee->children);
        return;
    }
    if(employee==NULL)
    {
        logError(logger, "No se puede agregar un hijo a un empleado nulo");
        return;
    }
    if(child==NULL){
        logError(logger, "No se puede agregar un hijo nulo");
        return;
    }
    employee->children = temp;
    employee->children[employee->childrenCount] = child;
    employee->childrenSize++;
    employee->childrenCount++;
}

static void generateRemoveExpression(RemoveExpression *removeExpression) {
    TEmployee employee = getEmployeeFromState(removeExpression->idToRemove);
    if (employee == NULL)
        logError(logger, "No se encontro el empleado con id %s", removeExpression->idToRemove);
    else
        removeEmployee(employee);
}

static void removeEmployee(TEmployee employee) {
    if (employee == NULL) {
        logError(logger, "No se puede remover un empleado que no existe");
        return;
    }
    removeBosses(employee);
    removeChildren(employee);
}

static void removeBosses(TEmployee employee) {
    if(employee==NULL){
        logError(logger, "No se puede remover los jefes de un empleado nulo");
        return;
    }
    for (size_t i = 0; i < employee->bossesSize; i++) {
        TEmployee boss = employee->bosses[i];
        if (boss != NULL) {
            for (size_t j = 0; j < boss->childrenSize; j++) {
                if(boss->children[j]!=NULL){
                    if (boss->children[j] == employee) {
                        boss->children[j] = NULL;
                        boss->childrenCount--;
                    }
                }
                
            }
            employee->bosses[i] = NULL;
            employee->bossesCount--;
        }
    }
}

static void removeChildren(TEmployee employee) {
    if(employee==NULL){
        logError(logger, "No se puede remover los hijos de un empleado nulo");
        return;
    }
    for (size_t i = 0; i < employee->childrenSize; i++) {
        TEmployee child = employee->children[i];
        if (child != NULL) {
            for (size_t j = 0; j < child->bossesSize; j++) {
               if(child->bosses[j]!=NULL){ 
                    if (child->bosses[j] == employee) {
                        child->bosses[j] = NULL;
                        child->bossesCount--;
                        if (child->bossesCount == 0)
                            removeChildren(child);
                        //removeEmployee(child);
                    }
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
    else{
        logError(logger, "No se puede reemplazar un empleado nulo");
        return;
    }
}

static void replaceBosses(TEmployee old, TEmployee new) {
    if(old==NULL){
        logError(logger, "No se puede reemplazar los jefes de un empleado nulo");
        return;
    }
    if(new==NULL){
        logError(logger, "No se puede reemplazar los jefes de un empleado por un empleado nulo");
        return;
    }
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
    if(old==NULL){
        logError(logger, "No se puede reemplazar los hijos de un empleado nulo");
        return;
    }
    if(new==NULL){
        logError(logger, "No se puede reemplazar los hijos de un empleado por un empleado nulo");
        return;
    }
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
    if(employee==NULL){
        logError(logger, "No se puede obtener la raiz de un empleado nulo");
        return NULL;
    }
    if (employee->bossesCount == 0)
        return employee;

    for (int i = 0; i < employee->bossesSize; i++) {
        if (employee->bosses[i] != NULL)
            return getRoot(employee->bosses[i]);
    }

    return NULL;
}

static void searchEmployeesRec(TEmployee employee, Attributes *attributes, TEmployee *employees, int *size) {
    if(employee==NULL){
        logError(logger, "No se puede buscar empleados nulos");
        return;
    }
    if (isPresent(employee, employees))
        return;
    if (hasAttributes(employee, attributes)) {
        TEmployee *temp = (TEmployee *) realloc(employees, (*size + 1) * sizeof(TEmployee));
        if (temp == NULL) {
            logError(logger, "No more memory available");
            // free(employees);
            return;
        }
        employees = temp;
        employees[*size] = employee;
        (*size)++;
    }

    for (size_t i = 0; i < employee->childrenCount; i++) {
        searchEmployeesRec(employee->children[i], attributes, employees, size);
    }
}

static TEmployee *searchEmployees(TEmployee root, Attributes *attributes) {
    TEmployee *employees = (TEmployee *) malloc(sizeof(TEmployee));
   

    if (root == NULL) {
        logError(logger, "No se puede buscar empleados si no hay un empleado raiz");
        return NULL;
    }
    if(attributes==NULL){
        logError(logger, "No se puede buscar empleados sin atributos");
        return NULL;
    }

    int size = 0;
    searchEmployeesRec(root, attributes, employees, &size);
    return employees;

}

static bool hasProperties(TEmployee employee, Properties *properties) {
    if(employee==NULL){
        logError(logger, "No se puede buscar propiedades en un empleado nulo");
        return false;
    }
    if(properties==NULL){
        logError(logger, "No se puede buscar propiedades nulas");
        return false;
    }
    return hasAttributes(employee, properties->attributes);
}

static bool hasAttributes(TEmployee employee, Attributes *attributes) {
    if(attributes==NULL){
        logError(logger, "No se puede buscar empleados sin atributos");
        return false;
    }
    if(employee==NULL){
        logError(logger, "No se puede buscar atributos en un empleado nulo");
        return false;
    }
    for (size_t i = 0; i < attributes->count; i++) {
        if (!hasAttribute(employee, attributes->attributes[i]))
            return false;
    }
    return true;
}

static bool hasAttribute(TEmployee employee, Attribute *attribute) {
    if(employee==NULL){
        logError(logger, "No se puede buscar atributos en un empleado nulo");
        return false;
    }
    if(attribute==NULL){
        logError(logger, "No se puede buscar un atributo nulo");
        return false;
    }
    for (size_t i = 0; i < employee->metadataCount; i++) {
        if (strcmp(employee->metadata[i].tag, attribute->tag) == 0) {
            switch (employee->metadata[i].metadataType) {
                case METADATA_STRING:
                    if (strcmp(employee->metadata[i].stringValue, attribute->stringValue) == 0)
                        return true;
                    break;
                case METADATA_INTEGER:
                    if (employee->metadata[i].numValue == attribute->numValue)
                        return true;
                    break;
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

static size_t concatenateEmployees(TEmployee *employees1, TEmployee *employees2, size_t size1, size_t size2) {
    if(employees1==NULL){
        logError(logger, "No se puede concatenar empleados nulos");
        return -1;
    }
    if (employees2 == NULL)
        return size1;
    int j = 0;
    TEmployee *temp = (TEmployee *) realloc(employees1, (size1 + size2 + 1) * sizeof(TEmployee));
    for (size_t i = 0; i < size2; i++) {
        if (employees2[i] != NULL && !isPresent(employees2[i], employees1)) {
            if (temp == NULL) {
                logError(logger, "No more memory available");
                // free(employees1);
                return -1;
            }
            employees1 = temp;
            employees1[size1 + j] = employees2[i];
            j++;
        }
    }
    return size1 + j;
}

static TEmployeeList getListFromState(char *id) {
    for (int i = 0; i < state->sizeLists; i++) {
        if (state->lists[i] != NULL && state->lists[i]->id != NULL) {
            if (strcmp(state->lists[i]->id, id) == 0)
                return state->lists[i];
        }
    }
    return NULL;
}

/**
 * Generates the output of the program.
 */
static void generateProgram(Program *program) {
    state = (GeneratorState *) calloc(1, sizeof(GeneratorState));

    if (state == NULL) {
        logError(logger, "No hay mas memoria disponible");
        return;
    }

    generateExpressions(program->expressions);
}

static void generateExpressions(Expressions *expressions) {
    if(expressions==NULL){
        logError(logger, "No se puede generar expresiones nulas");
        return;
    }
    for (int i = 0; i < expressions->count; i++) {
        generateExpression(expressions->expressions[i]);
    }
}

/**
 * Generates the output of an expression.
 */
static void generateExpression(Expression *expression) {
    if(expression==NULL){
        logError(logger, "No se puede generar una expresion nula");
        return;
    }
    switch (expression->type) {
        case VARIABLE_EMPLOYEE_EXPRESSION:
            generateVariableEmployeeExpression(expression->variableEmployeeExpression);
            break;
        case EMPLOYEE_EXPRESSION:
            generateEmployeeExpression(expression->employeeExpression);
            break;
        case REMOVE_EXPRESSION:
            generateRemoveExpression(expression->removeExpression);
            break;
        case REPLACE_EXPRESSION:
            generateReplaceExpression(expression->replaceExpression);
            break;
        case ASSIGN_EXPRESSION:
            generateAssignExpression(expression->assignExpression);
            break;
        case RELATIONSHIP_EXPRESSION:
            generateRelationshipExpression(expression->relationshipExpression);
            break;
        default:
            logError(logger, "The specified expression type is unknown: %d", expression->type);
            break;
    }
}

static void generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression) {

    if(variableEmployeeExpression==NULL){
        logError(logger, "No se puede generar una expresion de empleado nula");
        return;
    }
    newEmployee(variableEmployeeExpression->employeeId, variableEmployeeExpression->properties);
}

static void generateProperties(TEmployee employee, Properties *properties) {
    if(properties==NULL){
        logError(logger, "No se puede generar propiedades nulas");
        return;
    }
    if(employee==NULL){
        logError(logger, "No se puede agregar propiedades a un empleado nulo");
        return;
    }
    generateAttributes(employee, properties->attributes);
}

static void generateAttributes(TEmployee employee, Attributes *attributes) {
    if(attributes==NULL){
        logError(logger, "No se puede generar atributos nulos");
        return;
    }
    if(employee==NULL){
        logError(logger, "No se puede agregar atributos a un empleado nulo");
        return;
    }
    for (int i = 0; i < attributes->count; i++) {
        generateAttribute(employee, attributes->attributes[i]);
    }
}

static void generateAttribute(TEmployee employee, Attribute *attribute) {
    if(employee==NULL){
        logError(logger, "No se puede agregar un atributo a un empleado nulo");
        return;
    }
    if(attribute==NULL){
        logError(logger, "No se puede agregar un atributo nulo");
        return;
    }
     Metadata *temp = (Metadata *) realloc(employee->metadata, (employee->metadataCount + 1) * sizeof(Metadata));
    if (temp == NULL) {
        logError(logger, "No more memory available");
        // free(employee->metadata);
        return;
    }
    employee->metadata = temp;

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

static void generateEmployeeExpression(EmployeeExpression *employeeExpression) {
    if(employeeExpression==NULL){
        logError(logger, "No se puede generar una expresion de empleado nula");
        return;
    }
    TEmployee employee = getEmployeeFromState(employeeExpression->employeeId);
    TEmployeeList list;
    TEmployee root;

    if (employee == NULL)
        employee = newEmployee(employeeExpression->employeeId, employeeExpression->properties);
    else
        generateProperties(employee, employeeExpression->properties);

    switch (employeeExpression->list->listType) {
        case LIST_PROPERTIES:
            root = getRoot(employee);
            if (root == NULL) {
                logError(logger, "No se puede buscar empleados si no hay un empleado raiz");
                return;
            }
            TEmployee *employees = searchEmployees(root, employeeExpression->list->attributes);
            if (employees == NULL || employees[0] == NULL) {
                logError(logger, "No se encontraron empleados con las propiedades especificadas");
                return;
            }

            int size = sizeof(employees) / sizeof(employees[0]);
            for (int i = 0; i < size; i++) {
                addBoss(employee, employees[i]);
                addChild(employees[i], employee);
            }
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < employeeExpression->list->elements->count; i++) {
                TEmployee employeeAux = getEmployeeFromState(employeeExpression->list->elements->ids[i]);
                if (employeeAux == NULL) {
                    logError(logger, "No se encontro el empleado con id %s",
                             employeeExpression->list->elements->ids[i]);
                } else {
                    addBoss(employee, employeeAux);
                    addChild(employeeAux, employee);
                }
            }
            break;

        case LIST_EMPLOYEE:
            list = getListFromState(employeeExpression->list->employeeId);
            if (list == NULL) {
                logError(logger, "No se encontro la lista con id %s", employeeExpression->list->employeeId);
                return;
            }
            for (int i = 0; i < list->employeesSize; i++) {
                if(list->employees[i] != NULL) {
                    addBoss(employee, list->employees[i]);
                    addChild(list->employees[i], employee);
                }
            }
            break;
    }
}

static void generateReplaceExpression(ReplaceExpression *replaceExpression) {
    if(replaceExpression==NULL){
        logError(logger, "No se puede generar una expresion de reemplazo nula");
        return;
    }
    TEmployee old = getEmployeeFromState(replaceExpression->idToReplace);
    if (old == NULL) {
        logError(logger, "No se encontro el empleado con id %s", replaceExpression->idToReplace);
        return;
    }

    TEmployee new;
    if (replaceExpression->properties != NULL) {
        new = newEmployee(replaceExpression->idToReplaceWith, replaceExpression->properties);
        if (new == NULL) {
            logError(logger, "No se pudo crear el empleado con id %s", replaceExpression->idToReplaceWith);
            return;
        }
    } else {
        new = getEmployeeFromState(replaceExpression->idToReplaceWith);
        if (new == NULL) {
            logError(logger, "No se encontro el empleado con id %s", replaceExpression->idToReplaceWith);
            return;
        }
    }

    replaceEmployee(old, new);
}

static void generateAssignExpression(AssignExpression *assignExpression) {
    if(assignExpression==NULL){
        logError(logger, "No se puede generar una expresion de asignacion nula");
        return;
    }
    TEmployee *employees = NULL;
    TEmployeeList aux;
    size_t size = 0;

    switch (assignExpression->list->listType) {
        case LIST_PROPERTIES:
            employees = searchEmployeesInState(assignExpression->list->attributes);
            if (employees == NULL || employees[0] == NULL) {
                logError(logger, "No se encontraron empleados con las propiedades especificadas");
                exit(1);
            }
            size = sizeof(employees) / sizeof(employees[0]);
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < assignExpression->list->elements->count; i++) {
                TEmployee employeeAux = getEmployeeFromState(assignExpression->list->elements->ids[i]);
                size = concatenateEmployees(employees, &employeeAux, size, 1);
            }
            break;
        case LIST_EMPLOYEE:
            aux = getListFromState(assignExpression->list->employeeId);
            if (aux == NULL) {
                logError(logger, "No se encontraron empleados con las propiedades especificadas");
                exit(1);
            }
            employees = aux->employees;
            size = aux->employeesSize;
            break;
    }
    TEmployeeList list = NULL;
    switch (assignExpression->employees->employeesType) {
        case LIST:
            list = newList(assignExpression->employees->employeesId, employees);
            break;
        case VARIABLE:
            list = getListFromState(assignExpression->list->employeeId);
            if (list == NULL) {
                logError(logger, "No se encontro el empleado con id %s", assignExpression->employees->employeesId);
                exit(1);
            }
            list->employees = employees;
            list->employeesCount = size;
            list->employeesSize = size;
            break;
    }
}

static TEmployee *searchEmployeesInState(Attributes *attributes) {
    if(attributes==NULL){
        logError(logger, "No se puede buscar empleados sin atributos");
        return NULL;
    }
    TEmployee *employees = NULL;

    for (int i = 0; i < state->sizeEmployees; i++) {
        if (state->employees[i] != NULL && hasAttributes(state->employees[i], attributes)) {
            TEmployee *temp = (TEmployee *) realloc(employees, (i + 1) * sizeof(TEmployee));
            if (temp == NULL) {
                logError(logger, "No more memory available");
                free(employees);
                return NULL;
            }
            employees = temp;
            employees[i] = state->employees[i];
        }
    }
    return employees;
}

static void generateRelationshipExpression(RelationshipExpression *relationshipExpression) {
    if(relationshipExpression==NULL){
        logError(logger, "No se puede generar una expresion de relacion nula");
        return;
    }
    TEmployee *employees = NULL;
    TEmployeeList list = NULL;
    size_t size = 0;

    switch (relationshipExpression->list->listType) {
        case LIST_PROPERTIES:
            employees = searchEmployeesInState(relationshipExpression->list->attributes);
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < relationshipExpression->list->elements->count; i++) {
                printf("%d",relationshipExpression->list->elements->count);
                printf("\n%s\n",relationshipExpression->list->elements->ids[i]);
                TEmployee employeeAux = getEmployeeFromState(relationshipExpression->list->elements->ids[i]);
                size = concatenateEmployees(employees, &employeeAux, size, 1);
            }
            break;
        case LIST_EMPLOYEE:
            list = getListFromState(relationshipExpression->list->employeeId);
            if (list == NULL) {
                logError(logger, "No se encontraron empleados con las propiedades especificadas");
                return;
            }
            employees = list->employees;

            break;
    }

    if (employees == NULL || employees[0] == NULL) {
        logError(logger, "No se encontraron empleados con las propiedades especificadas");
        return;
    }

    TEmployee *employeesInRelationship = NULL;
    size = 0;
    int aux = sizeof(employees) / sizeof(employees[0]);

    switch (relationshipExpression->relationship->relationshipType) {
        case RELATIONSHIP_CHILD_AND_SELF:

            for (int i = 0; i < aux; i++) {
                size = concatenateEmployees(employeesInRelationship, &employees[i], size, 1);
                size = concatenateEmployees(employeesInRelationship, employees[i]->children, size,
                                            employees[i]->childrenSize);
            }
            break;
        case RELATIONSHIP_CHILD:
            for (int i = 0; i < aux; i++)
                size = concatenateEmployees(employeesInRelationship, employees[i]->children, size,
                                            employees[i]->childrenSize);
            break;

        case RELATIONSHIP_SIBLING:
            for (int i = 0; i < aux; i++) {
                if (employees[i] != NULL) {
                    for (int j = 0; employees[i]->bossesSize; j++) {
                        if (employees[i]->bosses[j] != NULL) {
                            size = concatenateEmployees(employeesInRelationship, employees[i]->bosses[j]->children,
                                                        size,
                                                        employees[i]->bosses[j]->childrenSize);
                        }
                    }
                }
            }
            break;
    }

    TEmployee *bosses = NULL;
    TEmployeeList bossesList = NULL;
    size_t bossesSize = 0;

    switch (relationshipExpression->hierarchy->list->listType) {
        case LIST_PROPERTIES:
            bosses = searchEmployeesInState(relationshipExpression->hierarchy->list->attributes);
            if (bosses == NULL) {
                logError(logger, "No se encontraron empleados con las propiedades especificadas");
                return;
            }
            break;
        case LIST_EMPLOYEE:
            bossesList = getListFromState(relationshipExpression->hierarchy->list->employeeId);
            if (bossesList == NULL) {
                logError(logger, "No se encontro la lista con id %s", relationshipExpression->hierarchy->list->employeeId);
                return;
            }
            bosses = bossesList->employees;
            break;
        case LIST_ELEMENTS:
            for (int i = 0; i < relationshipExpression->hierarchy->list->elements->count; i++) {
                TEmployee employeeAux = getEmployeeFromState(relationshipExpression->hierarchy->list->elements->ids[i]);
                bossesSize = concatenateEmployees(bosses, &employeeAux, bossesSize, 1);
            }
            break;
    }

    if (bosses == NULL || bosses[0] == NULL) {
        logError(logger, "No se encontraron empleados con las propiedades especificadas");
        return;
    }
    aux = sizeof(bosses) / sizeof(bosses[0]);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < aux; j++) {
            addBoss(employeesInRelationship[i], bosses[j]);
            addChild(bosses[j], employeesInRelationship[i]);
        }
    }
}

// Destruye al nodo empleado
static void fireEmployee(TEmployee employee) {
    if (employee != NULL) {
        removeEmployee(employee);
        for (int i = 0; i < state->sizeEmployees; i++) {
            if (state->employees[i] == employee) {
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
static void generatePrologue(void) {
    fprintf(file, "%s", "{ \"nodes\": [");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void generateEpilogue() {
    fprintf(file, "%s", "] }\n");
}

/**
 * Generates an myIndentation string for the specified level.
 */
static char *myIndentation(const unsigned int level) {
    return indentation(indentationCharacter, level, indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void output(const unsigned int indentationLevel, const char *const format, ...) {
    va_list arguments;
    va_start(arguments, format);
    char *indentation = myIndentation(indentationLevel);
    char *effectiveFormat = concatenate(2, indentation, format);
    vfprintf(stdout, effectiveFormat, arguments);
    free(effectiveFormat);
    free(indentation);
    va_end(arguments);
}

static TEmployee newEmployee(char *employeeId, Properties *properties) {
    if (employeeId == NULL) {
        logError(logger, "No se puede crear un empleado sin id");
        exit(1);
    } else if (getEmployeeFromState(employeeId) != NULL) {
        logError(logger, "El empleado ya existe");
        exit(1);
    }

    TEmployee *temp = (TEmployee *) realloc(state->employees, (state->sizeEmployees + 1) * sizeof(TEmployee));
    if (temp == NULL) {
        logError(logger, "No more memory available");
        // free(state->employees);
        exit(1);
    }
    state->employees = temp;

    state->sizeEmployees++;

    TEmployee employee = (TEmployee) malloc(sizeof(struct Employee));
    if (employee == NULL) {
        logError(logger, "No hay mas memoria disponible");
        exit(2);
    }
    employee->employeeId = employeeId;
    employee->metadata = NULL;
    employee->metadataCount = 0;
    employee->children = NULL;
    employee->childrenCount = 0;
    employee->bosses = NULL;
    employee->bossesCount = 0;
    generateProperties(employee, properties);

    state->employees[state->sizeEmployees - 1] = employee;

    return employee;
}

static TEmployeeList newList(char *id, TEmployee *employees) {
    TEmployeeList *temp = (TEmployeeList *) realloc(state->lists, (state->sizeLists + 1) * sizeof(TEmployeeList));
    if (temp == NULL) {
        logError(logger, "No more memory available");
        free(state->lists);
        exit(2);
    }
    state->lists = temp;

    TEmployeeList list = (TEmployeeList) malloc(sizeof(struct EmployeeList));
    if (list == NULL) {
        logError(logger, "No hay mas memoria disponible");
        exit(2);
    }

    list->id = id;
    list->employees = employees;
    list->employeesSize = sizeof(employees) / sizeof(employees[0]);
    list->employeesCount = list->employeesSize;

    state->lists[state->sizeLists] = list;
    state->sizeLists++;
    return list;
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
    logDebugging(logger, "Generating final output...");
    file = fopen("project.json", "w");
    generatePrologue();
    generateProgram(compilerState->abstractSyntaxtTree);
    generateEpilogue();
    logDebugging(logger, "Generation is done.");
}
