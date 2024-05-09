#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

/* Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression); */
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);

Expression* ProjectExpressionSemanticAction(ID * id);
Expression* EmployeeExpressionSemanticAction(ID* id, Project* project, Hierachy* hierachy, Properties* properties);
Expression* RemoveExpressionSemanticAction(ID* id1, ID* id2);
Expression* ReplaceExpressionSemanticAction(ID* id1, ID* id2, Define* define);
Expression* EmployeesAssignExpressionSemanticAction(Employees* employees, List* list);
Expression* ListRelationshipProjectHierachyExpressionSemanticAction(List* list, Relationship* relationship, Project* project, Hierachy* hierachy);
Attributes* AttributesSemanticAction(Attributes* attributes1, Attributes* attributes2);
Attributes* MetadataSemanticAction(Metadata* metadata, STRING* string);
Attributes* MetadataSemanticAction(Metadata* metadata, INTEGER* integer);
Properties* PropertiesSemanticAction(Attributes* attributes);
Hierachy* HierachySemanticAction(List* list);
Project* ProjectSemanticAction(ID* id);
Define* DefineSemanticAction(ID* id);
Define* DefineSemanticAction(Properties* properties);
Employee* EmployeeSemanticAction();
Employees* EmployeesSemanticAction(ID* id);
List* ListSemanticAction(Properties* properties, ID* id);
List* ListSemanticAction(Elements* elements);
Elements* ElementsSemanticAction(Elements* elements1, Elements* elements2);
Elements* ElementsSemanticAction(ID* id);
Relationship* ChildSemanticAction();
Relationship* SiblingSemanticAction();
Relationship* ChildAndSelfSemanticAction();

#endif
