// Compact First/Follow set calculator for a user-supplied grammar.
// Production format: "A=XYZ" (no spaces). Use '#' for an epsilon RHS, e.g. "R=#".
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXP 20   // max productions
#define MAXS 128  // ASCII symbol range

char prod[MAXP][10];
int n;
int first[MAXS][MAXS], follow[MAXS][MAXS];

// Fixed-point computation of First sets: keep sweeping the productions,
// adding symbols to First(A) until nothing changes anymore.
void computeFirst(void) {
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < n; i++) {
            int A = prod[i][0], allEps = 1;
            for (int j = 2; prod[i][j] && allEps; j++) {
                int X = prod[i][j];
                if (!isupper(X)) {
                    if (!first[A][X]) { first[A][X] = 1; changed = 1; }
                    allEps = 0;
                } else {
                    for (int c = 0; c < MAXS; c++)
                        if (first[X][c] && c != '#' && !first[A][c]) { first[A][c] = 1; changed = 1; }
                    if (!first[X]['#']) allEps = 0;
                }
            }
            if (allEps && !first[A]['#']) { first[A]['#'] = 1; changed = 1; }
        }
    }
}

// Fixed-point computation of Follow sets using the standard rules:
//   $ in Follow(start)
//   A -> a B b   =>  First(b)-{eps} subset of Follow(B)
//   A -> a B b, eps in First(b) (or b empty)  =>  Follow(A) subset of Follow(B)
void computeFollow(void) {
    follow[(int)prod[0][0]]['$'] = 1;
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < n; i++) {
            int A = prod[i][0], len = (int)strlen(prod[i]);
            for (int j = 2; j < len; j++) {
                int B = prod[i][j];
                if (!isupper(B)) continue;

                int k = j + 1, epsToEnd = 1;
                while (k < len) {
                    int X = prod[i][k];
                    if (!isupper(X)) {
                        if (!follow[B][X]) { follow[B][X] = 1; changed = 1; }
                        epsToEnd = 0;
                        break;
                    }
                    for (int c = 0; c < MAXS; c++)
                        if (first[X][c] && c != '#' && !follow[B][c]) { follow[B][c] = 1; changed = 1; }
                    if (!first[X]['#']) { epsToEnd = 0; break; }
                    k++;
                }
                if (epsToEnd)
                    for (int c = 0; c < MAXS; c++)
                        if (follow[A][c] && !follow[B][c]) { follow[B][c] = 1; changed = 1; }
            }
        }
    }
}

// Print the set for symbol A from the given table, skipping repeats.
void printSet(const char *label, int table[MAXS][MAXS]) {
    char done[MAXP];
    int dn = 0;
    for (int i = 0; i < n; i++) {
        int A = prod[i][0];
        int seen = 0;
        for (int k = 0; k < dn; k++) if (done[k] == A) seen = 1;
        if (seen) continue;
        done[dn++] = (char)A;

        printf("%s(%c) = { ", label, A);
        int firstItem = 1;
        for (int c = 0; c < MAXS; c++)
            if (table[A][c]) { printf("%s%c", firstItem ? "" : ", ", c); firstItem = 0; }
        printf(" }\n");
    }
}

int main(void) {
    printf("Enter the number of productions: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAXP) { printf("Invalid input!\n"); return 1; }

    printf("Enter the productions\n");
    for (int i = 0; i < n; i++) {
        printf("Production %d: ", i + 1);
        scanf("%9s", prod[i]);
    }

    computeFirst();
    computeFollow();

    printf("\n");
    printSet("First", first);
    printf("\n");
    printSet("Follow", follow);
    return 0;
}