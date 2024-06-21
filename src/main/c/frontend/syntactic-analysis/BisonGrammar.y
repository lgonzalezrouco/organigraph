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
	
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseExpressions($$); } <expressions>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseProgram($$); } <program>
%destructor { releaseEmployees($$); } <employees>
%destructor { releaseList($$); } <list>
%destructor { releaseElements($$); } <elements>
%destructor { releaseRelationship($$); } <relationship>
%destructor { releaseProperties($$); } <properties>
%destructor { releaseHierarchy($$); } <hierarchy>
%destructor { releaseAttributes($$); } <attributes>
*/

/** Terminals. */

%token <token> EMPLOYEE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> UNDER
%token <token> SEARCH
%token <token> ASSIGN
%token <token> REMOVE
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

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%%

program: expressions												{ $$ = ExpressionsProgramSemanticAction(currentCompilerState(), $1); }
	;
expressions: expressions expression SEMICOLON						{ $$ = AppendExpressionSemanticAction($1, $2); }
	| expression SEMICOLON											{ $$ = ExpressionSemanticAction($1); }
	;

expression: EMPLOYEE ID properties									{ $$ = VariableEmployeeExpressionSemanticAction($2, $3); }	
	| EMPLOYEE ID UNDER list properties								{ $$ = EmployeeExpressionSemanticAction($2, $4, $5); }
	| ID UNDER list properties										{ $$ = EmployeeExpressionSemanticAction($1, $3, $4); }
	| REMOVE ID														{ $$ = RemoveExpressionSemanticAction($2); }
	| REPLACE ID WITH ID properties									{ $$ = ReplaceExpressionSemanticAction($2, $4, $5); }
	| employees ASSIGN list											{ $$ = AssignExpressionSemanticAction($1, $3); }
	| list relationship hierarchy									{ $$ = RelationshipExpressionSemanticAction($1, $2, $3); }
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
	;

employees: EMPLOYEE ID OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET		{ $$ = EmployeesSemanticAction($2); }
	| ID															{ $$ = VariableEmployeesSemanticAction($1); }
	;

list: OPEN_PARENTHESIS SEARCH OPEN_BRACKET attributes CLOSE_BRACKET CLOSE_PARENTHESIS			{ $$ = ListSemanticAction($4); }
	| SEARCH OPEN_BRACKET attributes CLOSE_BRACKET												{ $$ = ListSemanticAction($3); }
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
