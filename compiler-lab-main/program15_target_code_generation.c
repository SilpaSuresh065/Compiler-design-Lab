#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

char tac[MAX][100];
char variables[MAX][20];
int varCount = 0;

/* Check whether a string is already stored */
int exists(char *name)
{
    int i;

    for (i = 0; i < varCount; i++)
        if (strcmp(variables[i], name) == 0)
            return 1;

    return 0;
}

/* Add variable to symbol table */
void addVariable(char *name)
{
    if (name[0] == '\0')
        return;

    if (isdigit(name[0]))
        return;

    if (strcmp(name, "IF") == 0 ||
        strcmp(name, "GOTO") == 0)
        return;

    if (!exists(name)) {
        strcpy(variables[varCount], name);
        varCount++;
    }
}

/* Remove spaces */
void removeSpaces(char *src, char *dest)
{
    int i, j = 0;

    for (i = 0; src[i] != '\0'; i++) {
        if (src[i] != ' ')
            dest[j++] = src[i];
    }

    dest[j] = '\0';
}

/* Find variables from TAC */
void collectVariables(char *line)
{
    char s[100];
    char lhs[20], a[20], b[20];
    char op;

    removeSpaces(line, s);

    /* Arithmetic expression */
    if (sscanf(s, "%19[^=]=%19[^+*/-]%c%19s",
               lhs, a, &op, b) == 4) {

        addVariable(lhs);
        addVariable(a);
        addVariable(b);
        return;
    }

    /* Assignment */
    if (sscanf(s, "%19[^=]=%19s", lhs, a) == 2) {
        addVariable(lhs);
        addVariable(a);
    }
}

/* Generate assembly */
void generateAssembly(char *line)
{
    char s[100];
    char lhs[20], a[20], b[20];
    char op;

    removeSpaces(line, s);

    /* Label */
    if (s[strlen(s) - 1] == ':') {
        printf("%s\n", s);
        return;
    }

    /* GOTO */
    if (sscanf(s, "GOTO%19s", a) == 1) {
        printf("JMP %s\n", a);
        return;
    }

    /*
       IF condition
       Example:
       IFa<bGOTOL1
    */
    if (strncmp(s, "IF", 2) == 0) {

        char left[20], right[20], label[20], rel[3];

        if (sscanf(s, "IF%19[^<>=]%2[<>=]%19[^G]GOTO%19s", left, rel, right, label) == 4) {

            printf("MOV AX, %s\n", left);
            printf("CMP AX, %s\n", right);

            if (strcmp(rel, "<") == 0)
                printf("JL %s\n", label);

            else if (strcmp(rel, ">") == 0)
                printf("JG %s\n", label);

            else if (strcmp(rel, "==") == 0)
                printf("JE %s\n", label);

            return;
        }
    }

    /* Arithmetic expression */
    if (sscanf(s, "%19[^=]=%19[^+*/-]%c%19s",
               lhs, a, &op, b) == 4) {

        printf("MOV AX, %s\n", a);

        switch (op) {

            case '+':
                printf("ADD AX, %s\n", b);
                break;

            case '-':
                printf("SUB AX, %s\n", b);
                break;

            case '*':
                printf("MOV BX, %s\n", b);
                printf("MUL BX\n");
                break;

            case '/':
                printf("MOV BX, %s\n", b);
                printf("XOR DX, DX\n");
                printf("DIV BX\n");
                break;
        }

        printf("MOV %s, AX\n", lhs);
        return;
    }

    /* Simple assignment */
    if (sscanf(s, "%19[^=]=%19s", lhs, a) == 2) {

        printf("MOV AX, %s\n", a);
        printf("MOV %s, AX\n", lhs);

        return;
    }

    printf("; Invalid TAC: %s\n", line);
}

int main()
{
    int n, i;

    printf("Enter number of three address code statements: ");
    scanf("%d", &n);
    getchar();

    printf("\nEnter three address code:\n");

    for (i = 0; i < n; i++) {
        printf("%d: ", i + 1);

        fgets(tac[i], sizeof(tac[i]), stdin);

        tac[i][strcspn(tac[i], "\n")] = '\0';

        collectVariables(tac[i]);
    }

    /* Print final assembly */
    printf("\n\n===== 8086 ASSEMBLY CODE =====\n\n");

    printf(".MODEL SMALL\n");
    printf(".STACK 100H\n\n");

    printf(".DATA\n");

    for (i = 0; i < varCount; i++)
        printf("%s DW ?\n", variables[i]);

    printf("\n.CODE\n");
    printf("MAIN PROC\n");

    printf("MOV AX, @DATA\n");
    printf("MOV DS, AX\n\n");

    for (i = 0; i < n; i++) {
        generateAssembly(tac[i]);
        printf("\n");
    }

    printf("MOV AH, 4CH\n");
    printf("INT 21H\n");

    printf("MAIN ENDP\n");
    printf("END MAIN\n");

    return 0;
}
