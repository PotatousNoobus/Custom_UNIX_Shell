#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define csh_TOK_BUFSIZE 64
#define csh_TOK_DELIM " \t\r\n\a"

int csh_cd(char **args);
int csh_help(char **args);
int csh_exit(char **args);

char* builtin_str[] = {"cd", "help", "exit"};
int (*builtin_func[]) (char **) = {&csh_cd, &csh_help, &csh_exit};

int csh_numOfBuiltins() {
    return sizeof(builtin_str) / sizeof (char*);
}


char* csh_read_lines(void) {
    char* line = NULL;
    ssize_t buffer_size = 0; // make getline allocate some memory

    if (getline(&line, &buffer_size, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        }
        else {
            perror("readline error");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char** csh_split_lines(char *line) {
    int buffer_size = csh_TOK_BUFSIZE, position=0;
    char** tokens = malloc(buffer_size * sizeof(char*)); //array of pointers
    char *token;

    if (!tokens) {
        fprintf(stderr, "Memory allocation failed !");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, csh_TOK_DELIM);
    while (token!=NULL) {
        tokens[position] = token;
        position++;

        if (position >buffer_size) {
            buffer_size+=csh_TOK_BUFSIZE;
            tokens = realloc(tokens, buffer_size*sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "Memory allocation failed");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, csh_TOK_DELIM); //repeats until no token is left
    }
    tokens[position] = NULL;
    return tokens;

}

int csh_launch(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid==0) {
        if (execvp(args[0],args)==-1) { //child process
            perror("Child error");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid<0) {
        perror("Fork error"); // forking error
    }
    else { //Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int csh_exec(char **args) {
    if (args[0] == NULL) {
        return 1;
    }
    for (int i =0; i<csh_numOfBuiltins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return csh_launch(args);
}

void csh_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("@ ");
        line = csh_read_lines();
        args = csh_split_lines(line);
        status = csh_exec(args);

        free(line);
        free(args);
    }while (status != 0);
}


int main(int argc, char **argv){

    //1.load config files

    // run loop
    csh_loop();

    //3. do shutdown/cleanup commands

    return EXIT_SUCCESS;
}

int csh_cd(char **args) {
    if (args[1]==NULL) {
        fprintf(stderr, "Expected an argument to cd");
    }
    else {
        if (chdir(args[1])!=0) {
            perror("csh");
        }
    }
    return 1;
}
int csh_help(char **args) {
    printf("Welcome to Raj's first custom shell !");
    printf("The following commands are available to use :D");

    for (int i=0; i<csh_numOfBuiltins(); i++) {
        printf("%s \n",builtin_str[i]);
    }

    return 1;
}
int csh_exit(char **args) {
    return 0;
}