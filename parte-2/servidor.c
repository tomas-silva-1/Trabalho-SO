#include <unistd.h>
#include "Consulta.h"

const int lista_size =10;
Consulta lista_consultas[10];
int tipo1=0;
int tipo2=0;
int tipo3=0;
int perdidas=0;


void initializer(){
    for(int i=0;i<lista_size;i++){
        Consulta c;
        c.tipo=-1;
        lista_consultas[i]=c;
    }
}
void registerPID(){
    int pid=getpid();
   if(remove("SrvConsultas.pid")==0);
    FILE* file = fopen( "SrvConsultas.pid", "a");
    fprintf( file, "%d", pid);
    fclose(file);
}
/* int getNumC(Consulta c, int tipo){
    int result;
    if (tipo ==1 || tipo==2 || tipo==3 ){
        result= calculateCons(c,tipo);
        return result;
    }else{
        fprintf(stderr,"tipo de consulta nao valido\n");
        exit -1;
    }   
} */
int calculateCons(Consulta c, int tipo){
    int total=0;
    for(int i=0;i<lista_size;i++){
        if(lista_consultas[i].tipo == tipo) total++;
    }
       return total;
}

void readPedidoCons(){
    int tipo;
    int pid;
    char inf[100];
    FILE* file = fopen( "PedidoConsulta.txt", "r");
    fscanf(file,"%d %s %d",&tipo,&inf,&pid);
    if(checksVagas(pid))
         printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",tipo,inf,pid);
    
}
int checksVagas(int pid){
    if(vagas()==0){
        printf("Lista de consultas cheia\n");
        kill(pid,SIGUSR2);
        perdidas++;
        return 0;
    }
    return 1;
}
int vagas(){
    int vagas=0;
    for(int i=0;i<lista_size;i++){
    if(lista_consultas[i].tipo==-1) vagas++;
    } 
    return vagas;
}


void trata_sinal(int sinal){
    readPedidoCons();   
}

    void test(){
        for(int i=0;i<lista_size;i++){
        Consulta c;
        c.tipo=1;
        lista_consultas[i]=c;
    }
    }
int main(int argc, char const *argv[]){
    int n=0;
    //initializer();
    test();
    registerPID();
    signal(SIGUSR1,trata_sinal);
    printf("Servidor Cliniq-IUL...\n");
        
    while(n==0){
        pause();
    }
    printf("deu2\n");
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