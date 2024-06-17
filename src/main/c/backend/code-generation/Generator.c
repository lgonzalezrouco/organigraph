#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

FILE * file;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateProgram(Program * program);
static void _generateExpressions(const unsigned int indentationLevel, Expressions * expressions);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateProjectExpression(const unsigned int indentationLevel, ProjectExpression * projectExpression);
static void _generateVariableEmployeeExpression(const unsigned int indentationLevel, VariableEmployeeExpression * variableEmployeeExpression);
static void _generateEmployeeExpression(const unsigned int indentationLevel, EmployeeExpression * employeeExpression);
static void _generateRemoveExpression(const unsigned int indentationLevel, RemoveExpression * removeExpression);
static void _generateReplaceExpression(const unsigned int indentationLevel, ReplaceExpression * replaceExpression);
static void _generateAssignExpression(const unsigned int indentationLevel, AssignExpression * assignExpression);
static void _generateRelationshipExpression(const unsigned int indentationLevel, RelationshipExpression * relationshipExpression);
static void _generateListRelationshipExpression(const unsigned int indentationLevel, ListRelationshipExpression * listRelationshipExpression);
static void _generateListExpression(const unsigned int indentationLevel, ListExpression * listExpression);

static void _generatePrologue(void);
static void _generateEpilogue();
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
    _generateExpressions(3, program->expressions);
}

static void _generateExpressions(const unsigned int indentationLevel, Expressions * expressions) {
    for(int i = 0; i < expressions->count; i++) {
        _generateExpression(indentationLevel + 1, expressions->expressions[i]);
    }
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
    switch (expression->type) {
        case PROJECT_EXPRESSION:
            _generateProjectExpression(indentationLevel + 1, expression->projectExpression);
            break;
        case VARIABLE_EMPLOYEE_EXPRESSION:
            _generateVariableEmployeeExpression(indentationLevel + 1, expression->variableEmployeeExpression);
            break;
        case EMPLOYEE_EXPRESSION:
            _generateEmployeeExpression(indentationLevel + 1, expression->employeeExpression);
            break;
        case REMOVE_EXPRESSION:
            _generateRemoveExpression(indentationLevel + 1, expression->removeExpression);
            break;
        case REPLACE_EXPRESSION:
            _generateReplaceExpression(indentationLevel + 1, expression->replaceExpression);
            break;
        case ASSIGN_EXPRESSION:
            _generateAssignExpression(indentationLevel + 1, expression->assignExpression);
            break;
        case RELATIONSHIP_EXPRESSION:
            _generateRelationshipExpression(indentationLevel + 1, expression->relationshipExpression);
            break;
        case LIST_RELATIONSHIP_EXPRESSION:
            _generateListRelationshipExpression(indentationLevel + 1, expression->listRelationshipExpression);
            break;
        case LIST_EXPRESSION:
            _generateListExpression(indentationLevel + 1, expression->listExpression);
            break;
        default:
            logError(_logger, "The specified expression type is unknown: %d", expression->type);
            break;
    }
}

static void _generateProjectExpression(const unsigned int indentationLevel, ProjectExpression * projectExpression) {}

static void _generateVariableEmployeeExpression(const unsigned int indentationLevel, VariableEmployeeExpression * variableEmployeeExpression) {}

static void _generateEmployeeExpression(const unsigned int indentationLevel, EmployeeExpression * employeeExpression) {}

static void _generateRemoveExpression(const unsigned int indentationLevel, RemoveExpression * removeExpression) {}

static void _generateReplaceExpression(const unsigned int indentationLevel, ReplaceExpression * replaceExpression) {}

static void _generateAssignExpression(const unsigned int indentationLevel, AssignExpression * assignExpression) {}

static void _generateRelationshipExpression(const unsigned int indentationLevel, RelationshipExpression * relationshipExpression) {}

static void _generateListRelationshipExpression(const unsigned int indentationLevel, ListRelationshipExpression * listRelationshipExpression) {}

static void _generateListExpression(const unsigned int indentationLevel, ListExpression * listExpression) {}

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
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
    file = fopen("project.json", "w");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}
