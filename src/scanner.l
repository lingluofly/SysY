%{
#include "token.h"
#include <string>
extern int yylex();
extern char* yytext;
extern int yylineno;
%}

%option noyywrap

digit       [0-9]
int_const   {digit}+
float_const {digit}+\.{digit}+([eE][+-]?{digit}+)?
identifier  [_a-zA-Z][_a-zA-Z0-9]*

%%

"int"       { return static_cast<int>(TokenType::INT_TYPE); }
"float"     { return static_cast<int>(TokenType::FLOAT_TYPE); }
"const"     { return static_cast<int>(TokenType::CONST); }
"void"      { return static_cast<int>(TokenType::VOID_TYPE); }
"if"        { return static_cast<int>(TokenType::IF); }
"else"      { return static_cast<int>(TokenType::ELSE); }
"while"     { return static_cast<int>(TokenType::WHILE); }
"break"     { return static_cast<int>(TokenType::BREAK); }
"continue"  { return static_cast<int>(TokenType::CONTINUE); }
"return"    { return static_cast<int>(TokenType::RETURN); }

{int_const}   { yylval.int_val = atoi(yytext); return static_cast<int>(TokenType::INT_CONST); }
{float_const} { yylval.float_val = atof(yytext); return static_cast<int>(TokenType::FLOAT_CONST); }
{identifier}  { yylval.string_val = strdup(yytext); return static_cast<int>(TokenType::IDENT); }

"+"   { return static_cast<int>(TokenType::PLUS); }
"-"   { return static_cast<int>(TokenType::MINUS); }
"*"   { return static_cast<int>(TokenType::MUL); }
"/"   { return static_cast<int>(TokenType::DIV); }
"%"   { return static_cast<int>(TokenType::MOD); }

"=="  { return static_cast<int>(TokenType::EQ); }
"!="  { return static_cast<int>(TokenType::NE); }
"<"   { return static_cast<int>(TokenType::LT); }
">"   { return static_cast<int>(TokenType::GT); }
"<="  { return static_cast<int>(TokenType::LE); }
">="  { return static_cast<int>(TokenType::GE); }

"&&"  { return static_cast<int>(TokenType::AND); }
"||"  { return static_cast<int>(TokenType::OR); }
"!"   { return static_cast<int>(TokenType::NOT); }

"="   { return static_cast<int>(TokenType::ASSIGN); }

"("   { return static_cast<int>(TokenType::LPAREN); }
")"   { return static_cast<int>(TokenType::RPAREN); }
"["   { return static_cast<int>(TokenType::LBRACKET); }
"]"   { return static_cast<int>(TokenType::RBRACKET); }
"{"   { return static_cast<int>(TokenType::LBRACE); }
"}"   { return static_cast<int>(TokenType::RBRACE); }

";"   { return static_cast<int>(TokenType::SEMICOLON); }
","   { return static_cast<int>(TokenType::COMMA); }

[ \t]+  /* 跳过空白 */
\n      { yylineno++; }

"//".*  /* 单行注释 */
"/*"([^*]|[*][^/])*"*/" /* 多行注释 */

.       { /* 错误处理 */ }

%%