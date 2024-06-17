#include "Generator.h"

#include "../domain-specific/projectADT.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;

typedef struct GeneratorState {
	projectADT *projects;
	TEmployee *employees;
	size_t sizeProjects;
	size_t sizeEmployees;
} GeneratorState;

static GeneratorState *state;

FILE *file;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateProgram(Program *program);
static void _generateExpressions(Expressions *expressions);
static void _generateExpression(Expression *expression);
static void _generateProjectExpression(ProjectExpression *projectExpression);
static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression);
static void _generateEmployeeExpression(EmployeeExpression *employeeExpression);
static void _generateRemoveExpression(RemoveExpression *removeExpression);
static void _generateReplaceExpression(ReplaceExpression *replaceExpression);
static void _generateAssignExpression(AssignExpression *assignExpression);
static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression);
static void _generateListRelationshipExpression(ListRelationshipExpression *listRelationshipExpression);
static void _generateListExpression(ListExpression *listExpression);

static void _generatePrologue(void);
static void _generateEpilogue();
static char *_indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char *const format, ...);

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program *program) {
	state = (GeneratorState *) calloc(1, sizeof(GeneratorState));

	if (state == NULL) {
		logError(_logger, "No hay mas memoria disponible");
		return;
	}

	_generateExpressions(program->expressions);

	file
}

static void _generateExpressions(Expressions *expressions) {
	for (int i = 0; i < expressions->count; i++) {
		_generateExpression(expressions->expressions[i]);
	}
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(Expression *expression) {
	switch (expression->type) {
		case PROJECT_EXPRESSION:
			_generateProjectExpression(expression->projectExpression);
			break;
		case VARIABLE_EMPLOYEE_EXPRESSION:
			_generateVariableEmployeeExpression(expression->variableEmployeeExpression);
			break;
		case EMPLOYEE_EXPRESSION:
			_generateEmployeeExpression(expression->employeeExpression);
			break;
		case REMOVE_EXPRESSION:
			_generateRemoveExpression(expression->removeExpression);
			break;
		case REPLACE_EXPRESSION:
			_generateReplaceExpression(expression->replaceExpression);
			break;
		case ASSIGN_EXPRESSION:
			_generateAssignExpression(expression->assignExpression);
			break;
		case RELATIONSHIP_EXPRESSION:
			_generateRelationshipExpression(expression->relationshipExpression);
			break;
		case LIST_RELATIONSHIP_EXPRESSION:
			_generateListRelationshipExpression(expression->listRelationshipExpression);
			break;
		case LIST_EXPRESSION:
			_generateListExpression(expression->listExpression);
			break;
		default:
			logError(_logger, "The specified expression type is unknown: %d", expression->type);
			break;
	}
}

static void _generateProjectExpression(ProjectExpression *projectExpression) {
	state->projects = (projectADT *) realloc(state->projects, state->sizeProjects + 1);

	if (state->projects == NULL) {
		logError(_logger, "No hay mas memoria disponible");
		return;
	}

	state->sizeProjects++;

	setName(state->projects[state->sizeProjects - 1], projectExpression->projectId);
}

static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression) {
	state->employees = (TEmployee *) realloc(state->employees, state->sizeEmployees + 1);

	if (state->employees == NULL) {
		logError(_logger, "No hay mas memoria disponible");
		return;
	}

	state->sizeEmployees++;

	variableEmployeeExpression->employeeId;

	// TODO: check what to do with variableEmployeeExpression->employeeId (variable name)
	setProperties(state->employees[state->sizeEmployees - 1], variableEmployeeExpression->properties);
}

static void _generateEmployeeExpression(EmployeeExpression *employeeExpression) {
	TEmployee employee = getEmployee(employeeExpression->employeeId);
	if (employee == NULL)
		employee = newEmployee(employeeExpression->employeeId, employeeExpression->properties);

	projectADT project = getProject(employeeExpression->projectId);

	if (employeeExpression->hierarchy == NULL)
		addEmployee(project, employee);
	else
		addChild(project, employeeExpression->hierarchy, employee);
}

static void _generateRemoveExpression(RemoveExpression *removeExpression) {
	TEmployee employee = getEmployee(removeExpression->idToRemove);

	projectADT project = getProject(removeExpression->projectId);

	removeEmployee(project, employee);
}

static void _generateReplaceExpression(ReplaceExpression *replaceExpression) {
	TEmployee old = getEmployee(replaceExpression->idToReplace);
	projectADT project = getProject(replaceExpression->projectId);
	TEmployee new;

	switch (replaceExpression->define->defineType) {
		case DEFINE_EMPLOYEE:
			new = getEmployee(replaceExpression->define->employeeId);
			break;
		case DEFINE_PROPERTIES:
			new = newEmployee(generateNewEmployeeId(), replaceExpression->define->properties);
			break;
	}
	replaceEmployee(project, old, new);
}

static void _generateAssignExpression(AssignExpression *assignExpression) {
	// TODO: check if will be used
}

static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression) {
	projectADT project = getProject(relationshipExpression->projectId);

	TEmployee * employees;

	switch (relationshipExpression->list->listType) {
		case LIST_PROPERTIES:
			Properties *toSearchProperties = relationshipExpression->list->properties;
			projectADT toSearchProject = getProject(relationshipExpression->list->projectId);
			employees = searchEmployees(toSearchProject, toSearchProperties);
			break;
		case LIST_EMPLOYEE:
			employees = getEmployee(relationshipExpression->list->employeeId);
			break;
		case LIST_ELEMENTS:
			employees = getEmployees(relationshipExpression->list->elements);
			break;
	}

	addHierarchy(project, employees, relationshipExpression->hierarchy, relationshipExpression->relationship);
}

static void _generateListRelationshipExpression(ListRelationshipExpression *listRelationshipExpression) {
	// TODO: check if will be used (there is no project id)
}

static void _generateListExpression(ListExpression *listExpression) {
	// TODO: check if will be used (there is no project id)
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	fprintf(file, "%s", "{ \"nodes\": [");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue() {
	fprintf(file, "%s", "] }\n");
}

/**
 * Generates an indentation string for the specified level.
 */
static char *_indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char *const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char *indentation = _indentation(indentationLevel);
	char *effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState *compilerState) {
	logDebugging(_logger, "Generating final output...");
	file = fopen("project.json", "w");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}
