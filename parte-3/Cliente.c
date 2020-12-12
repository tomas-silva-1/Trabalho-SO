#include "Consulta.h"

Consulta cons;
int n=1;
int msgId;


int tipoConsValido(int t){
    if (t==1 || t==2 || t==3 ) return 1;
    return -1;
}
void termina(){
    printf("Terminando processo...\n");
    sleep(1);
    exit(0);
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
void enviaConsulta(Consulta c){
    int status;
    mensagem m;
    msgId = msgget( MSGKEY, 0 );
        exit_on_error(msgId, "Erro no msgget.");
    m.tipo= MSGTYP1;
    m.consulta=c;
    status = msgsnd(msgId, &m, sizeof(m.consulta), 0);
        exit_on_error(status, "erro ao enviar");
}
int main(int argc, char const *argv[]){
    int status;
    cons = getInfo();
    enviaConsulta(cons);
    printf("chega a enviar\n");
    while (n==1){

        pause();
    }
    
}