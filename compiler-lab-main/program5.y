%{
#include <stdio.h>
#include <ctype.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER IDENTIFIER
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%
input: expr { printf("Valid Expression\n"); return 0; } ;

expr:  expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | '(' expr ')'
     | '-' expr %prec UMINUS
     | NUMBER
     | IDENTIFIER
     ;
%%

int yylex(void) {
    int c;
    while ((c = getchar()) == ' ' || c == '\t'); // Skip whitespace
    if (c == '\n' || c == EOF) return 0;
    if (isdigit(c)) return NUMBER;
    if (isalpha(c)) return IDENTIFIER;
    return c; // Return single-character operators directly
}

void yyerror(const char *s) { printf("Invalid Expression\n"); }

int main(void) {
    printf("Enter expression: ");
    yyparse();
    return 0;
} 