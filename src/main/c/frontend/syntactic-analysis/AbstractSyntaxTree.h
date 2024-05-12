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

typedef struct ProjectExpression ProjectExpression;
typedef struct VariableEmployeeExpression VariableEmployeeExpression;
typedef struct EmployeeExpression EmployeeExpression;
typedef struct RemoveExpression RemoveExpression;
typedef struct ReplaceExpression ReplaceExpression;
typedef struct AssignExpression AssignExpression;
typedef struct RelationshipExpression RelationshipExpression;
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
    PROJECT_EXPRESSION,
    VARIABLE_EMPLOYEE_EXPRESSION,
    EMPLOYEE_EXPRESSION,
    REMOVE_EXPRESSION,
    REPLACE_EXPRESSION,
    ASSIGN_EXPRESSION,
    RELATIONSHIP_EXPRESSION
};

struct ProjectExpression {
    char* projectId;
};

struct VariableEmployeeExpression {
    char* employeeId;
    Properties* properties;
};

struct EmployeeExpression {
    char* employeeId;
    char* projectId;
    Hierarchy* hierarchy;
    Properties* properties;
};

struct RemoveExpression {
    char* idToRemove;
    char* projectId;
};

struct ReplaceExpression {
    char* idToReplace;
    char* projectId;
    Define* define;
};

struct AssignExpression {
    Employees* employees;
    List* list;
};

struct RelationshipExpression {
    List* list;
    Relationship* relationship;
    char* projectId;
    Hierarchy* hierarchy;
};

struct Expression {
    ExpressionType type;
    union {
        ProjectExpression* projectExpression;
        VariableEmployeeExpression* variableEmployeeExpression;
        EmployeeExpression* employeeExpression;
        RemoveExpression* removeExpression;
        ReplaceExpression* replaceExpression;
        AssignExpression* assignExpression;
        RelationshipExpression* relationshipExpression;
    };
};

struct Program {
    Expression* expression;
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
        struct {
            Properties* properties;
            char* projectId;
        };
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

void releaseProjectExpression(ProjectExpression* projectExpression);
void releaseVariableEmployeeExpression(VariableEmployeeExpression* variableEmployeeExpression);
void releaseEmployeeExpression(EmployeeExpression* employeeExpression);
void releaseRemoveExpression(RemoveExpression* removeExpression);
void releaseReplaceExpression(ReplaceExpression* replaceExpression);
void releaseAssignExpression(AssignExpression* assignExpression);
void releaseRelationshipExpression(RelationshipExpression* relationshipExpression);
void releaseExpression(Expression* expression);
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
