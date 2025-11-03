#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>

#define MAX_HISTORY 10

char history[MAX_HISTORY][100];
int historyCount = 0;

void addHistory(const char *cmd)
{
    if (historyCount < MAX_HISTORY)
    {
        strcpy(history[historyCount++], cmd);
    }
    else
    {
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[MAX_HISTORY - 1], cmd);
    }
}

void showHistory()
{
    printf("Last %d commands:\n", historyCount);
    for (int i = 0; i < historyCount; i++)
    {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

void listDirectory()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    else
    {
        printf("Unable to open directory\n");
    }
}


void createFile(char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp != NULL)
    {
        printf("Created file: %s\n", filename);
        fclose(fp);
    }
    else
    {
        printf("Failed to create file\n");
    }
}

void showDate()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%d-%m-%Y", tm_info);
    printf("Today's Date: %s\n", buffer);
}

void showTime()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%H:%M:%S", tm_info);
    printf("Current Time: %s\n", buffer);
}

void removeFile(char *filename)
{
    if (DeleteFileA(filename))
    {
        printf("Deleted file: %s\n", filename);
    }
    else
    {
        printf("File not found or cannot delete file\n");
    }
}

void removeDir(char *dirname)
{
    char path[1024];
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    snprintf(path, sizeof(path), "%s\\*", dirname);
    hFind = FindFirstFileA(path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Directory not found\n");
        return;
    }

    do
    {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", dirname, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            removeDir(fullPath);
        }
        else
        {
            SetFileAttributesA(fullPath, FILE_ATTRIBUTE_NORMAL);
            DeleteFileA(fullPath);
        }
    } while (FindNextFileA(hFind, &findFileData));

    FindClose(hFind);
    SetFileAttributesA(dirname, FILE_ATTRIBUTE_NORMAL);
    if (RemoveDirectoryA(dirname))
    {
        printf("Deleted directory: %s\n", dirname);
    }
    else
    {
        printf("Failed to delete directory (might be in use or not empty)\n");
    }
}

void makeDir(char *dirname)
{
    if (CreateDirectoryA(dirname, NULL))
    {
        printf("Created directory: %s\n", dirname);
    }
    else
    {
        printf("Failed to create directory\n");
    }
}

int main()
{
    char command[100], cwd[1024];

    while (1)
    {
        getcwd(cwd, sizeof(cwd));
        printf("myshell%s> ", cwd);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // remove newline

        if (strlen(command) == 0)
            continue;
        addHistory(command);

        if (strcmp(command, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }
        else if (strcmp(command, "ls") == 0)
        {
            listDirectory();
        }
        else if (strcmp(command, "pwd") == 0)
        {
            printf("%s\n", cwd);
        }
        else if (strncmp(command, "cd ", 3) == 0)
        {
            char *path = command + 3;
            if (chdir(path) != 0)
                printf("No directory found\n");
        }
        else if (strcmp(command, "date") == 0)
        {
            showDate();
        }
        else if (strcmp(command, "time") == 0)
        {
            showTime();
        }
        else if (strncmp(command, "rm ", 3) == 0)
        {
            removeFile(command + 3);
        }
        else if (strncmp(command, "rmdir ", 6) == 0)
        {
            removeDir(command + 6);
        }
        else if (strncmp(command, "mkdir ", 6) == 0)
        {
            makeDir(command + 6);
        }
        else if (strcmp(command, "history") == 0)
        {
            showHistory();
        }
        else if (strcmp(command, "clear") == 0)
        {
            system("cls");
        }
        else if (strncmp(command, "run ", 4) == 0)
        {
            char *filename = command + 4;
            char exeName[100] = {0};

            // remove .c extension if present for exe name
            strcpy(exeName, filename);
            char *dot = strrchr(exeName, '.');
            if (dot && strcmp(dot, ".c") == 0)
            {
                *dot = '\0';
            }

            char compileCmd[200], runCmd[200];
            snprintf(compileCmd, sizeof(compileCmd), "gcc \"%s\" -o \"%s.exe\"", filename, exeName);
            snprintf(runCmd, sizeof(runCmd), "%s.exe", exeName);

            printf("Compiling %s...\n", filename);
            if (system(compileCmd) == 0)
            {
                printf("Running %s...\n", runCmd);
                system(runCmd);
            }
            else
            {
                printf(" Compilation failed. Check for errors.\n");
            }
            printf("\n");
        }
        else if (strncmp(command, "notepad ", 8) == 0)
        {
            char filename[100];
            strcpy(filename, command + 8);
            char cmd[200];
            snprintf(cmd, sizeof(cmd), "notepad %s", filename);
            system(cmd);
        }
        else if (strncmp(command, "create ", 7) == 0)
        {
            createFile(command + 7);
        }
        else if (strcmp(command, "whoami") == 0)
        {
            char username[100];
            DWORD size = sizeof(username);
            if (GetUserNameA(username, &size))
            {
                printf("Current user: %s\n", username);
            }
            else
            {
                printf("Unable to retrieve username.\n");
            }
        }
        else if (strcmp(command, "help") == 0)
        {
            printf("Available commands:\n");
            printf("  ls           - List files in current directory\n");
            printf("  pwd          - Show current working directory\n");
            printf("  cd <dir>     - Change directory\n");
            printf("  mkdir <dir>  - Create directory\n");
            printf("  rmdir <dir>  - Remove directory\n");
            printf("  create <f>   - Create empty file\n");
            printf("  rm <f>       - Delete file\n");
            printf("  cat <f>      - Display file content\n");
            printf("  rename <o> <n> - Rename file or directory\n");
            printf("  echo <text>  - Print text\n");
            printf("  date         - Show today's date\n");
            printf("  time         - Show current time\n");
            printf("  whoami       - Show current username\n");
            printf("  run <file.c> - Compile and run C file\n");
            printf("  notepad <f>  - Open file in Notepad\n");
            printf("  history      - Show command history\n");
            printf("  clear        - Clear the screen\n");
            printf("  exit         - Exit the shell\n");
        }
        else if (strncmp(command, "echo ", 5) == 0)
        {
            printf("%s\n", command + 5);
        }
        else if (strncmp(command, "cat ", 4) == 0)
        {
            char *filename = command + 4;
            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
            {
                printf("Cannot open file: %s\n", filename);
            }
            else
            {
                char ch;
                while ((ch = fgetc(fp)) != EOF)
                    putchar(ch);
                fclose(fp);
                printf("\n");
            }
        }
        else if (strncmp(command, "rename ", 7) == 0)
        {
            char oldName[100], newName[100];
            if (sscanf(command + 7, "%s %s", oldName, newName) == 2)
            {
                if (MoveFileA(oldName, newName))
                {
                    printf("Renamed %s to %s\n", oldName, newName);
                }
                else
                {
                    printf("Failed to rename %s\n", oldName);
                }
            }
            else
            {
                printf("Usage: rename <oldname> <newname>\n");
            }
        }
        else if (strncmp(command, "cp ", 3) == 0)
        {
            char src[100], dest[100];
            if (sscanf(command + 3, "%s %s", src, dest) == 2)
            {
                FILE *srcFile = fopen(src, "rb");
                if (srcFile == NULL)
                {
                    printf("Cannot open source file: %s\n", src);
                }
                else
                {
                    FILE *destFile = fopen(dest, "wb");
                    if (destFile == NULL)
                    {
                        printf("Cannot create destination file: %s\n", dest);
                        fclose(srcFile);
                    }
                    else
                    {
                        char buffer[1024];
                        size_t bytes;
                        while ((bytes = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
                            fwrite(buffer, 1, bytes, destFile);

                        fclose(srcFile);
                        fclose(destFile);
                        printf("Copied %s to %s\n", src, dest);
                    }
                }
            }
            else
            {
                printf("Usage: cp <source> <destination>\n");
            }
        }
        else if (strncmp(command, "mv ", 3) == 0)
        {
            char src[100], dest[100];
            if (sscanf(command + 3, "%s %s", src, dest) == 2)
            {
                if (MoveFileExA(src, dest, MOVEFILE_REPLACE_EXISTING))
                {
                    printf("Moved %s to %s\n", src, dest);
                }
                else
                {
                    printf("Failed to move %s\n", src);
                }
            }
            else
            {
                printf("Usage: mv <source> <destination>\n");
            }
        }
        else
        {
            printf("Command not found\n");
        }
    }
    return 0;
}
