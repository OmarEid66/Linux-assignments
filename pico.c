#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define SIZE 1000000

int picoshell_main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    char input[SIZE];
    int last_status = 0;

    while (1) {
        printf("pico shell -> ");
        fflush(stdout);

        if (fgets(input, SIZE, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (*input == '\0') {
            continue;
        }

        if (strncmp(input, "echo ", 5) == 0) {
            char *text = input + 5;
            char *dst = text;
            int space_flag = 0;

            while (isspace(*text)) {
                text++;
            }

            while (*text) {
                if (isspace(*text)) {
                    if (!space_flag) {
                        *dst++ = ' ';
                        space_flag = 1;
                    }
                    text++;
                } else {
                    *dst++ = *text++;
                    space_flag = 0;
                }
            }
            *dst = '\0';
            printf("%s\n", input + 5);
            last_status = 0;
        }
        else if (strcmp(input, "exit") == 0) {
            printf("Good Bye\n");
            break;
        }
        else if (strncmp(input, "cd ", 3) == 0) {
            char *directory = input + 3;
            if (chdir(directory) == -1) {
                printf("cd: %s: %s\n", directory, strerror(errno));
                last_status = 1;
            } else {
                last_status = 0;
            }
        }
        else if (strcmp(input, "pwd") == 0) {
            char cwd[SIZE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
                last_status = 0;
            } else {
                perror("getcwd error");
                last_status = 1;
            }
        }
        else {
            pid_t pid = fork();

            if (pid == -1) {
                perror("fork failed");
                last_status = 1;
                continue;
            }
            
            if (pid == 0) {
                char *args[SIZE / 2];
                int index = 0;
                char *token = strtok(input, " ");
                char *command = token; // Save the command name for error reporting
                
                while (token != NULL) {
                    args[index++] = token;
                    token = strtok(NULL, " ");
                }
                args[index] = NULL;
                
                if (execvp(args[0], args) == -1) {
                    fprintf(stderr, "%s: command not found\n", command);
                    exit(127); // Standard exit code for "command not found"
                }
            }
            else {
                int status;
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid failed");
                    last_status = 1;
                } else {
                    last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
                }
            }
        }
    }
    
    return last_status;
}