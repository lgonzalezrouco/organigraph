#include "SymbolTable.h"

#define INITIAL_CAPACITY 100

SymbolTable* createSymbolTable() {
    SymbolTable* table = (SymbolTable*) malloc(sizeof(SymbolTable));
    if (table == NULL)
        return NULL;

    table->entries = (Symbol*) malloc(INITIAL_CAPACITY * sizeof(Symbol));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = INITIAL_CAPACITY;
    table->size = 0;

    return table;
}

void destroySymbolTable(SymbolTable* table) {
    if (table == NULL) return;

    for (size_t i = 0; i < table->size; ++i) {
        free(table->entries[i].lexeme);
        if (table->entries[i].value != NULL) {
            free(table->entries[i].value);
        }
    }
    free(table->entries);
    free(table);
}

void insertSymbol(SymbolTable* table, const char* lexeme, TokenType type, int scope, void* value) {
    if (table == NULL || lexeme == NULL) 
        return;

    Symbol* existingEntry = lookupSymbol(table, lexeme);
    if (existingEntry != NULL) {
        return;
    }

    if (table->size >= table->capacity) {
        table->capacity = 2;
        table->entries = (Symbol*) realloc(table->entries, table->capacity * sizeof(Symbol));
        if (table->entries == NULL) {
            return;
        }
    }

    Symbol newEntry;
    newEntry.lexeme = strdup(lexeme);
    newEntry.type = type;
    newEntry.scope = scope;
    newEntry.value = value;

    table->entries[table->size++] = newEntry;
}

Symbol* lookupSymbol(SymbolTable* table, const char* lexeme) {
    if (table == NULL || lexeme == NULL) return NULL;
     for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->entries[i].lexeme, lexeme) == 0) {
            return &table->entries[i];
        }
    }

    return NULL;
}

const char* tokenToString(TokenType type) {
    switch (type) {
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_EMPLOYEE: return "EMPLOYEE";
        case TOKEN_PROJECT: return "PROJECT";
        default: return "UNKNOWN";
    }
}