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
Expression* ProjectExpressionSemanticAction(char* projectId);
Expression* VariableEmployeeExpressionSemanticAction(char* employeeId, Properties* properties);
Expression* EmployeeExpressionSemanticAction(char* employeeId, char* projectId, Hierarchy* hierarchy,
                                             Properties* properties);
Expression* RemoveExpressionSemanticAction(char* idToRemove, char* projectId);
Expression* ReplaceExpressionSemanticAction(char* idToReplace, char* projectId, Define* define);
Expression* AssignExpressionSemanticAction(Employees* employees, List* list);
Expression* RelationshipExpressionSemanticAction(List* list, Relationship* relationship, char* projectId,
                                                 Hierarchy* hierarchy);
Expression* ListRelationshipExpressionSemanticAction(List* list, Relationship* relationship);
Expression* ListExpressionSemanticAction(List* list);
Attributes* AppendAttributesSemanticAction(Attributes* attributes, Attribute* attribute);
Attributes* AttributeSemanticAction(Attribute* attribute);
Attribute* StringMetadataSemanticAction(char* metadataId, char* value);
Attribute* IntegerMetadataSemanticAction(char* metadataId, int value);
Properties* PropertiesSemanticAction(Attributes* attributes);
Hierarchy* HierarchySemanticAction(List* list);
Define* VariableDefineSemanticAction(char* employeeId);
Define* PropertiesDefineSemanticAction(Properties* properties);
Employees* EmployeesSemanticAction(char* employeeId);
Employees* VariableEmployeesSemanticAction(char* employeeId);
List* ListSemanticAction(Properties* properties, char* projectId);
List* VariableListSemanticAction(char* employeesId);
List* ElementsListSemanticAction(Elements* elements);
Elements* AppendElementsSemanticAction(Elements* elements, char* employeeId);
Elements* ElementSemanticAction(char* employeesId);
Relationship* ChildSemanticAction();
Relationship* SiblingSemanticAction();
Relationship* ChildAndSelfSemanticAction();

#endif
