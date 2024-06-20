#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeAbstractSyntaxTreeModule() { _logger = createLogger("AbstractSyntaxTree"); }

void shutdownAbstractSyntaxTreeModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

void releaseVariableEmployeeExpression(VariableEmployeeExpression* variableEmployeeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (variableEmployeeExpression != NULL) {
        free(variableEmployeeExpression->employeeId);
        releaseProperties(variableEmployeeExpression->properties);
        free(variableEmployeeExpression);
    }
}

void releaseEmployeeExpression(EmployeeExpression* employeeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (employeeExpression != NULL) {
        free(employeeExpression->employeeId);
        releaseHierarchy(employeeExpression->hierarchy);
        releaseProperties(employeeExpression->properties);
        free(employeeExpression);
    }
}

void releaseRemoveExpression(RemoveExpression* removeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (removeExpression != NULL) {
        free(removeExpression->idToRemove);
        free(removeExpression);
    }
}

void releaseReplaceExpression(ReplaceExpression* replaceExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (replaceExpression != NULL) {
        free(replaceExpression->idToReplace);
        free(replaceExpression->idToReplaceWith);
        releaseProperties(replaceExpression->properties);
        free(replaceExpression);
    }
}

void releaseAssignExpression(AssignExpression* assignExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (assignExpression != NULL) {
        releaseEmployees(assignExpression->employees);
        releaseList(assignExpression->list);
        free(assignExpression);
    }
}

void releaseRelationshipExpression(RelationshipExpression* relationshipExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (relationshipExpression != NULL) {
        releaseList(relationshipExpression->list);
        releaseRelationship(relationshipExpression->relationship);
        releaseHierarchy(relationshipExpression->hierarchy);
        free(relationshipExpression);
    }
}

void releaseExpression(Expression* expression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (expression != NULL) {
        switch (expression->type) {
            case VARIABLE_EMPLOYEE_EXPRESSION:
                releaseVariableEmployeeExpression(expression->variableEmployeeExpression);
                break;
            case EMPLOYEE_EXPRESSION:
                releaseEmployeeExpression(expression->employeeExpression);
                break;
            case REMOVE_EXPRESSION:
                releaseRemoveExpression(expression->removeExpression);
                break;
            case REPLACE_EXPRESSION:
                releaseReplaceExpression(expression->replaceExpression);
                break;
            case ASSIGN_EXPRESSION:
                releaseAssignExpression(expression->assignExpression);
                break;
            case RELATIONSHIP_EXPRESSION:
                releaseRelationshipExpression(expression->relationshipExpression);
                break;
        }
        free(expression);
    }
}

void releaseProgram(Program* program) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (program != NULL) {
        releaseExpressions(program->expressions);
        free(program);
    }
}

void releaseExpressions(Expressions* expressions) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (expressions != NULL) {
        for (int i = 0; i < expressions->count; i++) releaseExpression(expressions->expressions[i]);
        free(expressions->expressions);
        free(expressions);
    }
}

void releaseAttribute(Attribute* attribute) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (attribute != NULL) {
        free(attribute->tag);
        if (attribute->attributeType == ATTRIBUTE_STRING)
            free(attribute->stringValue);
        free(attribute);
    }
}

void releaseAttributes(Attributes* attributes) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (attributes != NULL) {
        for (int i = 0; i < attributes->count; i++) releaseAttribute(attributes->attributes[i]);
        free(attributes->attributes);
        free(attributes);
    }
}

void releaseProperties(Properties* properties) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (properties != NULL) {
        releaseAttributes(properties->attributes);
        free(properties);
    }
}

void releaseHierarchy(Hierarchy* hierarchy) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (hierarchy != NULL) {
        releaseList(hierarchy->list);
        free(hierarchy);
    }
}

void releaseEmployees(Employees* employees) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (employees != NULL) {
        free(employees->employeesId);
        free(employees);
    }
}

void releaseList(List* list) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (list != NULL) {
        switch (list->listType) {
            case LIST_PROPERTIES:
                releaseProperties(list->properties);
                break;
            case LIST_EMPLOYEE:
                free(list->employeeId);
                break;
            case LIST_ELEMENTS:
                releaseElements(list->elements);
                break;
        }
        free(list);
    }
}

void releaseElements(Elements* elements) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (elements != NULL) {
        for (int i = 0; i < elements->count; i++) 
            free(elements->ids[i]);
        free(elements->ids);
        free(elements);
    }
}

void releaseRelationship(Relationship* relationship) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (relationship != NULL) {
        free(relationship);
    }
}
