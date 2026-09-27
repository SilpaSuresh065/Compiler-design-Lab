%{
    #include <stdio.h>
%}

%%
"abc" { printf("ABC"); }
.     { printf("%s", yytext); }
%%

int yywrap(){
    return 1;
}

int main(){
    printf("Etner String:");
    yylex();
    return 0;
}