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

	/* Constant * constant; */
	Expression * expression;
	Attributes * attributes;
	/* Factor * factor; */
	Program * program;
	Employee * employee;
	Employees * employees;
	List * list;
	Elements * elements;
	Relationship * relationship;
	Properties * properties;
	Hierachy * hierachy;
	Project * project;
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
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
/* %token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB */

%token <token> EMPLOYEE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> PROJECT
%token <token> IN
%token <token> UNDER
%token <token> SEARCH
%token <token> ASSIGN
%token <string> STRING
%token <token> REMOVE
%token <token> FROM
%token <token> OPEN_SQUARE_BRACKET
%token <token> CLOSE_SQUARE_BRACKET
%token <token> CHILD
%token <token> CHILDANDSELF
%token <token> SIBLING
%token <token> REPLACE
%token <token> WITH
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> METADATA
%token <string> ID

%token <integer> INTEGER

%token <token> UNKNOWN

/** Non-terminals. */
/* %type <constant> constant */
/* %type <factor> factor */
%type <expression> expression
%type <attributes> attributes
%type <program> program
%type <employee> employee
%type <employees> employees
%type <list> list
%type <elements> elements
%type <relationship> relationship
%type <properties> properties
%type <hierachy> hierachy 
%type <project> project
%type <define> define

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
/* %left ADD SUB
%left MUL DIV */

%%

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: PROJECT ID												{ $$ = ProjectExpressionSemanticAction($2); }
	| employee ID project hierachy properties						{ $$ = EmployeeExpressionSemanticAction($2, $3, $4, $5); }
	| REMOVE ID FROM ID												{ $$ = RemoveExpressionSemanticAction($2, $4); }
	| REPLACE ID FROM ID WITH define								{ $$ = ReplaceExpressionSemanticAction($2, $4, $6); }
	| employees ASSIGN list											{ $$ = EmployeesAssignExpressionSemanticAction($1, $3); }
	| list relationship project hierachy							{ $$ = ListRelationshipProjectHierachyExpressionSemanticAction($1, $2, $3, $4); }
	;
/* 	| expression[left] ADD expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); } */

attributes: attributes attributes									{ $$ = AttributesSemanticAction($1, $2); }
	| METADATA STRING												{ $$ = MetadataSemanticAction($1, $2); }
	| METADATA INTEGER												{ $$ = MetadataSemanticAction($1, $2); }
	;

properties: OPEN_BRACKET attributes CLOSE_BRACKET					{ $$ = PropertiesSemanticAction($2); }
	|																{ $$ = PropertiesSemanticAction(NULL); }
	;

hierachy: UNDER list												{ $$ = HierachySemanticAction($2); }
	| 																{ $$ = HierachySemanticAction(NULL); }
	;

project: IN ID														{ $$ = ProjectSemanticAction($2); }
	|																{ $$ = ProjectSemanticAction(NULL); }
	;

define: ID															{ $$ = DefineSemanticAction($1); }
	| properties													{ $$ = DefineSemanticAction($1); }
	;

employee: EMPLOYEE													{ $$ = EmployeeSemanticAction(); }
	|																{ $$ = EmployeeSemanticAction(); }				
	;

employees: EMPLOYEE ID OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET		{ $$ = EmployeesSemanticAction($2); }
	| ID															{ $$ = EmployeesSemanticAction($1); }
	;

list: OPEN_PARENTHESIS SEARCH properties IN ID CLOSE_PARENTHESIS	{ $$ = ListSemanticAction($3, $5); }
	| ID															{ $$ = ListSemanticAction(NULL, $1); }
	| OPEN_BRACKET elements CLOSE_BRACKET							{ $$ = ListSemanticAction($2, NULL); }
	;

elements: elements elements											{ $$ = ElementsSemanticAction($1, $2); }
	| ID															{ $$ = ElementsSemanticAction(NULL, $1); }
	| 																{ $$ = ElementsSemanticAction(NULL, NULL); }
	;

relationship: CHILD													{ $$ = ChildSemanticAction(); }
	| SIBLING														{ $$ = SiblingSemanticAction(); }
	| CHILDANDSELF													{ $$ = ChildAndSelfSemanticAction(); }
	;

/* factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS			{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	; */
%%
