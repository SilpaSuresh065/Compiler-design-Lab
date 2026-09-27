%{
    #include <stdio.h>
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INT_NUM     {DIGIT}+
FLOAT_NUM   {DIGIT}+"."{DIGIT}+
WS          [ \t\n]+

%%

"/*"([^*]|\*+[^*/])*\*+"/"    { /* Ignore multi-line comments */ }
"//".*                        { /* Ignore single-line comments */ }
{WS}                          { /* Ignore whitespace, tabs, and newlines */ }

"int"|"float"|"char"|"double"|"if"|"else"|"while"|"for"|"return"|"void"|"break" {
    printf("[KEYWORD]    : %s\n", yytext);
}

{ID}        { printf("[IDENTIFIER] : %s\n", yytext); }
{FLOAT_NUM} { printf("[FLOAT_LIT]  : %s\n", yytext); }
{INT_NUM}   { printf("[INT_LIT]    : %s\n", yytext); }

"=="|"!="|"<="|">="|"&&"|"||"|"++"|"--"|"+"|"-"|"*"|"/"|"%"|"="|"<"|">" {
    printf("[OPERATOR]   : %s\n", yytext);
}

"{"|"}"|"("|")"|"["|"]"|";"|","|"." { printf("[DELIMITER]  : %s\n", yytext); }

.  { printf("[UNKNOWN]    : %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}

int main(void) {
    printf("--- Lexical Analysis Output ---\n");
    yylex();
    return 0;
}
