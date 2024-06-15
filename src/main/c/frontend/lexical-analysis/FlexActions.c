#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
    _logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
    _logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char* functionName, LexicalAnalyzerContext* lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char* functionName, LexicalAnalyzerContext* lexicalAnalyzerContext) {
    char* escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
    logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)", functionName, escapedLexeme,
                 lexicalAnalyzerContext->currentContext, lexicalAnalyzerContext->length, lexicalAnalyzerContext->line);
    free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
}

void IgnoredLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
}

void BeginOnelineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
}

void EndOnelineCommentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
}

Token EmployeeLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = EMPLOYEE;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token BracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token bracket) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = bracket;
    return bracket;
}

Token ProjectLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = PROJECT;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token InLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = IN;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token UnderLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = UNDER;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token SearchLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = SEARCH;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token AssignmentLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = ASSIGN;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token RemoveLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = REMOVE;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token FromLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = FROM;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token SquareBracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token squareBracket) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    
    lexicalAnalyzerContext->semanticValue->token = squareBracket;
    return squareBracket;
}

Token ChildLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = CHILD;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token SiblingsLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = SIBLING;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token ChildAndSelfLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = CHILD_AND_SELF;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token ReplaceLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = REPLACE;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token WithLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = WITH;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token parenthesis) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    lexicalAnalyzerContext->semanticValue->token = parenthesis;
    return parenthesis;
}

Token ColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = COLON;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token CommaLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = COMMA;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token SemiColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    Token token = SEMICOLON;
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token StringLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
    return STRING;
}

Token IdentifierLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
    return ID;
}

Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
    return INTEGER;
}

Token UnknownLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return UNKNOWN;
}
