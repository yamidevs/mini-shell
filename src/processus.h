#ifndef PROCESSUS_H
#define PROCESSUS_H
#define MAX_CMD 100
#define MAX_DATA_FILE 10000
#define MAX_PATH_SIZE 1024
#define MAX_ARGS 128
#define MAX_ENV 256
#define MAX_TOKEN_STRE 1024
#include <stdlib.h>


typedef struct processus_t{
char path[MAX_PATH_SIZE];
char * argv[MAX_ARGS];
char * envp[MAX_ENV];
int status;
int  stdin ,  stdout,  stderr;
int background;
pid_t pid;
struct processus_t *  pipe;
struct processus_t *  next;
struct processus_t *  next_success;
struct processus_t *  next_fail;
int fd[2];
}processus_t;

int exec_process(processus_t* process);
int exec_cmdline(processus_t* process);
int call_process(processus_t* process);	
void exec_static_cmd(processus_t* process);
void init_commands(processus_t * cmd );


#endif