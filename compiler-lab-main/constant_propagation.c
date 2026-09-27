
#include <stdio.h>
#include <string.h>

#define MAX 10
#define SIZE 5

typedef struct {
    char op, op1[SIZE], op2[SIZE], res[SIZE];
} Quad;

Quad q[MAX];
int n;

char *getConst(char *x)
{
    int i;

    for (i = 0; i < n; i++)
        if (q[i].op == '=' && strcmp(q[i].res, x) == 0)
            return q[i].op1;

    return x;
}

void propagate()
{
    int i;
    char a[SIZE], b[SIZE];

    for (i = 0; i < n; i++)
    {
        strcpy(a, getConst(q[i].op1));
        strcpy(b, getConst(q[i].op2));

        strcpy(q[i].op1, a);
        strcpy(q[i].op2, b);
    }
}

int main()
{
    int i;

    printf("Enter the number of 3-address instructions: ");
    scanf("%d", &n);

    printf("\nEnter each instruction\n");

    for (i = 0; i < n; i++)
    {
        printf("Instruction %d: ", i + 1);

        scanf(" %c %s %s %s",
              &q[i].op,
              q[i].op1,
              q[i].op2,
              q[i].res);
    }

    propagate();

    printf("\nThe code after constant propagation is:\n\n");

    printf("op\toperand1\toperand2\tresult\n");
    printf("-----------------------------------------------\n");

    for (i = 0; i < n; i++)
        printf("%c\t%s\t\t%s\t\t%s\n",
               q[i].op,
               q[i].op1,
               q[i].op2,
               q[i].res);

    return 0;
}

