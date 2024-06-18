#include "Generator.h"

#include "../domain-specific/projectADT.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;

typedef struct GeneratorState {
	TEmployee *employees;
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
static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression);
static void _generateProperties(TEmployee employee, Properties *properties);
static void _generateAttributes(TEmployee employee, Attributes *attributes);
static void _generateAttribute(TEmployee employee, Attribute *attribute);

static void _generateEmployeeExpression(EmployeeExpression *employeeExpression);
static void _generateRemoveExpression(RemoveExpression *removeExpression);
static void _generateReplaceExpression(ReplaceExpression *replaceExpression);
static void _generateAssignExpression(AssignExpression *assignExpression);
static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression);
static void _generateListRelationshipExpression(ListRelationshipExpression *listRelationshipExpression);
static void _generateListExpression(ListExpression *listExpression);

static TEmployee newEmployee(char *employeeId, Properties *properties);

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

static void _generateVariableEmployeeExpression(VariableEmployeeExpression *variableEmployeeExpression) {
	newEmployee(variableEmployeeExpression->employeeId, variableEmployeeExpression->properties);
}

static void _generateProperties(TEmployee employee, Properties *properties) {
	_generateAttributes(employee, properties->attributes);
}

static void _generateAttributes(TEmployee employee, Attributes *attributes) {
	for (int i = 0; i < attributes->count; i++) {
		_generateAttribute(employee, attributes->attributes[i]);
	}
}

static void _generateAttribute(TEmployee employee, Attribute *attribute) {
	employee->metadata = (Metadata *) realloc(employee->metadata, employee->metadataCount + 1);
	employee->metadata->tag = attribute->tag;
	switch (attribute->attributeType)
	{
	case ATTRIBUTE_INTEGER:
		employee->metadata->metadataType = METADATA_INTEGER;
		employee->metadata->numValue = attribute->numValue;
		break;
	case ATTRIBUTE_STRING:
		employee->metadata->metadataType = METADATA_STRING;
		employee->metadata->stringValue = attribute->stringValue;
		break;
	}
}

static void _generateEmployeeExpression(EmployeeExpression *employeeExpression) {
	TEmployee employee = getEmployeeFromState(employeeExpression->employeeId);

	if (employee == NULL)
		employee = newEmployee(employeeExpression->employeeId, employeeExpression->properties);
	if (employeeExpression->hierarchy != NULL){
		switch (employeeExpression->hierarchy->list->listType) {
		case LIST_PROPERTIES://que quilombo
			// TEmployee *bosses = searchEmployees(project, employeeExpression->hierarchy->list->properties); // TODO: check if should be deleted
			break;
		case LIST_ELEMENTS: //No estoy seguro si en este me equivoco pero no entiendo la diferencia entre list_elements y list_empolyee REVISAR
			for(int i = 0; i < employeeExpression->hierarchy->list->elements->count; i++) {
				TEmployee employeeAux = getEmployeeFromState(employeeExpression->hierarchy->list->elements->ids[i]);
				if(employeeAux == NULL) {
					//Aca tendria que haber error, entiendo que no lo acepta nuestra gramatica
				}
				else{
					employee->bosses = (TEmployee *) realloc(employee->bosses, employee->bossesCount + 1);
					employee->bosses[employee->bossesCount] = employeeAux;
					employee->bossesCount++;
					employeeAux->children = (TEmployee *) realloc(employeeAux->children, employeeAux->childrenCount + 1);
					employeeAux->children[employeeAux->childrenCount] = employee;
					employeeAux->childrenCount++;
				}
			}
			break;
		case LIST_EMPLOYEE: //Mirar comentarios de arriba
			TEmployee boss = getEmployee(project, employeeExpression->hierarchy->list->employeeId);
			addChild(project, boss, employee);
			break;
		}
	}
}

static void _generateRemoveExpression(RemoveExpression *removeExpression) {
	TEmployee employee = getEmployeeFromState(removeExpression->idToRemove);
	if(employee!=NULL){
		for (size_t i = 0; i < employee->bossesSize; i++)
		{
			//Recorro los jefes y tengo que hacer que dejen de apuntar al empleado

		}
		//Despues hago que el eempleado deje de apuntarlo a los jefes
		//Hago ciclo para que los hijos dejen de apuntar al empleado
		//Hago que el empleado deje de apuntar a los hijos
		
	}

}

static void _generateReplaceExpression(ReplaceExpression *replaceExpression) {
	projectADT project = getProject(replaceExpression->projectId);
	TEmployee old = getEmployee(project, replaceExpression->idToReplace);
	
	TEmployee new;

	//Parecido a delete pero hago que en vez de simplemente dejar de apuntar al empleado, apunte al nuevo y el nuevo apunte a los que apuntaba el empleado

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

static TEmployee newEmployee(char *employeeId, Properties *properties) {
	if(employeeId == NULL) {
		logError(_logger, "No se puede crear un empleado sin id");
		return;
	}else if(getEmployeeFromState(employeeId) != NULL) {
		logError(_logger, "El empleado ya existe");
		return;
	}
	state->employees = (TEmployee *) realloc(state->employees, state->sizeEmployees + 1);

	if (state->employees == NULL) {
		logError(_logger, "No hay mas memoria disponible");
		return;
	}

	state->sizeEmployees++;

	TEmployee employee = (TEmployee) malloc(sizeof(struct Employee));
	employee->employeeId = employeeId;
	employee->metadata = properties->attributes;
	employee->metadataCount = 0;
	employee->children = NULL;
	employee->childrenCount = 0;
	employee->bosses = NULL;
	employee->bossesCount = 0;
	_generateProperties(employee, properties);

	state->employees[state->sizeEmployees - 1] = employee;

	return employee;
}

static TEmployee getEmployeeFromState(char *employeeId) {
	for (int i = 0; i < state->sizeEmployees; i++) {
		if (strcmp(state->employees[i]->employeeId, employeeId) == 0) {
			return state->employees[i];
		}
	}

	return NULL;
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
