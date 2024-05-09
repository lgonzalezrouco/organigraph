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
"{"                                         { return BracketLexemeAction(createLexicalAnalyzerContext()); }
"}"                                         { return BracketLexemeAction(createLexicalAnalyzerContext()); }
"project"                                   { return ProjectLexemeAction(createLexicalAnalyzerContext()); }
"in"                                        { return InLexemeAction(createLexicalAnalyzerContext()); }
"under"                                     { return UnderLexemeAction(createLexicalAnalyzerContext()); }
"search"                                    { return SearchLexemeAction(createLexicalAnalyzerContext()); }
"="                                         { return AssignmentLexemeAction(createLexicalAnalyzerContext()); }
"remove"                                    { return RemoveLexemeAction(createLexicalAnalyzerContext()); }
"from"                                      { return FromLexemeAction(createLexicalAnalyzerContext()); }  
"["                                         { return SquareBracketLexemeAction(createLexicalAnalyzerContext()); }
"]"                                         { return SquareBracketLexemeAction(createLexicalAnalyzerContext()); }
"->"                                        { return ChildLexemeAction(createLexicalAnalyzerContext()); }
"=="                                        { return SiblingsLexemeAction(createLexicalAnalyzerContext()); }
"=>"                                        { return ChildAndSelfLexemeAction(createLexicalAnalyzerContext()); }
"replace"                                   { return ReplaceLexemeAction(createLexicalAnalyzerContext()); }
"with"                                      { return WithLexemeAction(createLexicalAnalyzerContext()); }
"("									        { return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }
")"									        { return ParenthesisLexemeAction(createLexicalAnalyzerContext()); }

[[:alpha:]]+":"                             { return MetadataLexemeAction(createLexicalAnalyzerContext()); }
[[:alphnum]|[:digits:]]+                    { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
"\""[[:alpha:][:space:][:digits:]]+"\""		{ return StringLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						        { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.									        { return UnknownLexemeAction(createLexicalAnalyzerContext()); }


/* "{"[[:space:]]*[[:alpha:]]+":"[[:space:]]*((("\""[[:alpha:][:space:][:digit:]]+"\"")|([:digit:]+))[[:space:]]*","[[:space:]]*)*[[:alpha:]]+":"[[:space:]]*(("\""[[:alpha:][:space:][:digit:]]+"\"")|([:digit:]+))[[:space:]]*"}" { return ObjectLexemeAction(createLexicalAnalyzerContext()); } */

%%

#include "FlexExport.h"
