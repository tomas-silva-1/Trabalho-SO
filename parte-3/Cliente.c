#include "Consulta.h"

Consulta cons;
int n=1;

int tipoConsValido(int t){
    if (t==1 || t==2 || t==3 ) return 1;
    return -1;
}


Consulta getInfo(){
    Consulta consulta;
    printf("Indique qual o tipo da consulta dos seguintes:\n");
    printf("1-Normal\n2-COVID19\n3-Urgente\n");
    int t;
    scanf("%d",&t);
    if(tipoConsValido(t)>0){
        consulta.tipo=t;
        printf("Indique a descriçao da consulta:\n");
        scanf("\n%99[^\n]",&consulta.descricao);
        consulta.pid_consulta=getpid();
        consulta.status=1;  // 1-pedido
        return consulta;
    }else{
       fprintf(stderr,"tipo de consulta nao valido\n");
       termina();
       exit (-1); 
    }
}

int main(int argc, char const *argv[]){
    cons = getInfo();

    while (n==1){
        pause();
    }
    
}