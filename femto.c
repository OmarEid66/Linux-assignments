#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int femtoshell_main(int argc, char *argv[]) {
    char input[1000000];
    int last_command_status = 0; // Track the status of the last command (0 = success, 1 = failure)
    
    while(1) {
        printf(" Femto shell -> ");
        
        if (fgets(input, 1000000, stdin) == NULL)
            break;
        
        input[strcspn(input, "\n")] = '\0';

        if (*input == '\0') // Check for empty input
            continue;
        
        if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
            last_command_status = 0; // Success
        }
        else if (strcmp(input, "exit") == 0) {
            printf("Good Bye\n");
            break;
        }
        else {
            printf("Invalid command\n");
            last_command_status = 1; // Failure
        }
    }
    
    return last_command_status; // Return the status of the last command
}