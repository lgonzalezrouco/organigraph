#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include <stdlib.h>

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program* ExpressionsProgramSemanticAction(CompilerState* compilerState, Expressions* expressions);
Expressions* AppendExpressionSemanticAction(Expressions* expressions, Expression* expression);
Expressions* ExpressionSemanticAction(Expression* expression);
Expression* VariableEmployeeExpressionSemanticAction(char* employeeId, Properties* properties);
Expression* EmployeeExpressionSemanticAction(char* employeeId, List* list, Properties* properties);
Expression* RemoveExpressionSemanticAction(char* idToRemove);
Expression* ReplaceExpressionSemanticAction(char* idToReplace, char* idToReplaceWith, Properties* properties);
Expression* AssignExpressionSemanticAction(Employees* employees, List* list);
Expression* RelationshipExpressionSemanticAction(List* list, Relationship* relationship, Hierarchy* hierarchy);
Attributes* AppendAttributesSemanticAction(Attributes* attributes, Attribute* attribute);
Attributes* AttributeSemanticAction(Attribute* attribute);
Attribute* StringMetadataSemanticAction(char* metadataId, char* value);
Attribute* IntegerMetadataSemanticAction(char* metadataId, int value);
Properties* PropertiesSemanticAction(Attributes* attributes);
Hierarchy* HierarchySemanticAction(List* list);
Employees* EmployeesSemanticAction(char* employeeId);
Employees* VariableEmployeesSemanticAction(char* employeeId);
List* ListSemanticAction(Attributes * attributes);
List* VariableListSemanticAction(char* employeesId);
List* ElementsListSemanticAction(Elements* elements);
Elements* AppendElementsSemanticAction(Elements* elements, char* employeeId);
Elements* ElementSemanticAction(char* employeesId);
Relationship* ChildSemanticAction();
Relationship* SiblingSemanticAction();
Relationship* ChildAndSelfSemanticAction();

#endif
