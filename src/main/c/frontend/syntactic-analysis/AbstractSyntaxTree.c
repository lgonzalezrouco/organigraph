#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

void initializeAbstractSyntaxTreeModule() { _logger = createLogger("AbstractSyntxTree"); }

void shutdownAbstractSyntaxTreeModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/** PUBLIC FUNCTIONS */

void releaseProjectExpression(ProjectExpression* projectExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (projectExpression != NULL) {
        // free(projectExpression->projectId);
        free(projectExpression);
    }
}

void releaseVariableEmployeeExpression(VariableEmployeeExpression* variableEmployeeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (variableEmployeeExpression != NULL) {
        // free(variableEmployeeExpression->employeeId);
        releaseProperties(variableEmployeeExpression->properties);
        free(variableEmployeeExpression);
    }
}

void releaseEmployeeExpression(EmployeeExpression* employeeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (employeeExpression != NULL) {
        // free(employeeExpression->employeeId);
        // free(employeeExpression->projectId);
        releaseHierarchy(employeeExpression->hierarchy);
        releaseProperties(employeeExpression->properties);
        free(employeeExpression);
    }
}

void releaseRemoveExpression(RemoveExpression* removeExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (removeExpression != NULL) {
        // free(removeExpression->idToRemove);
        // free(removeExpression->projectId);
        free(removeExpression);
    }
}

void releaseReplaceExpression(ReplaceExpression* replaceExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (replaceExpression != NULL) {
        // free(replaceExpression->idToReplace);
        // free(replaceExpression->projectId);
        releaseDefine(replaceExpression->define);
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
        // free(relationshipExpression->projectId);
        releaseHierarchy(relationshipExpression->hierarchy);
        free(relationshipExpression);
    }
}

void releaseListRelationshipExpression(ListRelationshipExpression* listRelationshipExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (listRelationshipExpression != NULL) {
        releaseList(listRelationshipExpression->list);
        releaseRelationship(listRelationshipExpression->relationship);
        free(listRelationshipExpression);
    }
}

void releaseListExpression(ListExpression* listExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (listExpression != NULL) {
        releaseList(listExpression->list);
        free(listExpression);
    }
}

void releaseExpression(Expression* expression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (expression != NULL) {
        switch (expression->type) {
            case PROJECT_EXPRESSION:
                releaseProjectExpression(expression->projectExpression);
                break;
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
            case LIST_EXPRESSION:
                releaseListExpression(expression->listExpression);
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
        // free(attribute->tag);
        // if (attribute->attributeType == ATTRIBUTE_STRING)
            // free(attribute->stringValue);
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

void releaseDefine(Define* define) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (define != NULL) {
        switch (define->defineType) {
            case DEFINE_EMPLOYEE:
                // free(define->employeeId);
                break;
            case DEFINE_PROPERTIES:
                releaseProperties(define->properties);
                break;
        }
        free(define);
    }
}

void releaseEmployees(Employees* employees) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (employees != NULL) {
        // free(employees->employeesId);
        free(employees);
    }
}

void releaseList(List* list) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (list != NULL) {
        switch (list->listType) {
            case LIST_PROPERTIES:
                releaseProperties(list->properties);
                // free(list->projectId);
                break;
            case LIST_EMPLOYEE:
                // free(list->employeeId);
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
        // TODO: check if freeing each elements of ids is necessary
        for (int i = 0; i < elements->count; i++) // free(elements->ids[i]);
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
