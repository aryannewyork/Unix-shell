#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void read_command(char cmd[], char *params[]){
    char line[1024];
    int count = 0, i = 0, j = 0;
    char *array[100], *parse;
    // read one line
    for(;;){
        int c = fgetc(stdin);
        line[count++] = (char) c;
        if(c == '\n') {
            break;
        }
    }
    if(count == 1) {
        return;
    }
    parse = strtok(line, " \n");

    // parse the line into words

    while(parse != NULL){
        array[i++] = strdup(parse);
        parse = strtok(NULL, " \n");
    }
    // first word is the command
    strcpy(cmd, array[0]);

    // others are parameters
    for(int j = 0; j < i; j++){
        params[j] = array[j];
    }
    // NULL terminated the parameter list
    params[i] = NULL;

}

void input(){
    static int first = 1;
    if(first) {
        // clear screen for the first time
        const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first = 0;
    }
    // display prompt
    printf(">>> ");
}

int main() {
    char cmd[100], command[100], *params[20];
    // Environment Variable
    char *envp[] = {(char *) "PATH=/bin", 0};
    while(1) {
        input();
        read_command(command, params);
        int rc = fork();
        if(-1 == rc) {
            perror("Failed to fork\n");
            exit(0);
        } else if(0 == rc){
            strcpy(cmd, "/bin/");
            strcat(cmd, command);
            execve(cmd, params, envp);
        }
        else {
            wait(NULL);
        }

        if(strcmp(command, "exit") == 0) {
            break;
        }
    }
}