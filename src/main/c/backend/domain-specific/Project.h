#ifndef COMPILER_PROJECT_H
#define COMPILER_PROJECT_H

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"

#include <limits.h>

/** Initialize module's internal state. */
void initializeProjectModule();

/** Shutdown module's internal state. */
void shutdownProjectModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */
typedef struct {
	boolean succeed;
	union {
		int value;
		char * string;
	};
} ComputationResult;

typedef ComputationResult (*BinaryOperator)(const int, const int);

ComputationResult computeExpressions(Expressions *expressions);
ComputationResult computeExpression(Expression *expression);
ComputationResult computeProjectExpression(ProjectExpression *expression);
ComputationResult computeVariableEmployeeExpression(VariableEmployeeExpression *expression);
ComputationResult computeEmployeeExpression(EmployeeExpression *expression);
ComputationResult computeRemoveExpression(RemoveExpression *expression);
ComputationResult computeReplaceExpression(ReplaceExpression *expression);
ComputationResult computeAssignExpression(AssignExpression *expression);
ComputationResult computeRelationshipExpression(RelationshipExpression *expression);
ComputationResult computeListRelationshipExpression(ListRelationshipExpression *expression);
ComputationResult computeListExpression(ListExpression *expression);

#endif
