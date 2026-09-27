%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
%}
%token NUMBER
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%
lines:
      lines expr '\n' { printf("= %d\n\n", $2); }
    | lines '\n'
    | /* empty */
    ;
expr:
      expr '+' expr        { $$ = $1 + $3; }
    | expr '-' expr        { $$ = $1 - $3; }
    | expr '*' expr        { $$ = $1 * $3; }
    | expr '/' expr        { $$ = ($3 == 0) ? (yyerror("Error: Division by zero!"), 0) : $1 / $3; }
    | '(' expr ')'         { $$ = $2; }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | NUMBER               { $$ = $1; }
    ;
%%
void yyerror(const char *s) { fprintf(stderr, "%s\n", s); }
int main(void) {
    printf("--- YACC Calculator ---\nEnter expressions:\n");
    yyparse();
    return 0;
}