#include <stdio.h>
#include <string.h>

#define MAX 20
#define MAX_SYMBOLS 20

int n, t;
int eps[MAX][MAX];
int trans[MAX][MAX][MAX_SYMBOLS];
char symbols[MAX_SYMBOLS];
int symbolCount = 0;
int closure[MAX][MAX];
int visited[MAX];

/* Find index of a symbol */
int symbolIndex(char c)
{
    int i;

    for (i = 0; i < symbolCount; i++)
        if (symbols[i] == c)
            return i;

    symbols[symbolCount] = c;
    return symbolCount++;
}

/* Find epsilon closure using DFS */
void dfs(int state)
{
    int i;

    visited[state] = 1;

    for (i = 0; i < n; i++) {
        if (eps[state][i] && !visited[i])
            dfs(i);
    }
}

/* Find epsilon closure of every state */
void findClosures()
{
    int i, j;

    for (i = 0; i < n; i++) {

        for (j = 0; j < n; j++)
            visited[j] = 0;

        dfs(i);

        for (j = 0; j < n; j++)
            closure[i][j] = visited[j];
    }
}

/*
   Find states reachable from state using:
   epsilon closure -> symbol -> epsilon closure
*/
void findNewTransitions()
{
    int i, j, k, s;
    int symbol;

    for (i = 0; i < n; i++) {

        for (symbol = 0; symbol < symbolCount; symbol++) {

            /* For every state in epsilon closure(i) */
            for (j = 0; j < n; j++) {

                if (closure[i][j]) {

                    /* Follow the current symbol */
                    for (k = 0; k < n; k++) {

                        if (trans[j][k][symbol]) {

                            /*
                               Add epsilon closure of
                               the destination state
                            */
                            for (s = 0; s < n; s++) {
                                if (closure[k][s])
                                    trans[i][s][symbol] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    int i, j, k;
    char input[20];
    int from, to;
    char symbol;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter total number of transitions: ");
    scanf("%d", &t);

    /* Initialize arrays */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            eps[i][j] = 0;

            for (k = 0; k < MAX_SYMBOLS; k++)
                trans[i][j][k] = 0;
        }
    }

    printf("\nEnter transitions (example: q0#q1 or q0aq1):\n");

    for (i = 0; i < t; i++) {

        scanf("%s", input);

        from = input[1] - '0';
        symbol = input[2];
        to = input[4] - '0';

        if (symbol == '#') {
            eps[from][to] = 1;
        }
        else {
            int s = symbolIndex(symbol);
            trans[from][to][s] = 1;
        }
    }

    /* Step 1: Find epsilon closures */
    findClosures();

    printf("\nEpsilon Closures:\n");

    for (i = 0; i < n; i++) {
        printf("E-closure(q%d) = { ", i);

        for (j = 0; j < n; j++) {
            if (closure[i][j])
                printf("q%d ", j);
        }

        printf("}\n");
    }

    /* Step 2: Construct NFA without epsilon */
    findNewTransitions();

    printf("\nNFA without epsilon transitions:\n");

    for (i = 0; i < n; i++) {

        for (k = 0; k < symbolCount; k++) {

            printf("q%d --%c--> { ", i, symbols[k]);

            for (j = 0; j < n; j++) {

                if (trans[i][j][k])
                    printf("q%d ", j);
            }

            printf("}\n");
        }
    }

    return 0;
}
