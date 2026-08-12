# Custom_UNIX_Shell

# Raj's Custom Shell (CSH)

A lightweight, minimalist Unix-based shell implementation written in C. This project demonstrates core shell functionality including command execution, built-in commands, and process management.

## Features

### Core Functionality
- **Interactive Command Loop**: Read and execute user commands in a REPL (Read-Eval-Print-Loop) environment
- **Command Parsing**: Intelligently tokenize input based on standard delimiters (spaces, tabs, newlines)
- **Process Management**: Fork and execute external programs while managing child process lifecycle
- **Built-in Commands**: Quick access to essential shell operations without spawning new processes

### Built-in Commands

| Command | Description |
|---------|-------------|
| `cd` | Change the current working directory |
| `help` | Display list of available built-in commands |
| `exit` | Terminate the shell session |

## Getting Started

### Prerequisites
- GCC or any POSIX-compliant C compiler
- Unix/Linux operating system (macOS, Linux, etc.)
- Standard C libraries (stdio, stdlib, unistd)

### Compilation

```bash
gcc -o csh main.c
```

Or with additional compiler flags:

```bash
gcc -Wall -Wextra -O2 -o csh main.c
```

### Running the Shell

```bash
./csh
```

You'll see the shell prompt:
```
@ 
```

Now you can start typing commands!

## Usage Examples

### Built-in Commands

```bash
@ help
Welcome to Raj's first custom shell !The following commands are available to use :D
cd 
help 
exit 

@ cd /path/to/directory
The cd command is working fine.

@ exit
```

### External Commands

Execute any standard Unix/Linux command:

```bash
@ ls -la
@ echo "Hello, World!"
@ cat filename.txt
@ pwd
@ whoami
```

## Project Structure

### Key Functions

**`csh_read_lines()`**
- Reads a single line of input from stdin using `getline()`
- Handles EOF gracefully by exiting the shell
- Dynamically allocates memory for the input buffer

**`csh_split_lines(char *line)`**
- Tokenizes the input string using standard shell delimiters
- Returns an array of argument strings (like `argv`)
- Dynamically resizes buffer if needed

**`csh_launch(char **args)`**
- Creates a child process using `fork()`
- Executes external programs using `execvp()`
- Waits for the child process to complete using `waitpid()`

**`csh_exec(char **args)`**
- Routes commands to either built-in functions or external launchers
- Checks if the command matches any built-in string

**`csh_loop()`**
- Main shell event loop
- Handles input → parsing → execution → cleanup cycle

**Built-in Command Handlers**
- `csh_cd()`: Changes directory using `chdir()`
- `csh_help()`: Displays available commands
- `csh_exit()`: Returns 0 to signal loop termination

## How It Works

1. **Initialization**: Shell enters the main loop and prints the prompt (`@`)
2. **Input Reading**: User input is read line-by-line from stdin
3. **Tokenization**: The line is split into individual command arguments
4. **Command Routing**: 
   - Check if the command is built-in
   - If yes, execute it directly
   - If no, fork a child process and execute it externally
5. **Process Management**: Parent process waits for child to complete
6. **Cleanup**: Free allocated memory before the next iteration
7. **Termination**: Loop exits when `exit` command is called

## Technical Highlights

### Memory Management
- Dynamic allocation for command buffers using `malloc()` and `realloc()`
- Proper cleanup with `free()` after each command execution
- Error checking for allocation failures

### Process Control
- Uses `fork()` to create child processes
- Uses `execvp()` to overlay child with new program image
- Uses `waitpid()` with `WUNTRACED` flag for robust child management
- Handles signal-terminated processes via `WIFSIGNALED()`

### Signal Handling
- Waits for child processes to either exit normally (`WIFEXITED`) or be killed by signal (`WIFSIGNALED`)

## Limitations & Future Enhancements

### Current Limitations
- No support for pipes (`|`) or redirection (`>`, `<`, `>>`)
- No background process execution (`&`)
- No command history or line editing
- No environment variable support
- Limited error messages
- No signal handling (e.g., Ctrl+C)

### Potential Improvements
- Implement I/O redirection (stdin/stdout/stderr)
- Add pipe support for command chaining
- Background job management
- Command history using readline library
- Environment variable expansion
- Alias support
- Better error reporting
- Config file parsing
- Job control (fg/bg commands)

## Code Quality Notes

### Strengths
- Clean separation of concerns with modular functions
- Good use of function pointers for command dispatch
- Proper memory allocation error checking
- Standard Unix conventions followed

### Areas for Improvement
- Add input validation and bounds checking
- Implement proper signal handlers for Ctrl+C
- Add more descriptive error messages
- Include comments for complex logic
- Handle edge cases (empty input, very long commands)
- Use `strtok_r()` instead of `strtok()` for thread safety

## Learning Resources

This project demonstrates important Unix programming concepts:
- Process creation and management (`fork()`, `exec()`, `wait()`)
- System calls and signals
- Dynamic memory allocation in C
- String tokenization and parsing
- File descriptor and I/O operations

---

*Happy Shelling! 🐚*
