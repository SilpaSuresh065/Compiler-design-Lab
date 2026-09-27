#include <stdio.h>
#include <string.h>

char *grammar[] = {
    "E->E+E",
    "E->E*E",
    "E->E-E",
    "E->E/E",
    "E->(E)",
    "E->i"
};

int num_rules = sizeof(grammar) / sizeof(grammar[0]);

char stack[100];
char input[100];

void reduce(int input_pos) {
    int reduced = 1;
    
    while (reduced) {
        reduced = 0;
        int stack_len = strlen(stack);
        
        for (int r = 0; r < num_rules; r++) {
            char lhs = grammar[r][0];       
            char *rhs = grammar[r] + 3;     
            int rhs_len = strlen(rhs);
            
            if (stack_len >= rhs_len) {
                if (strncmp(&stack[stack_len - rhs_len], rhs, rhs_len) == 0) {
                    
                    stack[stack_len - rhs_len] = lhs;
                    stack[stack_len - rhs_len + 1] = '\0'; 
                    
                    // FIXED: Increased size to 150 and used snprintf for safety
                    char st_str[150], in_str[150];
                    snprintf(st_str, sizeof(st_str), "$%s", stack);
                    snprintf(in_str, sizeof(in_str), "%s$", input + input_pos);
                    
                    printf("%-20s %-15s REDUCE TO %s\n", st_str, in_str, grammar[r]);
                    
                    reduced = 1; 
                    break;       
                }
            }
        }
    }
}

int main() {
    printf("CURRENT GRAMMAR:\n");
    for (int r = 0; r < num_rules; r++) {
        printf("%d. %s\n", r + 1, grammar[r]);
    }
    
    printf("\nEnter input string: ");
    if (scanf("%s", input) != 1) return 1;
    
    int input_len = strlen(input);
    stack[0] = '\0'; 
    
    printf("\n%-20s %-15s %s\n", "STACK", "INPUT BUFFER", "ACTION");
    printf("-------------------------------------------------------\n");
    
    for (int i = 0; i < input_len; i++) {
        int len = strlen(stack);
        stack[len] = input[i];
        stack[len + 1] = '\0';
        
        // FIXED: Increased size to 150 and used snprintf
        char st_str[150], in_str[150];
        snprintf(st_str, sizeof(st_str), "$%s", stack);
        snprintf(in_str, sizeof(in_str), "%s$", input + i + 1);
        
        printf("%-20s %-15s SHIFT\n", st_str, in_str);
        
        reduce(i + 1);
    }
    
    // FIXED: Increased size to 150 and used snprintf
    char final_st[150], final_in[15] = "$";
    snprintf(final_st, sizeof(final_st), "$%s", stack);
    
    if (strlen(stack) == 1 && stack[0] == grammar[0][0]) {
        printf("%-20s %-15s ACCEPT\n", final_st, final_in);
    } else {
        printf("%-20s %-15s REJECT\n", final_st, final_in);
    }
    
    printf("-------------------------------------------------------\n");
    
    return 0;
}