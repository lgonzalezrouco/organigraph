/*#include "semanticAnalyzer.h"
#include "AbstractSyntaxTree.h"
#include "SymbolTable.h"
#include <stdio.h>

void printSemanticError(const char* message, int lineNumber) {
    fprintf(stderr, "Semantic error at line %d: %s\n", lineNumber, message);
}

void checkExpressionType(Expression* expression, SymbolTable* symbolTable) {
    if (expression == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to expression type-checking function.\n");
        return;
    }

    switch (expression->type) {
        case PROJECT_EXPRESSION: {
            ProjectExpression* projExpr = expression->projectExpression;
            Symbol* symbol = getSymbol(symbolTable, projExpr->projectId);
            if (symbol == NULL || symbol->type != TOKEN_PROJECT) {
                printSemanticError("Undefined or incorrect type for project expression", projExpr->lineNumber);
            }
            break;
        }
        case VARIABLE_EMPLOYEE_EXPRESSION: {
            VariableEmployeeExpression* varEmpExpr = expression->variableEmployeeExpression;
            Symbol* symbol = getSymbol(symbolTable, varEmpExpr->employeeId);
            if (symbol == NULL || symbol->type != TOKEN_EMPLOYEE) {
                printSemanticError("Undefined or incorrect type for variable employee expression", varEmpExpr->lineNumber);
            }
            checkPropertiesType(varEmpExpr->properties->attributes, symbolTable);
            break;
        }
        case EMPLOYEE_EXPRESSION: {
            EmployeeExpression* empExpr = expression->employeeExpression;
            Symbol* symbol = getSymbol(symbolTable, empExpr->employeeId);
            if (symbol == NULL || symbol->type != TOKEN_EMPLOYEE) {
                printSemanticError("Undefined or incorrect type for employee expression", empExpr->lineNumber);
            }
            checkPropertiesType(empExpr->properties->attributes, symbolTable);
            checkHierarchyType(empExpr->hierarchy, symbolTable);
            break;
        }
        case REMOVE_EXPRESSION: {
            RemoveExpression* remExpr = expression->removeExpression;
            Symbol* symbol = getSymbol(symbolTable, remExpr->idToRemove);
            if (symbol == NULL) {
                printSemanticError("Undefined symbol in remove expression", remExpr->lineNumber);
            }
            break;
        }
        case REPLACE_EXPRESSION: {
            ReplaceExpression* repExpr = expression->replaceExpression;
            Symbol* symbol = getSymbol(symbolTable, repExpr->idToReplace);
            if (symbol == NULL) {
                printSemanticError("Undefined symbol in replace expression", repExpr->lineNumber);
            }
            checkDefineType(repExpr->define, symbolTable);
            break;
        }
        case ASSIGN_EXPRESSION: {
            AssignExpression* assignExpr = expression->assignExpression;
            Symbol* empSymbol = getSymbol(symbolTable, assignExpr->employees->employeesId);
            if (empSymbol == NULL || empSymbol->type != TOKEN_EMPLOYEE) {
                printSemanticError("Undefined or incorrect type for assigned employees", assignExpr->lineNumber);
            }
            checkListType(assignExpr->list, symbolTable);

            // Verificar tipo de asignación MAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if (assignExpr->assignmentType != ASSIGN_TYPE_EQUAL) {
                printSemanticError("Unsupported assignment type", assignExpr->lineNumber);
            }
            break;
        }
        case RELATIONSHIP_EXPRESSION: {
            RelationshipExpression* relExpr = expression->relationshipExpression;
            checkListType(relExpr->list, symbolTable);
            checkRelationshipType(relExpr->relationship);
            // Verificar projectId y hierarchy si es necesario
            break;
        }
        case LIST_RELATIONSHIP_EXPRESSION: {
            ListRelationshipExpression* listRelExpr = expression->listRelationshipExpression;
            checkListType(listRelExpr->list, symbolTable);
            checkRelationshipType(listRelExpr->relationship);
            break;
        }
        case LIST_EXPRESSION: {
            ListExpression* listExpr = expression->listExpression;
            checkListType(listExpr->list, symbolTable);
            break;
        }
        default:
            fprintf(stderr, "Warning: Unhandled expression type in type-checking function.\n");
            break;
    }
}

void checkPropertiesType(Attributes* attributes, SymbolTable* symbolTable) {
    if (attributes == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to properties type-checking function.\n");
        return;
    }

    for (int i = 0; i < attributes->count; ++i) {
        Attribute* attr = attributes->attributes[i];
        switch (attr->attributeType) {
            case ATTRIBUTE_STRING:
                if (attr->stringValue == NULL) {
                    printSemanticError("String attribute value cannot be NULL", attr->lineNumber);
                }
                break;
            case ATTRIBUTE_INTEGER:
                
                break;
            default:
                fprintf(stderr, "Warning: Unhandled attribute type in properties type-checking function.\n");
                break;
        }
    }
}

void checkHierarchyType(Hierarchy* hierarchy, SymbolTable* symbolTable) {
    if (hierarchy == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to hierarchy type-checking function.\n");
        return;
    }

    checkListType(hierarchy->list, symbolTable);
}

void checkDefineType(Define* define, SymbolTable* symbolTable) {
    if (define == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to define type-checking function.\n");
        return;
    }

    switch (define->defineType) {
        case DEFINE_EMPLOYEE: {
            Symbol* empSymbol = getSymbol(symbolTable, define->employeeId);
            if (empSymbol == NULL || empSymbol->type != TOKEN_EMPLOYEE) {
                printSemanticError("Undefined or incorrect type for defined employee", define->lineNumber);
            }
            break;
        }
        case DEFINE_PROPERTIES: {
            checkPropertiesType(define->properties->attributes, symbolTable);
            break;
        }
        default:
            fprintf(stderr, "Warning: Unhandled define type in define type-checking function.\n");
            break;
    }
}

void checkListType(List* list, SymbolTable* symbolTable) {
    if (list == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to list type-checking function.\n");
        return;
    }

    switch (list->listType) {
        case LIST_PROPERTIES: {
            checkPropertiesType(list->properties->attributes, symbolTable);
            break;
        }
        case LIST_EMPLOYEE: {
            Symbol* empSymbol = getSymbol(symbolTable, list->employeeId);
            if (empSymbol == NULL || empSymbol->type != TOKEN_EMPLOYEE) {
                printSemanticError("Undefined or incorrect type for employee list", list->lineNumber);
            }
            break;
        }
        case LIST_ELEMENTS: {
            // Aquí podrían agregarse más verificaciones según sea necesario para elementos de lista
            break;
        }
        default:
            fprintf(stderr, "Warning: Unhandled list type in list type-checking function.\n");
            break;
    }
}

void checkRelationshipType(Relationship* relationship) {
    if (relationship == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to relationship type-checking function.\n");
        return;
    }

    // Aquí podrían agregarse verificaciones específicas para el tipo de relación si es necesario
}

void checkSemanticAnalysis(Program* program, SymbolTable* symbolTable) {
    if (program == NULL || symbolTable == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to semantic analysis function.\n");
        return;
    }

    for (int i = 0; i < program->expressions->count; ++i) {
        checkExpressionType(program->expressions->expressions[i], symbolTable);
    }
}
