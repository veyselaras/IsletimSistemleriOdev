#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// Arka plan süreçlerini takip için bir yapı
typedef struct arkaplanProcess {
    pid_t pid;
    struct arkaplanProcess* sonraki;
} arkaplanProcess_t;

// Global Degiskenler
arkaplanProcess_t* arkaplanListesi = NULL;
int quit_requested = 0;

// Function declarations
void sigchld_handler(int);
void arkaPlanProcessEkle(pid_t);
void arkplanBekle();
int parse_command(char* satir, char** args, char** girisDosyasi, char** cikisDosyasi, int* arkaplan, char*** pipeKomutları, int* pipeSayac);
int execute_increment(const char*input_file) ;
int execute_command(char**args, char*girisDosyasi, char*cikisDosyasi, int);
int execute_pipe_commands(char** pipeCmd, int, char* globalGirisDosyasi, char* globalCikisDosyasi);
void execute_sequential_commands(char* line);

#endif