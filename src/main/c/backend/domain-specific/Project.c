#include "Project.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeProjectModule() {
	_logger = createLogger("Project");

}

void shutdownProjectModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static BinaryOperator _expressionTypeToBinaryOperator(const ExpressionType type);
static ComputationResult _invalidBinaryOperator(const int x, const int y);
static ComputationResult _invalidComputation();

/**
 * Converts an expression type to the proper binary operator. If that's not
 * possible, returns a binary operator that always returns an invalid
 * computation result.
 */
static BinaryOperator _expressionTypeToBinaryOperator(const ExpressionType type) {
	switch (type) {
		case PROJECT_EXPRESSION:
			return projectExpression;
		case VARIABLE_EMPLOYEE_EXPRESSION:
			return variableEmployeeExpression;
		case EMPLOYEE_EXPRESSION:
			return employeeExpression;
		case REMOVE_EXPRESSION:
			return removeExpression;
		case REPLACE_EXPRESSION:
			return replaceExpression;
		case ASSIGN_EXPRESSION:
			return assingExpression;
		case RELATIONSHIP_EXPRESSION:
			return relationshipExpression;
		case LIST_RELATIONSHIP_EXPRESSION:
			return listRelationshipExpression;
		case LIST_EXPRESSION:
			return listExpression;
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return _invalidBinaryOperator;
	}
}

/**
 * A binary operator that always returns an invalid computation result.
 */
static ComputationResult _invalidBinaryOperator(const int x, const int y) {
	return _invalidComputation();
}

/**
 * A computation that always returns an invalid result.
 */
static ComputationResult _invalidComputation() {
	ComputationResult computationResult = {.succeed = false, .value = 0};
	return computationResult;
}

ComputationResult computeExpressions(Expressions *expressions) {
	for (int i = 0; i < expressions->count; i++) {
		if (!(computeExpression(expressions->expressions[i]).succeed))
			return _invalidComputation();
	}
	ComputationResult computationResult = {.succeed = true, .value = 1};
	return computationResult;
}

ComputationResult computeExpression(Expression *expression) {
	switch (expression->type) {
		case PROJECT_EXPRESSION:
			ComputationResult result = computeProjectExpression(expression->projectExpression);

		case VARIABLE_EMPLOYEE_EXPRESSION:
			return computeVariableEmployeeExpression(expression->variableEmployeeExpression);
		case EMPLOYEE_EXPRESSION:
			return computeEmployeeExpression(expression->employeeExpression);
		case REMOVE_EXPRESSION:
			return computeRemoveExpression(expression->removeExpression);
		case REPLACE_EXPRESSION:
			return computeReplaceExpression(expression->replaceExpression);
		case ASSIGN_EXPRESSION:
			return computeAssignExpression(expression->assignExpression);
		case RELATIONSHIP_EXPRESSION:
			return computeRelationshipExpression(expression->relationshipExpression);
		case LIST_RELATIONSHIP_EXPRESSION:
			return computeListRelationshipExpression(expression->listRelationshipExpression);
		case LIST_EXPRESSION:
			return computeListExpression(expression->listExpression);
		default:
			return _invalidComputation();
	}
}

ComputationResult computeProjectExpression(ProjectExpression *expression){
	ComputationResult computationResult = {
		.succeed = true,
		.string = expression->projectId // TODO: check if malloc is necessary
	};
	return computationResult;
}

ComputationResult computeVariableEmployeeExpression(VariableEmployeeExpression *expression){}
ComputationResult computeEmployeeExpression(EmployeeExpression *expression){}
ComputationResult computeRemoveExpression(RemoveExpression *expression){}
ComputationResult computeReplaceExpression(ReplaceExpression *expression){}
ComputationResult computeAssignExpression(AssignExpression *expression){}
ComputationResult computeRelationshipExpression(RelationshipExpression *expression){}
ComputationResult computeListRelationshipExpression(ListRelationshipExpression *expression){}
ComputationResult computeListExpression(ListExpression *expression){}