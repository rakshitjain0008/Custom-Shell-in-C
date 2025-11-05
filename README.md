# Custom Shell in C

This is a simple shell program written in C as part of my Operating Systems course project.  
It works like a mini command-line interface and supports basic file, directory, and system operations.

---

## Features

- `ls` → list files and folders in the current directory  
- `pwd` → print current working directory  
- `cd <dir>` → change directory  
- `mkdir <dir>` → create a new directory  
- `rmdir <dir>` → delete an empty directory  
- `rm <file>` → remove a file  
- `cp <src> <dest>` → copy a file  
- `mv <src> <dest>` → move or rename a file  
- `cat <file>` → show file content  
- `date` → show current date  
- `time` → show current time  
- `whoami` → display the current username  
- `run <file.c>` → compile and execute a C file  
- `nano <file>` → open a file using the nano text editor  
- `clear` → clear the terminal screen  
- `history` → display the list of recent commands  
- `help` → show all available commands  
- `exit` → exit the shell  

---

## How to Run

1. Make sure you have **GCC (MinGW)** installed and added to your **PATH**.  
2. Open **Command Prompt** in the project folder.  
3. Compile using:  
   ```bash
   gcc myshell.c shell_functions.c -o myshell
