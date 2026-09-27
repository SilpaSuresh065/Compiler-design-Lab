#include <stdio.h>
#include <ctype.h>
#include <string.h>

char keywords[][20] = {
    "int", "float", "char", "if", "else", "while", "for",
    "return", "void", "break", "continue", "double", "long",
    "short", "switch", "case", "default", "do"
};

int isKeyword(char str[]) {
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {
    FILE *fp;
    char ch, buffer[100];
    int i;

    fp = fopen("input.c", "r");

    if (fp == NULL) {
        printf("Cannot open file!\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {

        /* Ignore spaces, tabs, and newlines */
        if (isspace(ch))
            continue;

        /* Ignore comments */
        if (ch == '/') {
            char next = fgetc(fp);

            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            }
            else if (next == '*') {
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '*') {
                        if ((ch = fgetc(fp)) == '/')
                            break;
                        else
                            fseek(fp, -1, SEEK_CUR);
                    }
                }
                continue;
            }
            else {
                fseek(fp, -1, SEEK_CUR);
                // Fall through to operator handling for '/'
            }
        }

        /* Identifier or Keyword */
        if (isalpha(ch) || ch == '_') {
            i = 0;
            buffer[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                if (i < 98) buffer[i++] = ch;
            }
            buffer[i] = '\0';

            if (ch != EOF)
                fseek(fp, -1, SEEK_CUR);

            if (isKeyword(buffer))
                printf("Keyword      : %s\n", buffer);
            else
                printf("Identifier   : %s\n", buffer);
        }

        /* Number */
        else if (isdigit(ch)) {
            i = 0;
            buffer[i++] = ch;
            int dot_count = 0;

            while ((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.')) {
                if (ch == '.') dot_count++;
                if (dot_count > 1) break; // prevent multiple dots
                if (i < 98) buffer[i++] = ch;
            }
            buffer[i] = '\0';

            if (ch != EOF)
                fseek(fp, -1, SEEK_CUR);

            printf("Number       : %s\n", buffer);
        }

        /* String Literal */
        else if (ch == '"') {
            i = 0;
            while ((ch = fgetc(fp)) != '"' && ch != EOF) {
                if (i < 98) buffer[i++] = ch;
            }
            buffer[i] = '\0';
            printf("String       : \"%s\"\n", buffer);
        }

        /* Multi-character and Single Operators */
        else if (strchr("+-*=<>!&|/", ch)) {
            char next = fgetc(fp);
            if ((ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && next == '=') ||
                (ch == '>' && next == '=') ||
                (ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|')) {
                printf("Operator     : %c%c\n", ch, next);
            } else {
                fseek(fp, -1, SEEK_CUR);
                printf("Operator     : %c\n", ch);
            }
        }

        /* Special Symbols */
        else if (strchr("(){}[];,.", ch)) {
            printf("Special Symbol : %c\n", ch);
        }

        /* Invalid Character */
        else {
            printf("Invalid Token : %c\n", ch);
        }
    }

    fclose(fp);
    return 0;
}