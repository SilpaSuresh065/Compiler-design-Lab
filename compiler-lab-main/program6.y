%{
#include <stdio.h>
#include <ctype.h>

int yylex(void);
void yyerror(const char *s);
%}

%token LETTER DIGIT

%%
// An identifier starts with a letter, followed by valid tail characters
input: LETTER tail { printf("Valid Identifier\n"); return 0; } ;

tail:  tail LETTER
     | tail DIGIT
     | /* empty */
     ;
%%

int yylex(void) {
    int c = getchar();
    if (c == '\n' || c == EOF) return 0;
    if (isalpha(c)) return LETTER;
    if (isdigit(c)) return DIGIT;
    return c; // Returns invalid characters as-is to trigger yyerror
}

void yyerror(const char *s) {
    printf("Invalid Identifier\n");
}

int main(void) {
    printf("Enter identifier: ");
    yyparse();
    return 0;
} 