%{

#include "BisonActions.h"

%}

%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	char* string;
	Token token;

	/** Non-terminals. */
	Expressions * expressions;
	Expression * expression;
	Attributes * attributes;
	Attribute * attribute;
	Program * program;
	Employees * employees;
	List * list;
	Elements * elements;
	Relationship * relationship;
	Properties * properties;
	Hierarchy * hierarchy;
	Define * define;
	
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseExpression($$); } <expression>
%destructor { releaseProgram($$); } <program>
%destructor { releaseEmployees($$); } <employees>
%destructor { releaseList($$); } <list>
%destructor { releaseElements($$); } <elements>
%destructor { releaseRelationship($$); } <relationship>
%destructor { releaseProperties($$); } <properties>
%destructor { releaseHierarchy($$); } <hierarchy>
%destructor { releaseDefine($$); } <define>
%destructor { releaseAttributes($$); } <attributes>
*/

/** Terminals. */

%token <token> EMPLOYEE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> PROJECT
%token <token> IN
%token <token> UNDER
%token <token> SEARCH
%token <token> ASSIGN
%token <token> REMOVE
%token <token> FROM
%token <token> OPEN_SQUARE_BRACKET
%token <token> CLOSE_SQUARE_BRACKET
%token <token> CHILD
%token <token> CHILD_AND_SELF
%token <token> SIBLING
%token <token> REPLACE
%token <token> WITH
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> COLON
%token <token> COMMA
%token <token> SEMICOLON

%token <string> ID

%token <string> STRING

%token <integer> INTEGER

%token <token> UNKNOWN

/** Non-terminals. */
%type <expressions> expressions
%type <expression> expression
%type <attributes> attributes
%type <attribute> attribute
%type <program> program
%type <employees> employees
%type <list> list
%type <elements> elements
%type <relationship> relationship
%type <properties> properties
%type <hierarchy> hierarchy 
%type <define> define

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%%

program: expressions												{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;
expressions: expression SEMICOLON expressions						{ $$ = }
	|expression SEMICOLON
	;

expression: PROJECT ID												{ $$ = ProjectExpressionSemanticAction($2); }
	| EMPLOYEE ID properties										{ $$ = VariableEmployeeExpressionSemanticAction($2, $3); }	
	| EMPLOYEE ID IN ID hierarchy properties						{ $$ = EmployeeExpressionSemanticAction($2, $4, $5, $6); }
	| ID IN ID hierarchy properties									{ $$ = EmployeeExpressionSemanticAction($1, $3, $4, $5); }
	| REMOVE ID FROM ID												{ $$ = RemoveExpressionSemanticAction($2, $4); }
	| REPLACE ID FROM ID WITH define								{ $$ = ReplaceExpressionSemanticAction($2, $4, $6); }
	| employees ASSIGN list											{ $$ = AssignExpressionSemanticAction($1, $3); }
	| list relationship IN ID hierarchy								{ $$ = RelationshipExpressionSemanticAction($1, $2, $4, $5); }
	;

attributes: attributes COMMA attribute								{ $$ = AppendAttributesSemanticAction($1, $3); }
	| attribute														{ $$ = AttributeSemanticAction($1); }
	;

attribute: ID COLON STRING											{ $$ = StringMetadataSemanticAction($1, $3); }
	| ID COLON INTEGER												{ $$ = IntegerMetadataSemanticAction($1, $3); }
	;

properties: OPEN_BRACKET attributes CLOSE_BRACKET					{ $$ = PropertiesSemanticAction($2); }
	|																{ $$ = PropertiesSemanticAction(NULL); }
	;

hierarchy: UNDER list												{ $$ = HierarchySemanticAction($2); }
	| 																{ $$ = HierarchySemanticAction(NULL); }
	;

define: ID															{ $$ = VariableDefineSemanticAction($1); }
	| properties													{ $$ = PropertiesDefineSemanticAction($1); }
	;

employees: EMPLOYEE ID OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET		{ $$ = EmployeesSemanticAction($2); }
	| ID															{ $$ = VariableEmployeesSemanticAction($1); }
	;

list: OPEN_PARENTHESIS SEARCH properties IN ID CLOSE_PARENTHESIS	{ $$ = ListSemanticAction($3, $5); }
	| SEARCH properties IN ID										{ $$ = ListSemanticAction($2, $4); }
	| ID															{ $$ = VariableListSemanticAction($1); }
	| OPEN_BRACKET elements CLOSE_BRACKET							{ $$ = ElementsListSemanticAction($2); }
	;

elements: elements COMMA ID											{ $$ = AppendElementsSemanticAction($1, $3); }
	| ID															{ $$ = ElementSemanticAction($1); }
	;

relationship: CHILD													{ $$ = ChildSemanticAction(); }
	| SIBLING														{ $$ = SiblingSemanticAction(); }
	| CHILD_AND_SELF												{ $$ = ChildAndSelfSemanticAction(); }
	;
%%
