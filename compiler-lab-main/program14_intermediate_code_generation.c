#include <stdio.h>
#include <string.h>
#include <ctype.h>

char expr[100];
char opStack[100];
char valStack[100][20];

int topOp = -1;
int topVal = -1;
int temp = 1;

int precedence(char op)
{
    if (op == '*' || op == '/')
        return 2;

    if (op == '+' || op == '-')
        return 1;

    return 0;
}

void generate(char op)
{
    char a[20], b[20], result[20];

    strcpy(b, valStack[topVal--]);
    strcpy(a, valStack[topVal--]);

    sprintf(result, "t%d", temp++);

    printf("%s = %s %c %s\n",
           result, a, op, b);

    strcpy(valStack[++topVal], result);
}

int main()
{
    int i;
    char c;

    printf("Enter expression: ");
    scanf("%s", expr);

    for (i = 0; expr[i] != '\0'; i++)
    {
        c = expr[i];

        /* Operand */
        if (isalnum(c))
        {
            char operand[2];

            operand[0] = c;
            operand[1] = '\0';

            strcpy(valStack[++topVal], operand);
        }

        /* Opening parenthesis */
        else if (c == '(')
        {
            opStack[++topOp] = c;
        }

        /* Closing parenthesis */
        else if (c == ')')
        {
            while (topOp >= 0 && opStack[topOp] != '(')
                generate(opStack[topOp--]);

            topOp--;
        }

        /* Operator */
        else
        {
            while (topOp >= 0 &&
                   opStack[topOp] != '(' &&
                   precedence(opStack[topOp]) >= precedence(c))
            {
                generate(opStack[topOp--]);
            }

            opStack[++topOp] = c;
        }
    }

    /* Remaining operators */
    while (topOp >= 0)
        generate(opStack[topOp--]);

    printf("Result = %s\n", valStack[topVal]);

    return 0;
}
