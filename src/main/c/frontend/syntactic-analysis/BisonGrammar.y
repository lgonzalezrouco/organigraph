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

	Constant * constant;
	Expression * expression;
	Attributes * attributes;
	Factor * factor;
	Program * program;
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
%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

%token <token> EMPLOYEE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> PROJECT
%token <token> IN
%token <token> UNDER
%token <token> SEARCH
%token <token> ASSIGN
%token <token> QUOTE
%token <token> REMOVE
%token <token> FROM
%token <token> OPEN_SQUARE_BRACKET
%token <token> CLOSE_SQUARE_BRACKET
%token <token> COMMA
%token <token> CHILD
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
%type <constant> constant
%type <expression> expression
%type <attributes> attributes
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	| PROJECT ID
	| employee ID project bosses properties
	| REMOVE ID FROM ID	
	| REPLACE ID FROM ID WITH ID
	| EMPLOYEE ID OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET ASSIGN SEARCH properties IN ID	
	| EMPLOYEE ID OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET ASSIGN SEARCH properties IN ID	
	;

attributes: attributes attributes
	| METADATA QUOTE ID QUOTE
	| METADATA INTEGER
	;

properties: OPEN_BRACKET attributes CLOSE_BRACKET
	|
	;

bosses: UNDER bosses bosses
	| ID
	|
	;

project: IN ID
	|
	;

employee: EMPLOYEE
	|
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
