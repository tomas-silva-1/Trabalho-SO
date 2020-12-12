#include "Consulta.h"

int n=1;
int shmId;
int shmId2;
int msgId;

Consulta lista_consultas[LISTA_SIZE];   // atençao à marcaçao das consultas nas salas e aos contadores

void initializer(Consulta* c){
    for(int i=0;i<LISTA_SIZE;i++){
        c[i].tipo=-1;
    }
}
int checkSHM1(){
    int temp =  shmget( IPC_KEY, LISTA_SIZE*sizeof(Consulta) , IPC_CREAT | IPC_EXCL | PERM );
     if(temp>0) return 0;
        return 1;
}
int checkSHM2(){
    int temp =  shmget( IPC_KEY2, 4*sizeof(n) , IPC_CREAT | IPC_EXCL | PERM );
    if(temp>0) return 0;
        return 1;
}
void iniciaSHM(){
    if(checkSHM1()){
        shmId = shmget( IPC_KEY, LISTA_SIZE*sizeof(Consulta) , IPC_CREAT | PERM );
            exit_on_error(shmId, "shmget");
        Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
            exit_on_null(c, "shmat");
        initializer(c);
    }
    if(checkSHM2()){
        shmId2= shmget( IPC_KEY2, sizeof(Contador) , IPC_CREAT | PERM );
            exit_on_error(shmId2, "shmget");
        Contador* tipo=(Contador*) shmat(shmId2, NULL, 0);
            exit_on_null(tipo, "shmat");
            tipo->tipo1=0;
            tipo->tipo2=0;
            tipo->tipo3=0;
            tipo->perdidas=0;
    }
    
    printf("Foi iniciada a SHM\n");
    }
void startMSGQ(){
    msgId = msgget( MSGKEY, PERM | IPC_CREAT );
        exit_on_error(msgId, "Erro no msgget");
    }
void counter(int tipo){
    Contador* memoria=(Contador*) shmat(shmId2, NULL, 0);
    switch (tipo){
    case 1:
        memoria.tipo1++;
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
int getSala(){
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    for(int i=0;i<LISTA_SIZE;i++){
        if(c[i].tipo==-1){
            return i;
        }
    }
    return -1;
}
void enviaMensagem(Consulta cons){

}
void recusaConsulta(Consulta cons){
    cons.status=4;
    enviaMensagem(cons);
    counter(cons.status);
}
int iniciaConsulta(Consulta cons){
    pid_t child = fork();
    if ( child == 0 ) {
        int sala=getSala();
        if(sala==-1){
            printf("Lista de consultas cheia");
            recusaConsulta(cons);
        }else{

        }
    }
    }
void receberConsulta(Consulta cons){
    printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",cons.tipo,cons.descricao,cons.pid_consulta);
    iniciaConsulta(cons);

    }



int main(int argc, char const *argv[]){
    int status;
    printf("Servidor Cliniq-IUL...\n");
    startMSGQ();
    iniciaSHM();
    mensagem m;
    while (n==1){
        status = msgrcv(msgId, &m, sizeof(m.consulta), MSGTYP1, 0);
        receberConsulta(m.consulta);
        
        //pause();
    }
}