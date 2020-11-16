#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __CONSULTA_H__
#define __CONSULTA_H__


typedef struct {
 int tipo; 
 char descricao[100]; 
 int pid_consulta; 
} Consulta;

#endif