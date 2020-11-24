#include "Consulta.h"


Consulta cons;
int n=0;
int secure=0;

int tipoConsValido(int t){
    if (t==1 || t==2 || t==3 ) return 1;
    return -1;
}

void termina(){
    printf("Terminando processo...\n");
    remove(P_CONSULTAS);
    sleep(1);
    n=1;
}
Consulta getInfo(){
    Consulta consulta;
    printf("Indique qual o tipo da consulta dos seguintes:\n");
    printf("1-Normal\n2-COVID19\n3-Urgente\n");
    int n;
    scanf("%d",&n);
    if(tipoConsValido(n)>0){
        consulta.tipo=n;
        printf("Indique a descriçao da consulta:\n");
        scanf("\n%99[^\n]",&consulta.descricao);
        consulta.pid_consulta=getpid();
        return consulta;
    }else{
       fprintf(stderr,"tipo de consulta nao valido\n");
       termina();
       exit (-1); 
    }
}
void criaConsulta(Consulta c){
    if(!access(P_CONSULTAS,F_OK)){
        printf("Encontra-se uma consulta a decorrer por favor aguarde...\n");
        alarm(TIME);
    }
    FILE* file = fopen( P_CONSULTAS, "a");
    fprintf( file, "%d,%s,%d", c.tipo, c.descricao, c.pid_consulta );
    fclose(file);
}


int getSrvPid(){
    int pid;
    if(!access(SRV_CONSULTAS,F_OK)){
        FILE* file = fopen( SRV_CONSULTAS, "r");
        fscanf(file,"%d",&pid);
        fclose(file);
        return pid;
    }else{
        fprintf(stderr,"Houve um erro no processo, por favor verifique que o servidor foi inicializado, tente outra vez\n");
        termina();
        exit(-1);
    }
        
}
void sendsignal(){
    int pid=getSrvPid();
    kill(pid,SIGUSR1);
    
}

void trata_sinalURS2(int sinal){
    printf("Consulta não é possível para o processo %d\n",cons.pid_consulta);
    termina();
}
void trata_sinalHUP(int sinal){
    printf("Consulta iniciada para o processo %d\n",cons.pid_consulta);
    printf("Consulta a decorrer...\n");
    secure++;
    
}
void trata_sinalTERM(int sinal){
    if(secure==1){
    printf("Consulta concluída para o processo %d\n",cons.pid_consulta);
    termina();
    }else{
        printf("erro durante o processo");
        termina();
        exit(-1);
    }
}
void trata_sinalINT(int sinal){
    printf("\nPaciente cancelou o pedido\n");
    termina();
}
void trata_alarm(int sinal){
    criaConsulta(cons);
}

int main(int argc, char const *argv[]){
    cons = getInfo();
    signal(SIGUSR2,trata_sinalURS2);
    signal(SIGHUP,trata_sinalHUP);
    signal(SIGTERM,trata_sinalTERM);
    signal(SIGINT,trata_sinalINT);
    signal(SIGALRM,trata_alarm);
    criaConsulta(cons);
    sendsignal();
    while(n==0){
        pause();
    }
}


