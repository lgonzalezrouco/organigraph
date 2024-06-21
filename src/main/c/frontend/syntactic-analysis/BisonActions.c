#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeBisonActionsModule() { _logger = createLogger("BisonActions"); }

void shutdownBisonActionsModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char* functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char* functionName) { logDebugging(_logger, "%s", functionName); }

/* PUBLIC FUNCTIONS */

Program* ExpressionsProgramSemanticAction(CompilerState* compilerState, Expressions* expressions) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program* program = calloc(1, sizeof(Program));
    program->expressions = expressions;
    compilerState->abstractSyntaxtTree = program;
    if (0 < flexCurrentContext()) {
        logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
        compilerState->succeed = false;
    } else {
        compilerState->succeed = true;
    }
    return program;
}

Expressions* AppendExpressionSemanticAction(Expressions* expressions, Expression* expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expressions* newExpressions = calloc(1, sizeof(Expressions));
    newExpressions->count = expressions->count + 1;
    newExpressions->expressions = calloc(newExpressions->count, sizeof(Expression));
    for (int i = 0; i < expressions->count; i++) {
        newExpressions->expressions[i] = expressions->expressions[i];
    }
    newExpressions->expressions[expressions->count] = expression;
    return newExpressions;
}

Expressions* ExpressionSemanticAction(Expression* expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expressions* expressions = calloc(1, sizeof(Expressions));
	expressions->count = 1;
	expressions->expressions = calloc(1, sizeof(Expression));
	expressions->expressions[0] = expression;
	return expressions;
}

Expression* VariableEmployeeExpressionSemanticAction(char* employeeId, Properties* properties) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = VARIABLE_EMPLOYEE_EXPRESSION;
    expression->variableEmployeeExpression = calloc(1, sizeof(struct VariableEmployeeExpression));
    expression->variableEmployeeExpression->employeeId = employeeId;
    expression->variableEmployeeExpression->properties = properties;
    return expression;
}

Expression* EmployeeExpressionSemanticAction(char* employeeId, List* list,
                                             Properties* properties) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = EMPLOYEE_EXPRESSION;
    expression->employeeExpression = calloc(1, sizeof(EmployeeExpression));
    expression->employeeExpression->employeeId = employeeId;
    expression->employeeExpression->list = list;
    expression->employeeExpression->properties = properties;
    return expression;
}
Expression* RemoveExpressionSemanticAction(char* idToRemove) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = REMOVE_EXPRESSION;
    expression->removeExpression = calloc(1, sizeof(RemoveExpression));
    expression->removeExpression->idToRemove = idToRemove;
    return expression;
}
Expression* ReplaceExpressionSemanticAction(char* idToReplace, char* idToReplaceWith, Properties* properties) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = REPLACE_EXPRESSION;
    expression->replaceExpression = calloc(1, sizeof(ReplaceExpression));
    expression->replaceExpression->idToReplace = idToReplace;
    expression->replaceExpression->idToReplaceWith = idToReplaceWith;
    expression->replaceExpression->properties = properties;
    return expression;
}
Expression* AssignExpressionSemanticAction(Employees* employees, List* list) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = ASSIGN_EXPRESSION;
    expression->assignExpression = calloc(1, sizeof(AssignExpression));
    expression->assignExpression->employees = employees;
    expression->assignExpression->list = list;
    return expression;
}

Expression* RelationshipExpressionSemanticAction(List* list, Relationship* relationship, 
                                                 Hierarchy* hierarchy) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression* expression = calloc(1, sizeof(Expression));
    expression->type = RELATIONSHIP_EXPRESSION;
    expression->relationshipExpression = calloc(1, sizeof(RelationshipExpression));
    expression->relationshipExpression->list = list;
    expression->relationshipExpression->relationship = relationship;
    expression->relationshipExpression->hierarchy = hierarchy;
    return expression;
}

Attributes* AppendAttributesSemanticAction(Attributes* attributes, Attribute* attribute) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Attributes* newAttributes = calloc(1, sizeof(Attributes));
    newAttributes->count = attributes->count + 1;
    newAttributes->attributes = calloc(newAttributes->count, sizeof(Attribute));
    for (int i = 0; i < attributes->count; i++) {
        newAttributes->attributes[i] = attributes->attributes[i];
    }
    newAttributes->attributes[attributes->count] = attribute;
    return newAttributes;
}

Attributes* AttributeSemanticAction(Attribute* attribute) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Attributes* attributes = calloc(1, sizeof(Attributes));
    attributes->count = 1;
    attributes->attributes = calloc(1, sizeof(Attribute));
    attributes->attributes[0] = attribute;
    return attributes;
}

Attribute* StringMetadataSemanticAction(char* metadataId, char* value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Attribute* attribute = calloc(1, sizeof(Attribute));
    attribute->attributeType = ATTRIBUTE_STRING;
    attribute->tag = metadataId;
    attribute->stringValue = value;
    return attribute;
}

Attribute* IntegerMetadataSemanticAction(char* metadataId, int value) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Attribute* attribute = calloc(1, sizeof(Attribute));
    attribute->attributeType = ATTRIBUTE_INTEGER;
    attribute->tag = metadataId;
    attribute->numValue = value;
    return attribute;
}

Properties* PropertiesSemanticAction(Attributes* attributes) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (attributes == NULL)
        return NULL;
    Properties* properties = calloc(1, sizeof(Properties));
    properties->attributes = attributes;
    return properties;
}

Hierarchy* HierarchySemanticAction(List* list) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Hierarchy* hierarchy = calloc(1, sizeof(Hierarchy));
    hierarchy->list = list;
    return hierarchy;
}

Employees* EmployeesSemanticAction(char* employeeId) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Employees* employees = calloc(1, sizeof(Employees));
    employees->employeesId = employeeId;
    employees->employeesType = LIST;
    return employees;
}

Employees* VariableEmployeesSemanticAction(char* employeeId) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Employees* employees = calloc(1, sizeof(Employees));
    employees->employeesId = employeeId;
    employees->employeesType = VARIABLE;

    return employees;
}

List* ListSemanticAction(Attributes * attributes) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    List* list = calloc(1, sizeof(List));
    list->listType = LIST_PROPERTIES;
    list->attributes = attributes;
    return list;
}

List* VariableListSemanticAction(char* employeesId) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    List* list = calloc(1, sizeof(List));
    list->listType = LIST_EMPLOYEE;
    list->employeeId = employeesId;
    return list;
}

List* ElementsListSemanticAction(Elements* elements) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    List* list = calloc(1, sizeof(List));
    list->listType = LIST_ELEMENTS;
    list->elements = elements;
    return list;
}

Elements* AppendElementsSemanticAction(Elements* elements, char* employeeId) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Elements* newElements = calloc(1, sizeof(Elements));
    newElements->count = elements->count + 1;
    newElements->ids = calloc(newElements->count, sizeof(char*));
    for (int i = 0; i < elements->count; i++) {
        newElements->ids[i] = elements->ids[i];
    }
    newElements->ids[elements->count] = employeeId;
    return newElements;
}

Elements* ElementSemanticAction(char* employeesId) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Elements* elements = calloc(1, sizeof(Elements));
    elements->count = 1;
    elements->ids = calloc(1, sizeof(char*));

    // TODO: check if calloc is necessary
    /* elements->ids[0] = calloc(strlen(employeesId) + 1, sizeof(char));
    strcpy(elements->ids[0], employeesId);
    elements->ids[0][strlen(employeesId)] = '\0'; */

    elements->ids[0] = employeesId;
    return elements;
}

Relationship* ChildSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Relationship* relationship = calloc(1, sizeof(Relationship));
    relationship->relationshipType = RELATIONSHIP_CHILD;
    return relationship;
}

Relationship* SiblingSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Relationship* relationship = calloc(1, sizeof(Relationship));
    relationship->relationshipType = RELATIONSHIP_SIBLING;
    return relationship;
}

Relationship* ChildAndSelfSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Relationship* relationship = calloc(1, sizeof(Relationship));
    relationship->relationshipType = RELATIONSHIP_CHILD_AND_SELF;
    return relationship;
}
