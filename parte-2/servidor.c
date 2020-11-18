#include "Consulta.h"

const int lista_size =10;
Consulta lista_consultas[10];

void initializer(){
    for(int i=0;i<lista_size;i++){
        Consulta c;
        c.tipo=-1;
        lista_consultas[i]=c;
    }
}
void registerPID(){
    int pid=getpid();
    if(access("SrvConsultas.pid")==0) remove("SrvConsultas.pid");
    FILE* file = fopen( "SrvConsultas.pid", "a");
    fprintf( file, "%d", pid);
    fclose(file);
}
int getNumC(Consulta c, int tipo){
    int result;
    if (tipo ==1 || tipo==2 || tipo==3 ){
        result= calculateCons(c,tipo);
        return result;
    }else{
        fprintf(stderr,"tipo de consulta nao valido\n");
        exit -1;
    }   
}
int calculateCons(Consulta c, int tipo){
    int total=0;
    for(int i=0;i<lista_size;i++){
        if(lista_consultas[i].tipo == tipo) total++;
    }
       return total;
}
int getPerdidas(){
    int total=0;
    for(int i=0;i<lista_size;i++){
        if(lista_consultas[i].tipo == 0) total++;
    }
    return total;
}
void readPedidoCons(){
    int tipo;
    int pid;
    char inf[100];
    FILE* file = fopen( "PedidoConsulta.txt", "r");
    fscanf(file,"%d",&tipo);
    fscanf(file,"%s",&inf);
    fscanf(file,"%d",&pid);
    printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d");
    fclose(file);
}
int n=0;

void trata_sinal(int sinal){
    printf("deu1");
    //readPedidoCons();
    
}


int main(int argc, char const *argv[]){
    initializer();
    registerPID();
    signal(SIGUSR1,trata_sinal);
    printf("Servidor Cliniq-IUL...");    
    while(n==0){
       // signal (SIGUSR1, trata_sinal);
        pause();
    }
    printf("deu2");
    /* Consulta c ;
    c.tipo=1;
    c.pid_consulta=2222;
    lista_consultas[0]=c;
    Consulta c1 ;
    c1.tipo=0;
    c1.pid_consulta=2221;
    lista_consultas[1]=c1;
    printf("%d,%d\n",getPerdidas(),getNumC(c,1)); */

}