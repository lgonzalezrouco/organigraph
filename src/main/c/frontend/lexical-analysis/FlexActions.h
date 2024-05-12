#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

void BeginOnelineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndOnelineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token EmployeeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ProjectLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token InLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UnderLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SearchLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token AssignmentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token QuotationMarkLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token RemoveLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token FromLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SquareBracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ChildLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SiblingsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ChildAndSelfLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ReplaceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token WithLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
