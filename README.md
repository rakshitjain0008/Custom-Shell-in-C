# Custom Shell in C

This is a simple shell program written in C as part of my Operating Systems course project.  
It works like a mini command-line interface and supports basic file, directory, and system operations.

---

## Features

- ls → list files and folders in the current directory  
- pwd → print current working directory  
- cd <dir> → change directory  
- mkdir <dir> → create a new directory  
- rmdir <dir> → delete a directory  
- create <file> → create a new empty file  
- rm <file> → remove a file  
- cat <file> → show file content  
- rename <old> <new> → rename a file or folder  
- cp <src> <dest> → copy a file  
- mv <src> <dest> → move or rename file  
- date / time → show current date or time  
- whoami → display the current Windows username  
- run <file.c> → compile and run a C file using GCC  
- notepad <file> → open a file in Notepad  
- history → show last 10 executed commands  
- clear → clear the console screen  
- exit → close the shell  

---

## How to Run

1. Make sure you have *GCC (MinGW)* installed and added to your PATH.  
2. Open Command Prompt in the project folder.  
3. Compile using:
   ```bash
   gcc myshell.c -o myshell.exe
