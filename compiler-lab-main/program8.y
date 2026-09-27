%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct node {
    char *data;
    struct node *left, *right;
} Node;
Node *createNode(char *data, Node *left, Node *right);
void postorder(Node *root);
int yylex(void);
void yyerror(const char *s);
%}

%union {
    char *str;
    Node *node;
}

%token <str> NUMBER IDENTIFIER
%type <node> expr
%left '+' '-'
%left '*' '/'

%%
input:
      expr '\n'      {
          printf("\nPostorder Traversal of AST:\n");
          postorder($1);
          printf("\n");
          return 0;
      }
      ;
expr:
      expr '+' expr { $$ = createNode("+", $1, $3); }
    | expr '-' expr { $$ = createNode("-", $1, $3); }
    | expr '*' expr { $$ = createNode("*", $1, $3); }
    | expr '/' expr { $$ = createNode("/", $1, $3); }
    | '(' expr ')'  { $$ = $2; }
    | NUMBER        { $$ = createNode($1, NULL, NULL); }
    | IDENTIFIER    { $$ = createNode($1, NULL, NULL); }
    ;
%%

Node *createNode(char *data, Node *left, Node *right){
    Node *n = malloc(sizeof(Node));
    n->data = strdup(data);
    n->left = left;
    n->right = right;
    return n;
}

void postorder(Node *root){
    if(root){
        postorder(root->left);
        postorder(root->right);
        printf("%s ", root->data);
    }
}

int yylex(void){
    int c;
    while((c = getchar()) == ' ' || c == '\t');
    if(c == '\n' || c == EOF)
        return '\n';
    if(isalnum(c))    {
        char buf[50];
        int i = 0;
        buf[i++] = c;
        while(isalnum(c = getchar()))
            buf[i++] = c;
        ungetc(c, stdin);
        buf[i] = '\0';
        yylval.str = strdup(buf);
        return isdigit(buf[0]) ? NUMBER : IDENTIFIER;
    }
    return c;
}
void yyerror(const char *s){
    printf("Invalid Expression\n");
}
int main(){
    printf("Enter Expression: ");
    yyparse();
    return 0;
}