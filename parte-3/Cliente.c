#include "Consulta.h"

Consulta cons;
int n=1;
int msgId;
int iniciada=0;


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
void enviaConsulta(){
    int status;
    mensagem m;
    msgId = msgget( MSGKEY, 0 );
        exit_on_error(msgId, "Erro no msgget.");
    m.tipo= MSGTYP1;
    m.consulta=cons;
    status = msgsnd(msgId, &m, sizeof(m.consulta), 0);
        exit_on_error(status, "erro ao enviar");
}
void trataMensagens(int status){
    switch (status){
    case 2:
        printf("Consulta iniciada para o processo %d\n", cons.pid_consulta);
        iniciada=1;
        break;
    case 3:
        if(iniciada==1){
            printf("Consulta concluída para o processo %d\n", cons.pid_consulta);
            exit(0);
        }else{
            printf("Erro na conclusão da consulta\n");
            exit(-1);
        }
        break;
    case 4:
        printf("Consulta não é possível para o processo %d\n", cons.pid_consulta);
        exit(0);
        break;
    default:
        printf("status nao valido\n");
        exit(-1);
        break;
    }
}
void cancelaConsulta(){
    int status;
    mensagem m;
    msgId = msgget( MSGKEY, 0 );
        exit_on_error(msgId, "Erro no msgget.");
    m.tipo= cons.pid_consulta;
    m.consulta=cons;
    status = msgsnd(msgId, &m, sizeof(m.consulta), 0);
        exit_on_error(status, "erro ao enviar");
}
void trata_sinalINT(int sinal){
    printf("Paciente cancelou o pedido\n");
    cons.status=5;
    cancelaConsulta();
    exit(0);
}
int main(int argc, char const *argv[]){
    int status;
    mensagem m;
    cons = getInfo();
    enviaConsulta();
    signal(SIGINT,trata_sinalINT);
    while (n==1){
        status = msgrcv(msgId, &m, sizeof(m.consulta), cons.pid_consulta, 0);
        trataMensagens(m.consulta.status);
    }
    
}