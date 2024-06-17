#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include <stdlib.h>

#include "../../shared/Logger.h"

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum RelationshipType RelationshipType;
typedef enum ListType ListType;
typedef enum DefineType DefineType;
typedef enum AttributeType AttributeType;
typedef enum ExpressionType ExpressionType;

typedef struct VariableEmployeeExpression VariableEmployeeExpression;
typedef struct EmployeeExpression EmployeeExpression;
typedef struct RemoveExpression RemoveExpression;
typedef struct ReplaceExpression ReplaceExpression;
typedef struct AssignExpression AssignExpression;
typedef struct RelationshipExpression RelationshipExpression;
typedef struct ListRelationshipExpression ListRelationshipExpression;
typedef struct ListExpression ListExpression;
typedef struct Expressions Expressions;
typedef struct Expression Expression;
typedef struct Program Program;
typedef struct Attribute Attribute;
typedef struct Attributes Attributes;
typedef struct Properties Properties;
typedef struct Hierarchy Hierarchy;
typedef struct Define Define;
typedef struct Employees Employees;
typedef struct List List;
typedef struct Elements Elements;
typedef struct Relationship Relationship;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum RelationshipType { RELATIONSHIP_CHILD, RELATIONSHIP_SIBLING, RELATIONSHIP_CHILD_AND_SELF };

enum ListType { LIST_PROPERTIES, LIST_EMPLOYEE, LIST_ELEMENTS };

enum DefineType { DEFINE_EMPLOYEE, DEFINE_PROPERTIES };

enum AttributeType { ATTRIBUTE_STRING, ATTRIBUTE_INTEGER };

enum ExpressionType {
    VARIABLE_EMPLOYEE_EXPRESSION,
    EMPLOYEE_EXPRESSION,
    REMOVE_EXPRESSION,
    REPLACE_EXPRESSION,
    ASSIGN_EXPRESSION,
    RELATIONSHIP_EXPRESSION,
    LIST_RELATIONSHIP_EXPRESSION,
    LIST_EXPRESSION
};



struct VariableEmployeeExpression {
    char* employeeId;
    Properties* properties;
};

struct EmployeeExpression {
    char* employeeId;
    Hierarchy* hierarchy;
    Properties* properties;
};

struct RemoveExpression {
    char* idToRemove;
};

struct ReplaceExpression {
    char* idToReplace;
    Define* define;
};

struct AssignExpression {
    Employees* employees;
    List* list;
};

struct RelationshipExpression {
    List* list;
    Relationship* relationship;
    Hierarchy* hierarchy;
};

struct ListRelationshipExpression {
    List* list;
    Relationship* relationship;
};

struct ListExpression {
    List* list;
};

struct Expression {
    ExpressionType type;
    union {
        VariableEmployeeExpression* variableEmployeeExpression;
        EmployeeExpression* employeeExpression;
        RemoveExpression* removeExpression;
        ReplaceExpression* replaceExpression;
        AssignExpression* assignExpression;
        RelationshipExpression* relationshipExpression;
        ListRelationshipExpression* listRelationshipExpression;
        ListExpression* listExpression;
    };
};

struct Program {
    Expressions* expressions;
};

struct Expressions {
    Expression** expressions;
    int count;
};

struct Attribute {
    char* tag;
    union {
        char* stringValue;
        int numValue;
    };
    AttributeType attributeType;
};

struct Attributes {
    Attribute** attributes;
    int count;
};

struct Properties {
    Attributes* attributes;
};

struct Hierarchy {
    List* list;
};

struct Define {
    union {
        char* employeeId;
        Properties* properties;
    };
    DefineType defineType;
};

struct Employees {
    char* employeesId;
};

struct List {
    union {
        Properties* properties;
        char* employeeId;
        Elements* elements;
    };
    ListType listType;
};

struct Elements {
    char** ids;
    int count;
};

struct Relationship {
    RelationshipType relationshipType;
};

/**
 * Node recursive destructors.
 */

void releaseVariableEmployeeExpression(VariableEmployeeExpression* variableEmployeeExpression);
void releaseEmployeeExpression(EmployeeExpression* employeeExpression);
void releaseRemoveExpression(RemoveExpression* removeExpression);
void releaseReplaceExpression(ReplaceExpression* replaceExpression);
void releaseAssignExpression(AssignExpression* assignExpression);
void releaseRelationshipExpression(RelationshipExpression* relationshipExpression);
void releaseListRelationshipExpression(ListRelationshipExpression* listRelationshipExpression);
void releaseListExpression(ListExpression* listExpression);
void releaseExpression(Expression* expression);
void releaseExpressions(Expressions* expressions);
void releaseProgram(Program* program);
void releaseAttribute(Attribute* attribute);
void releaseAttributes(Attributes* attributes);
void releaseProperties(Properties* properties);
void releaseHierarchy(Hierarchy* hierarchy);
void releaseDefine(Define* define);
void releaseEmployees(Employees* employees);
void releaseList(List* list);
void releaseElements(Elements* elements);
void releaseRelationship(Relationship* relationship);

#endif
