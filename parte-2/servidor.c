#include <unistd.h>
#include "Consulta.h"


Consulta lista_consultas[LISTA_SIZE];
int tipo1=0;
int tipo2=0;
int tipo3=0;
int perdidas=0;
int n=0;


void initializer(){
    for(int i=0;i<LISTA_SIZE;i++){
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
void counter(int tipo){
    switch (tipo){
    case 1:
        tipo1++;
        break;
    case 2:
        tipo2++;
        break;
    case 3:
        tipo3++;
        break;
    default:
        printf("tipo de consulta nao valida");
        break;
    }
}
int vagas(){
    int vagas=0;
    for(int i=0;i<LISTA_SIZE;i++){
    if(lista_consultas[i].tipo == -1) vagas++;
    } 
    return vagas;
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

void doConsulta(Consulta c, int sala){
    pid_t child = fork();
     if(child == 0){  //filho
        kill(c.pid_consulta,SIGHUP);
        sleep(TIME); //TIME
        printf("Consulta terminada na sala %d\n",sala);
        kill(c.pid_consulta,SIGTERM);
        kill(getpid(),SIGKILL);
     }else{  //pai
         wait(NULL);
         Consulta cf;
         cf.tipo=-1;
         lista_consultas[sala]=cf;
     }
}


void setupConsulta(Consulta c){
    for(int i=0;i<LISTA_SIZE;i++){
        if(lista_consultas[i].tipo == -1){ 
            lista_consultas[i]=c;
            printf("Consulta agendada para a sala %d\n",i);
            counter(c.tipo);
            doConsulta(c,i);
            break;
        }
    }
}

void readPedidoCons(){
    int tipo;
    int pid;
    char inf[100];
    Consulta c;
    FILE* file = fopen( "PedidoConsulta.txt", "r");
    fscanf(file,"%d %s %d",&tipo,&inf,&pid);
    if(checksVagas(pid)){
         printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",tipo,inf,pid);
        c.tipo=tipo;
        c.pid_consulta=pid;
        *c.descricao=*inf;

        setupConsulta(c);
    }
}
void statsConsultas(){
    FILE* file= fopen("StatsConsultas.dat","w");
    if ( file ) {
        fwrite(&perdidas, sizeof(perdidas), 1, file);
        fwrite(&tipo1,sizeof(tipo1),1,file);
        fwrite(&tipo2,sizeof(tipo2),1,file);
        fwrite(&tipo3,sizeof(tipo3),1,file);
        fclose(file);       //hexdump -e '4 "%i\t" "\n"' StatsConsultas.dat
    } else {
        fprintf(stderr,"Erro, não foi possível abrir o ficheiro 'StatsConsultas.dat' para escrita.\n");
    }

}
void trata_sinal(int sinal){
    readPedidoCons();   
}
void trata_sinalINT(int sinal){
    remove("SrvConsultas.pid");
    statsConsultas();
    n=1;

}

int main(int argc, char const *argv[]){
    initializer();
    registerPID();
    signal(SIGUSR1,trata_sinal);
    signal(SIGINT,trata_sinalINT);
    printf("Servidor Cliniq-IUL...\n");
    while(n==0){
        pause();
    }

}