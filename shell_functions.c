#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_CMD 100
#define MAX_HISTORY 10

char history[MAX_HISTORY][MAX_CMD];
int historyCount = 0;


void addHistory(const char *cmd) {
    if (historyCount < MAX_HISTORY)
        strcpy(history[historyCount++], cmd);
    else {
        for (int i = 1; i < MAX_HISTORY; i++)
            strcpy(history[i - 1], history[i]);
        strcpy(history[MAX_HISTORY - 1], cmd);
    }
}

void showHistory() {
    if (historyCount == 0) {
        printf("No commands in history yet.\n");
        return;
    }
    printf("Command History (Total: %d)\n", historyCount);
    for (int i = 0; i < historyCount; i++)
        printf("%d: %s\n", i + 1, history[i]);
}


void executeFork(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1)
            perror("execution failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

void handleCd(char **args) {
    if (args[1] == NULL)
        chdir(getenv("HOME"));
    else if (chdir(args[1]) != 0)
        perror("cd");
}

void showDate() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[50];
    strftime(buffer, 50, "%d-%m-%Y", tm_info);
    printf("%s\n", buffer);
}

void showTime() {
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%I:%M:%S %p %Z", tm_info);
    printf("%s\n", buffer);
    unsetenv("TZ");
    tzset();
}

void runCFile(char *filename) {
    char output[50];
    strcpy(output, "a.out");
    char *compile[] = {"gcc", filename, "-o", output, NULL};
    executeFork(compile);

    char *run[] = {"./a.out", NULL};
    executeFork(run);
    printf("\n");
}

void openNotepad(char *filename) {
    char *args[] = {"nano", filename, NULL};
    executeFork(args);
}

void clearScreen() {
    printf("\033[H\033[J");
}

void printHelp() {
    printf("Available commands:\n");
    printf("ls                : List files and directories\n");
    printf("pwd               : Show current working directory\n");
    printf("cd <dir>          : Change current directory\n");
    printf("mkdir <dir>       : Create a new directory\n");
    printf("rmdir <dir>       : Remove an empty directory\n");
    printf("rm <file>         : Delete a file\n");
    printf("cp <src> <dest>   : Copy file from source to destination\n");
    printf("mv <src> <dest>   : Move or rename a file\n");
    printf("cat <file>        : Display contents of a file\n");
    printf("echo <text>       : Print text to the screen\n");
    printf("date              : Display current date\n");
    printf("time              : Display current time\n");
    printf("whoami            : Show current user name\n");
    printf("run <file.c>      : Compile and execute a C file\n");
    printf("notepad <file>    : Open file using nano text editor\n");
    printf("history           : Show list of previous commands\n");
    printf("clear             : Clear the terminal screen\n");
    printf("help              : Show all available commands\n");
    printf("exit              : Exit from the shell\n");
}

void copyFile(char *src, char *dest) {
    FILE *source = fopen(src, "rb");
    if (!source) {
        perror("Cannot open source file");
        return;
    }

    FILE *target = fopen(dest, "wb");
    if (!target) {
        perror("Cannot create destination file");
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0)
        fwrite(buffer, 1, bytes, target);

    fclose(source);
    fclose(target);
    printf("File copied successfully.\n");
}

void moveFile(char *src, char *dest) {
    if (rename(src, dest) != 0) {
        copyFile(src, dest);
        remove(src);
    }
}


void removeFile(char *filename) {
    if (remove(filename) != 0)
        perror("rm");
}

void makeDir(char *dirname) {
    if (mkdir(dirname, 0777) != 0)
        perror("mkdir");
}

void removeDir(char *dirname) {
    if (rmdir(dirname) != 0)
        perror("rmdir");
}
