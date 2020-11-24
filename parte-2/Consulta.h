#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifndef __CONSULTA_H__
#define __CONSULTA_H__

#define TIME 10
#define LISTA_SIZE 10
#define P_CONSULTAS "PedidoConsulta.txt"
#define SRV_CONSULTAS "SrvConsultas.pid"

typedef struct {
 int tipo; 
 char descricao[100]; 
 int pid_consulta; 
} Consulta;

#endif