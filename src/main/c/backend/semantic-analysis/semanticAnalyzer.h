#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "AbstractSyntaxTree.h"
#include "SymbolTable.h"

void printSemanticError(const char* message, int lineNumber);

void checkExpressionType(Expression* expression, SymbolTable* symbolTable);
void checkPropertiesType(Attributes* attributes, SymbolTable* symbolTable);
void checkHierarchyType(Hierarchy* hierarchy, SymbolTable* symbolTable);
void checkDefineType(Define* define, SymbolTable* symbolTable);
void checkListType(List* list, SymbolTable* symbolTable);
void checkRelationshipType(Relationship* relationship);
void checkSemanticAnalysis(Program* program, SymbolTable* symbolTable);

#endif
