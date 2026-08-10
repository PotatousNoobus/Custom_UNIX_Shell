#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define goth_TOK_BUFSIZE 64
#define goth_TOK_DELIM " \t\r\n\a"

char* goth_read_lines(void) {
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

char** goth_split_lines(char *line) {
    int buffer_size = goth_TOK_BUFSIZE, position=0;
    char** tokens = malloc(buffer_size * sizeof(char*)); //array of pointers
    char *token;

    if (!tokens) {
        fprintf(stderr, "Memory allocation failed !");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, goth_TOK_DELIM);
    while (token!=NULL) {
        tokens[position] = token;
        position++;

        if (position >buffer_size) {
            buffer_size+=goth_TOK_BUFSIZE;
            tokens = realloc(tokens, buffer_size*sizeof(char*));

            if (!tokens) {
                fprintf(stderr, "Memory allocation failed");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, goth_TOK_DELIM); //repeats until no token is left
    }
    tokens[position] = NULL;
    return tokens;

}

int goth_exec(char **args) {

}

void goth_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("@ ");
        line = goth_read_lines();
        args = goth_split_lines(line);
        status = goth_exec(args);

        free(line);
        free(args);
    }while (status != 0);
}
int goth_launch(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid==0) {
        if (execvp(args[0],args)==-1) { //child process
            perror("goth");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid<0) {
        perror("goth"); // forking error
    }
    else { //Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int main(int argc, char **argv){

    //1.load config files

    // run loop
    goth_loop();

    //3. do shutdown/cleanup commands

    return EXIT_SUCCESS;
}
