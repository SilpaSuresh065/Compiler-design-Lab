%{
    #include <stdio.h>
    int ctChar=0;
    int ctSpace=0;
    int ctWord=0;
    int ctLine=0;
%}
WORD [a-zA-Z0-9]+
EOL [\n]
BLANK [ ]
%%

{WORD} {ctWord++; ctChar+=yyleng;}
{BLANK} {ctSpace++;}
{EOL} {ctLine++;}
. { }
%%

void main(int argc, char *argv[]){
    if(argc!=2){
        printf("Usage:\n\t./a.out <FILENAME>\n");
        exit(0);
    }

    yyin=fopen(argv[1],"r");
    yylex();

    printf("No. of Lines: %d\n",ctLine);
    printf("No. of Words: %d\n",ctWord);
    printf("No. of Characters: %d\n",ctChar);
    fclose(yyin);

}

int yywrap(){
    return 1;
}