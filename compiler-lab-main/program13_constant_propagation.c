#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 50

typedef struct {
    char name;
    int value;
    int constant;
} Variable;

Variable table[MAX];
int count = 0;

int find(char name) {
    int i;
    for (i = 0; i < count; i++)
        if (table[i].name == name)
            return i;
    return -1;
}

void setVariable(char name, int value, int constant) {
    int p = find(name);
    if (p == -1) {
        table[count].name = name;
        table[count].value = value;
        table[count].constant = constant;
        count++;
    }
    else {
        table[p].value = value;
        table[p].constant = constant;
    }
}

int getValue(char name, int *value) {
    int p = find(name);
    if (p != -1 && table[p].constant) {
        *value = table[p].value;
        return 1;
    }
    return 0;
}

int main() {
    int n, i;
    char line[100];

    printf("Enter number of statements: ");
    scanf("%d", &n);
    getchar();

    printf("Enter statements:\n");

    for (i = 0; i < n; i++) {
        char clean[100];
        char lhs, x, y, op;
        int a, b, result;
        int j, k = 0;

        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';

        /* Remove spaces */
        for (j = 0; line[j] != '\0'; j++) {
            if (line[j] != ' ')
                clean[k++] = line[j];
        }
        clean[k] = '\0';

        /* Constant assignment: a=10 */
        if (sscanf(clean, "%c=%d", &lhs, &result) == 2) {
            setVariable(lhs, result, 1);
        }

        /* Expression: c=b+5, d=c*2, etc. */
        else if (sscanf(clean, "%c=%c%c%c", &lhs, &x, &op, &y) == 4) {
            int ok1, ok2;
            if (isdigit(x)) {
                a = x - '0';
                ok1 = 1;
            }
            else
                ok1 = getValue(x, &a);

            if (isdigit(y)) {
                b = y - '0';
                ok2 = 1;
            }
            else
                ok2 = getValue(y, &b);
            if (ok1 && ok2) {
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/':
                        if (b == 0) {
                            ok1 = ok2 = 0;
                            break;
                        }
                        result = a / b;
                        break;
                    default:
                        ok1 = ok2 = 0;
                }

                if (ok1 && ok2)
                    setVariable(lhs, result, 1);
                else
                    setVariable(lhs, 0, 0);
            }
            else
                setVariable(lhs, 0, 0);
        }
        /* Copy propagation: b=a */
        else if (sscanf(clean, "%c=%c", &lhs, &x) == 2) {
            if (getValue(x, &result))
                setVariable(lhs, result, 1);
            else
                setVariable(lhs, 0, 0);
        }
    }
    printf("\nAfter Constant Propagation:\n");
    for (i = 0; i < count; i++) {
        if (table[i].constant)
            printf("%c=%d\n", table[i].name, table[i].value);
    }
    return 0;
}
