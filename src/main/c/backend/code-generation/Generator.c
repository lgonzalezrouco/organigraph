#include "Generator.h"

#include <stdbool.h>


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
static void addBoss(TEmployee employee, TEmployee boss);
static void addChild(TEmployee employee, TEmployee child);
static void removeEmployee(TEmployee employee);
static void RemoveBosses(TEmployee employee);
static void RemoveChildren(TEmployee employee);
static void replaceEmployee(TEmployee old,TEmployee new);
static void replaceBosses(TEmployee old,TEmployee new);
static void replaceChildren(TEmployee old,TEmployee new);
static TEmployee getRoot(TEmployee employee);
static TEmployee *searchEmployees(TEmployee root,Properties *properties);
static void searchEmployeesRec(TEmployee employee,Properties* properties,TEmployee* employees);
static bool hasProperties(TEmployee employee,Properties* properties);
static bool hasAttribute(TEmployee employee,Attribute* attribute);
static bool isPresent(TEmployee employee, TEmployee* employees);
static TEmployee* getChildren(TEmployee employee,size_t* size);
static TEmployee* getSelfAndChildren(TEmployee employee,size_t* size);
static TEmployee* getSiblings(TEmployee employee,size_t* size);
static size_t concatenateEmployees(TEmployee* employees1,TEmployee* employees2,size_t size1,size_t size2);
static TEmployee getEmployeeFromState(char *employeeId);
static void _generatePrologue(void);
static void _generateEpilogue();
static char *_indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char *const format, ...);


static TEmployee newEmployee(char *employeeId, Properties *properties);

static void _generatePrologue(void);
static void _generateEpilogue();
static char *_indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char *const format, ...);



static void addBoss(TEmployee employee, TEmployee boss){
	employee->bosses = (TEmployee *) realloc(employee->bosses, employee->bossesSize + 1);
	employee->bosses[employee->bossesCount] = boss;
	employee->bossesCount++;
	employee->bossesSize++;
}

static void addChild(TEmployee employee,TEmployee child){
	employee->children = (TEmployee *) realloc(employee->children, employee->childrenSize + 1);
	employee->children[employee->childrenCount] = child;
	employee->childrenSize++;
	employee->childrenCount++;
}

static void _generateRemoveExpression(RemoveExpression *removeExpression) {
	TEmployee employee = getEmployeeFromState(removeExpression->idToRemove);
	removeEmployee(employee);

}

static void removeEmployee(TEmployee employee){
	if(employee!=NULL){
		RemoveBosses(employee);
		RemoveChildren(employee);
	}
}

static void RemoveBosses(TEmployee employee){
	for (size_t i = 0; i < employee->bossesSize; i++)
		{
			//Recorro los jefes y tengo que hacer que dejen de apuntar al empleado
			TEmployee boss = employee->bosses[i];
			if(boss!=NULL){
				//Hago que el jefe deje de apuntar al empleado
				for (size_t j = 0; j < boss->childrenSize; j++)
				{
					if(boss->children[j] == employee){
						boss->children[j] = NULL;
						boss->childrenSize--;
					}
				}
				//Hago que el empleado deje de apuntar al jefe
				employee->bosses[i] = NULL;
				employee->bossesCount--;
			}

		}
}

static void RemoveChildren(TEmployee employee){
	for (size_t i = 0; i < employee->childrenSize; i++)
		{
			//Recorro los hijos y tengo que hacer que dejen de apuntar al empleado
			TEmployee child = employee->children[i];
			if(child!=NULL){
				//Hago que el hijo deje de apuntar al empleado
				for (size_t j = 0; j < child->bossesSize; j++)
				{
					if(child->bosses[j] == employee){
						child->bosses[j] = NULL;
						child->bossesCount--;
						if(child->bossesCount == 0){ //Osea si este era el unico jefe que tenia hago que se desarme el arbol
							removeEmployee(child);
						}
					}
				}
				//Hago que el empleado deje de apuntar al hijo
				employee->children[i] = NULL;
				employee->childrenCount--;
			}

		}
}

static void replaceEmployee(TEmployee old,TEmployee new){
		if(old!=NULL && new!=NULL){
			replaceBosses(old,new);
			replaceChildren(old,new);
		}

	}

static void replaceBosses(TEmployee old,TEmployee new){
		for (size_t i = 0; i < old->bossesCount; i++)
		{
			//Recorro los jefes y tengo que hacer que dejen de apuntar al empleado
			TEmployee boss = old->bosses[i];
			if(boss!=NULL){
				//Hago que el jefe deje de apuntar al empleado
				for (size_t j = 0; j < boss->childrenSize; j++)
				{
					if(boss->children[j] == old){
						boss->children[j] = new;
					}
				}
				//Hago que el empleado deje de apuntar al jefe
				addBoss(new,boss);
				old->bosses[i] = NULL;
				old->bossesCount--;
			}

		}
	}

static void replaceChildren(TEmployee old,TEmployee new){
		for (size_t i = 0; i < old->childrenSize; i++)
		{
			//Recorro los hijos y tengo que hacer que dejen de apuntar al empleado
			TEmployee child = old->children[i];
			if(child!=NULL){
				//Hago que el hijo deje de apuntar al empleado
				for (size_t j = 0; j < child->bossesSize; j++)
				{
					if(child->bosses[j] == old){
						child->bosses[j] = new;
					}
				}
				//Hago que el empleado deje de apuntar al hijo
				addChild(new,child);
				old->children[i] = NULL;
				old->childrenCount--;
			}

		}
	}


static TEmployee getRoot(TEmployee employee){
	if(employee->bossesCount == 0){
		return employee;
	}
	else{
		return getRoot(employee->bosses[0]);
	}
}
//Quiero buscar empleados que cumplan con las propiedades en su metadata

static TEmployee *searchEmployees(TEmployee root,Properties *properties){
	TEmployee * employees;
	employees = (TEmployee *) malloc(sizeof(TEmployee));
	searchEmployeesRec(root,properties,employees);
	return employees;

}
static void searchEmployeesRec(TEmployee employee,Properties* properties,TEmployee* employees){
	if(isPresent(employee,employees)){
		return;
	}
	if(hasProperties(employee,properties)){
		employees = (TEmployee *) realloc(employees, sizeof(employees) + 1);
		employees[sizeof(employees)] = employee;
	}
	for (size_t i = 0; i < employee->childrenCount; i++)
	{
		searchEmployeesRec(employee->children[i],properties,employees);
	}

}

static bool hasProperties(TEmployee employee,Properties* properties){
	for (size_t i = 0; i < properties->attributes->count; i++)
	{
		if(!hasAttribute(employee,properties->attributes->attributes[i])){
			return false;
		}
	}
	return true;
} 
static bool hasAttribute(TEmployee employee,Attribute* attribute){
	for (size_t i = 0; i < employee->metadataCount; i++)
	{
		if(strcmp(employee->metadata[i].tag,attribute->tag) == 0){
			if(employee->metadata[i].metadataType == METADATA_INTEGER){
				if(employee->metadata[i].numValue == attribute->numValue){
					return true;
				}
			}
			else if(employee->metadata[i].metadataType == METADATA_STRING){
				if(strcmp(employee->metadata[i].stringValue,attribute->stringValue) == 0){
					return true;
				}
			}
		}
	}
	return false;
}

static bool isPresent(TEmployee employee, TEmployee* employees){
	for (int i = 0; i < sizeof(employees); i++) {
		if(employees[i] == employee){
			return true;
		}
	}
	return false;
}

static TEmployee* getChildren(TEmployee employee,size_t* size){
	TEmployee* children = (TEmployee *) malloc(sizeof(employee->children));
	*size=0;
	for (size_t i = 0; i < employee->childrenCount; i++)
	{
		if(employee->children[i] != NULL && !isPresent(employee->children[i],children)){
			children[*size] = employee->children[i];
			*size++;
		}
	}
	return children;
}

static TEmployee* getSelfAndChildren(TEmployee employee,size_t* size){
TEmployee* children = (TEmployee *) malloc(sizeof(employee->children)+sizeof(employee));
	*size=0;
	for (size_t i = 0; i < employee->childrenSize; i++)
	{
		if(employee->children[i] != NULL && !isPresent(employee->children[i],children)){
			children[*size] = employee->children[i];
			*size++;
		}
		
	}
	children[*size] = employee;
	*size++;
	return children;
}

static TEmployee* getSiblings(TEmployee employee,size_t* size){
	TEmployee* siblings;
	*size = 0;
	for(int i=0;i<employee->bossesSize;i++){
		TEmployee boss = employee->bosses[i];
		*size=concatenateEmployees(siblings,getChildren(boss,size),*size,boss->childrenSize);
	}
	return siblings;
}

static size_t concatenateEmployees(TEmployee* employees1,TEmployee* employees2,size_t size1,size_t size2){
	employees1=realloc(employees1,sizeof(employees1)+sizeof(employees2));
	int j=0;
	for (size_t i = 0; i < size2; i++)
	{
		if(employees2[i] != NULL && !isPresent(employees2[i],employees1)){
			employees1[size1+j] = employees2[i];
			j++;
		}
	}
	return size1+j;
}
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
		case LIST_PROPERTIES: //search que trae los jefes
		TEmployee root = getRoot(employee);
		TEmployee* employees=searchEmployees(root,employeeExpression->hierarchy->list->properties);
		for(int i = 0; i < sizeof(employees); i++) {
			addBoss(employee, employees[i]);
			addChild(employees[i],employee);
		}
		break;
		case LIST_ELEMENTS: 
			for(int i = 0; i < employeeExpression->hierarchy->list->elements->count; i++) {
				TEmployee employeeAux = getEmployeeFromState(employeeExpression->hierarchy->list->elements->ids[i]);
				if(employeeAux == NULL) {
					//Aca tendria que haber error, entiendo que no lo acepta nuestra gramatica
				}
				else{
					addBoss(employee, employeeAux);
					addChild(employeeAux,employee);
				}
			}
			break;
		/*
		case LIST_EMPLOYEE: //Revisar si vamos a sacar el assign
			TEmployee boss = getEmployee(project, employeeExpression->hierarchy->list->employeeId);
			addChild(project, boss, employee);
			break;
		*/
		}
	}
}

static void _generateReplaceExpression(ReplaceExpression *replaceExpression) {
	TEmployee old = getEmployeeFromState(replaceExpression->idToReplace);
	
	TEmployee new;


	switch (replaceExpression->define->defineType) {
		case DEFINE_EMPLOYEE:
			new = getEmployee(replaceExpression->define->employeeId);
			break;
		case DEFINE_PROPERTIES:
			new = newEmployee(generateNewEmployeeId(), replaceExpression->define->properties);
			break;
	}
	replaceEmployee(old, new);
}

//SEGURO LO SACAMOS
static void _generateAssignExpression(AssignExpression *assignExpression) {
	// TODO: check if will be used
}

static void _generateRelationshipExpression(RelationshipExpression *relationshipExpression) {

	TEmployee * employees;

	switch (relationshipExpression->list->listType) {
		case LIST_PROPERTIES:
			TEmployee root = getRoot(employees[0]); //tomo el del primero, asumo que todos pertenecen al  mismo grafo
			employees = searchEmployees( root, relationshipExpression->list->properties);
			break;
		case LIST_ELEMENTS:
			for(int i = 0; i < relationshipExpression->hierarchy->list->elements->count; i++) {
				TEmployee employeeAux = getEmployeeFromState(relationshipExpression->list->elements->ids[i]);
				concatenateEmployees(employees,&employeeAux,sizeof(employees),1);	
			}
			break;
	}
	TEmployee root = getRoot(employees[0]); //tomo el del primero, asumo que todos pertenecen al  mismo grafo para ser consistentes con los de arriba
	for(int i = 0; i < sizeof(employees); i++) {
		if(employees[i]!=NULL){
			switch (relationshipExpression->hierarchy->list->listType) {
				case LIST_PROPERTIES:
				TEmployee* bosses=searchEmployees(root,relationshipExpression->hierarchy->list->properties);
				for(int j = 0; j < sizeof(employees); j++) {
					addBoss(employees[i], bosses[j]);
					addChild(bosses[j],employees[i]);
				}
				break;
				case  LIST_ELEMENTS:
					for(int i = 0; i < relationshipExpression->hierarchy->list->elements->count; i++) {
						TEmployee employeeAux = getEmployeeFromState(relationshipExpression->hierarchy->list->elements->ids[i]);
						if(employeeAux != NULL) {
							addBoss(employees[i], employeeAux);
							addChild(employeeAux,employees[i]);
						}
					}
					break;
			}
		}
	}	
}
//Destruye al nodo empleado
static void fireEmployee(TEmployee employee){
	if(employee!=NULL){
	removeEmployee(employee);
		for(int i=0;i<state->sizeEmployees;i++){
			if(state->employees[i] == employee){
				state->employees[i] = NULL;
			}
		}
		free(employee);
	}
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
		return NULL;
	}else if(getEmployeeFromState(employeeId) != NULL) {
		logError(_logger, "El empleado ya existe");
		return NULL;
	}
	state->employees = (TEmployee *) realloc(state->employees, state->sizeEmployees + 1);

	if (state->employees == NULL) {
		logError(_logger, "No hay mas memoria disponible");
		return NULL;
	}

	state->sizeEmployees++;

	TEmployee employee = (TEmployee) malloc(sizeof(struct Employee));
	employee->employeeId = employeeId;
	employee->metadata = NULL;
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
