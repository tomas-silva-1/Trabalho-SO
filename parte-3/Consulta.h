#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define exit_on_error(s,m) if (s<0) {perror(m); exit(1);}
#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }

#define IPC_KEY 0x0a92419
#define IPC_KEY2 0x0b92419
#define MSGKEY 0x0a92419
#define SEMKEY 0x0a92419 
#define MSGTYP1 1
#define MSGTYPC 5
#define DURACAO 10
#define LISTA_SIZE 10
#define PERM 0600


#ifndef __CONSULTA_H__
#define __CONSULTA_H__

typedef struct {
 int tipo; 
 char descricao[100]; 
 int pid_consulta; 
 int status;
} Consulta;

typedef struct{
    int tipo1;
    int tipo2;
    int tipo3;
    int perdidas;
}Contador;

typedef struct {
    long tipo;
    Consulta consulta;
}mensagem;

struct sembuf UP = { 
    .sem_num = 0, 
    .sem_op = +1,
    .sem_flg = 0
};

struct sembuf DOWN = { 
    .sem_num = 0, 
    .sem_op = -1,
    .sem_flg = 0
};



#endif