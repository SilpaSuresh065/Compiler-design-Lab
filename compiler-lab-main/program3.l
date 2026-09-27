%{
    #include <stdio.h>
    int vowel_count = 0;
    int consonant_count = 0;
%}
%%
[aeiouAEIOU] { vowel_count++; }
[a-zA-Z]     { consonant_count++; }
\n           { printf("Vowel Count: %d\n", vowel_count);
              printf("Consonant Count: %d\n", consonant_count);
              return 0;
             }
.            ;
%%

void main(){
    printf("Enter String: ");
    yylex();
}

int yywrap(){
    return 1;
}