#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD 100
#define MAX_HISTORY 10

void executeFork(char **args);
void addHistory(const char *cmd);
void showHistory();
void handleCd(char **args);
void showDate();
void showTime();
void runCFile(char *filename);
void openNotepad(char *filename);
void clearScreen();
void printHelp();
void copyFile(char *src, char *dest);
void moveFile(char *src, char *dest);
void removeFile(char *filename);
void makeDir(char *dirname);
void removeDir(char *dirname);

int main() {
    char input[MAX_CMD];
    char *args[20];
    char cwd[200];


    printf("Custom Shell (myShell)\n");
    printf("Type 'help' to see available commands\n");

    while (1) {
        getcwd(cwd, sizeof(cwd));
        printf("myShell:%s> ", cwd);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
            continue;

        addHistory(input);

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL)
            args[++i] = strtok(NULL, " ");

        if (strcmp(args[0], "exit") == 0)
            break;
        else if (strcmp(args[0], "cd") == 0)
            handleCd(args);
        else if (strcmp(args[0], "history") == 0)
            showHistory();
        else if (strcmp(args[0], "date") == 0)
            showDate();
        else if (strcmp(args[0], "time") == 0)
            showTime();
        else if (strcmp(args[0], "clear") == 0)
            clearScreen();
        else if (strcmp(args[0], "help") == 0)
            printHelp();
        else if (strcmp(args[0], "whoami") == 0) {
            char *cmd[] = {"whoami", NULL};
            executeFork(cmd);
        }
        else if (strcmp(args[0], "notepad") == 0)
            openNotepad(args[1]);
        else if (strcmp(args[0], "run") == 0)
            runCFile(args[1]);
        else if (strcmp(args[0], "mkdir") == 0)
            makeDir(args[1]);
        else if (strcmp(args[0], "rmdir") == 0)
            removeDir(args[1]);
        else if (strcmp(args[0], "rm") == 0)
            removeFile(args[1]);
        else if (strcmp(args[0], "cp") == 0)
            copyFile(args[1], args[2]);
        else if (strcmp(args[0], "mv") == 0)
            moveFile(args[1], args[2]);
        else
            executeFork(args);
    }

    printf("Exiting myShell...\n");
    return 0;
}
