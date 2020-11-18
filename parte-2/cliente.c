#include "Consulta.h"


Consulta cons;

int tipoConsValido(int t){
    if (t==1 || t==2 || t==3 ) return 1;
    return -1;
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
       exit -1; 
    }
}
void criaConsulta(Consulta c){
    if(remove("PedidoConsulta.txt")==0);
    FILE* file = fopen( "PedidoConsulta.txt", "a");
    fprintf( file, "%d %s %d", c.tipo, c.descricao, c.pid_consulta );
    fclose(file);
}
int getSrvPid(){
    int pid;
        FILE* file = fopen( "SrvConsultas.pid", "r");
        fscanf(file,"%d",&pid);
        fclose(file);
        return pid;
}
void sendsignal(){
    int pid=getSrvPid();
    kill(pid,SIGUSR1);
    
}
void trata_sinal(int sinal){
    printf("Consulta não é possível para o processo %d\n",cons.pid_consulta);
    kill(cons.pid_consulta,SIGKILL);
}

int main(int argc, char const *argv[]){
    int n=0;
cons = getInfo();
signal(SIGUSR2,trata_sinal);
criaConsulta(cons);
printf("%d   %s  %d \n",cons.tipo,cons.descricao,cons.pid_consulta);
sendsignal();
while(n==0){
    pause();
}
}


