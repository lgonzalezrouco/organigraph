#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_EMPLOYEE,
    TOKEN_PROJECT
} TokenType;

typedef struct Symbol {
    char* lexeme;     
    TokenType type;
    int scope;        
    void* value;
} Symbol;

// Tabla de símbolos
typedef struct SymbolTable {
    Symbol* entries;
    size_t capacity;
    size_t size;
} SymbolTable;

SymbolTable* createSymbolTable();
void destroySymbolTable(SymbolTable* table);
void insertSymbol(SymbolTable* table, const char* lexeme, TokenType type, int scope, void* value);
Symbol* getSymbol(SymbolTable* table, const char* lexeme);

#endif