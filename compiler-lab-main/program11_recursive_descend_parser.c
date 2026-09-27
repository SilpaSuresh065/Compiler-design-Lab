// E → T E'
// E' → + T E' | ε
// T → F T'
// T' → * F T' | ε
// F → (E) | i

#include <stdio.h>
#include <string.h>

char input[100];
int pos = 0;
int error = 0;
void E();
void Eprime();
void T();
void Tprime();
void F();

void E() {
    if (error) return;
    T();
    Eprime();
}

void Eprime() {
    if (error) return;
    if (input[pos] == '+')
    {
        pos++;
        T();
        Eprime();
    }
}

void T() {
    if (error) return;
    F();
    Tprime();
}

void Tprime() {
    if (error) return;
    if (input[pos] == '*')
    {
        pos++;
        F();
        Tprime();
    }
}

void F() {
    if (error) return;
    if (input[pos] == '(')
    {
        pos++;
        E();
        if (input[pos] == ')')
            pos++;
        else
        {
            if (!error) {
                printf("String Rejected\n");
                error = 1;
            }
            return;
        }
    }
    else if (input[pos] == 'i')
        pos++;
    else
    {
        if (!error) {
            printf("String Rejected\n");
            error = 1;
        }
    }
}

int main() {
    pos = 0;
    error = 0;
    printf("Enter input string: ");
    if (scanf("%s", input) != 1) return 0;
    E();
    if (!error && input[pos] == '\0')
        printf("String Accepted\n");
    else if (!error)
        printf("String Rejected\n"); 
    return 0;
}