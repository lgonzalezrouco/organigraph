%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

%x ONELINE_COMMENT


/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								        { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				        { EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"//"								        { BEGIN(ONELINE_COMMENT); BeginOnelineCommentLexemeAction(createLexicalAnalyzerContext()); }
<ONELINE_COMMENT>"\n"				        { EndOnelineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<ONELINE_COMMENT>.					        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<ONELINE_COMMENT>[[:space:]]+		        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"employee"                                  { return EmployeeLexemeAction(createLexicalAnalyzerContext()); }
"{"                                         { return BracketLexemeAction(createLexicalAnalyzerContext(),OPEN_BRACKET); }
"}"                                         { return BracketLexemeAction(createLexicalAnalyzerContext(),CLOSE_BRACKET); }
"under"                                     { return UnderLexemeAction(createLexicalAnalyzerContext()); }
"search"                                    { return SearchLexemeAction(createLexicalAnalyzerContext()); }
"="                                         { return AssignmentLexemeAction(createLexicalAnalyzerContext()); }
"remove"                                    { return RemoveLexemeAction(createLexicalAnalyzerContext()); }
"["                                         { return SquareBracketLexemeAction(createLexicalAnalyzerContext(),OPEN_SQUARE_BRACKET); }
"]"                                         { return SquareBracketLexemeAction(createLexicalAnalyzerContext(),CLOSE_SQUARE_BRACKET); }
"->"                                        { return ChildLexemeAction(createLexicalAnalyzerContext()); }
"=="                                        { return SiblingsLexemeAction(createLexicalAnalyzerContext()); }
"=>"                                        { return ChildAndSelfLexemeAction(createLexicalAnalyzerContext()); }
"replace"                                   { return ReplaceLexemeAction(createLexicalAnalyzerContext()); }
"with"                                      { return WithLexemeAction(createLexicalAnalyzerContext()); }
"("									        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(),OPEN_PARENTHESIS); }
")"									        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(),CLOSE_PARENTHESIS); }
":"                                         { return ColonLexemeAction(createLexicalAnalyzerContext()); }
","                                         { return CommaLexemeAction(createLexicalAnalyzerContext()); }
";"                                         { return SemiColonLexemeAction(createLexicalAnalyzerContext());}

"\""[[:alnum:][:space:]]+"\""      { return StringLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						        { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:alnum:]]+                                { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }


[[:space:]]+						        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.									        { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
