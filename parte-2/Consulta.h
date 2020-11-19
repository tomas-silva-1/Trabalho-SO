#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#ifndef __CONSULTA_H__
#define __CONSULTA_H__

#define TIME 10
#define LISTA_SIZE 10

typedef struct {
 int tipo; 
 char descricao[100]; 
 int pid_consulta; 
} Consulta;

#endif