#include <stdio.h>
#include <string.h>

#define MAX 20

int n, t;
int epsilon[MAX][MAX];
int visited[MAX];

/* Find epsilon closure using DFS */
void dfs(int state)
{
    int i;

    visited[state] = 1;

    for (i = 0; i < n; i++) {
        if (epsilon[state][i] && !visited[i])
            dfs(i);
    }
}

int main()
{
    int i, j;
    char transition[20];
    int from, to;
    char symbol;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter total number of transitions: ");
    scanf("%d", &t);

    /* Initialize epsilon transition matrix */
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            epsilon[i][j] = 0;

    printf("\nEnter transitions (example: q0#q1):\n");

    for (i = 0; i < t; i++) {

        scanf("%s", transition);

        /*
           Format:
           q0#q1
           q0aq1
        */

        from = transition[1] - '0';
        symbol = transition[2];
        to = transition[4] - '0';

        /* # represents epsilon */
        if (symbol == '#')
            epsilon[from][to] = 1;
    }

    printf("\nEpsilon Closure of all states:\n");

    for (i = 0; i < n; i++) {

        /* Reset visited */
        for (j = 0; j < n; j++)
            visited[j] = 0;

        /* Find epsilon closure */
        dfs(i);

        printf("E-closure(q%d) = { ", i);

        for (j = 0; j < n; j++) {
            if (visited[j])
                printf("q%d ", j);
        }

        printf("}\n");
    }

    return 0;
}
