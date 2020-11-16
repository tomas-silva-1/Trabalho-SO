#include "Consulta.h"

Consulta getinfo(){
    Consulta consulta;
    printf("Indique o tipo da consulta:\n");
    scanf("%d",&consulta.tipo);
    printf("Indique a descriçao da consulta:\n");
    scanf("\n%99[^\n]",&consulta.descricao);
    consulta.pid_consulta=getpid();
    return consulta;
}

int main(int argc, char const *argv[]){
Consulta cons = getinfo();
printf("%d   %s  %d \n",cons.tipo,cons.descricao,cons.pid_consulta);
}


