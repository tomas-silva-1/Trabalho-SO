#include "Consulta.h"

int n=1;
int shmId;
int shmId2;
int msgId;
int semId;

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
    //if(checkSHM1()){
        shmId = shmget( IPC_KEY, LISTA_SIZE*sizeof(Consulta) , IPC_CREAT | PERM );
            exit_on_error(shmId, "shmget");
        Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
            exit_on_null(c, "shmat");
        initializer(c);
   // }
   // if(checkSHM2()){
        shmId2= shmget( IPC_KEY2, sizeof(Contador) , IPC_CREAT | PERM );
            exit_on_error(shmId2, "shmget");
        Contador* tipo=(Contador*) shmat(shmId2, NULL, 0);
            exit_on_null(tipo, "shmat");
            tipo->tipo1=0;
            tipo->tipo2=0;
            tipo->tipo3=0;
            tipo->perdidas=0;
 //   }
    
    printf("Foi iniciada a SHM\n");
}
void startSEM(){
    semId = semget(SEMKEY, 2, IPC_CREAT | PERM );
    exit_on_error(semId, "algo correu mal com o semget");
    
    int status = semctl(semId, 0, SETVAL, 1);
    exit_on_error(status, "semctl SETVAL failed");
}
void upSEM(){
    int res = semop(semId, &UP, 1);
    exit_on_error(res, "erro no UP");
}
void downSEM(){
    int res = semop(semId, &DOWN, 1);
    exit_on_error(res, "erro no DOWN");
}
void startMSGQ(){
    msgId = msgget( MSGKEY, PERM | IPC_CREAT );
        exit_on_error(msgId, "Erro no msgget");
}
void counter(int tipo){
    downSEM();
    Contador* memoria=(Contador*) shmat(shmId2, NULL, 0);
    switch (tipo){
    case 1:
        memoria->tipo1+=1;
        break;
    case 2:
        memoria->tipo2+=1;
        break;
    case 3:
        memoria->tipo3+=1;
        break;
    case 4:
        memoria->perdidas+=1;
        break;
    default:
        printf("tipo de consulta nao valida\n");
        break;
    }
    upSEM();
}
int getSala(){
    downSEM();
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    for(int i=0;i<LISTA_SIZE;i++){
        if(c[i].tipo==-1){
            upSEM();
            return i;
        }
    }
        upSEM();
    return -1;
}
void enviaMensagem(Consulta cons){
    int status;
    mensagem m;
    msgId = msgget( MSGKEY, 0 );
        exit_on_error(msgId, "Erro no msgget.");
    m.tipo= cons.pid_consulta;
    m.consulta=cons;
    status = msgsnd(msgId, &m, sizeof(m.consulta), 0);
        exit_on_error(status, "erro ao enviar");
}
void recusaConsulta(Consulta cons){
    cons.status=4;
    enviaMensagem(cons);
    counter(cons.status);
}
void doConsulta(Consulta cons, int sala){
    downSEM();
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    c[sala]=cons;
    upSEM();
    printf("Consulta agendada para a sala %d\n",sala);
    counter(cons.tipo);
    cons.status=2;
    enviaMensagem(cons);
}
void cleanSala(int sala){
    downSEM();
    Consulta cons;
    cons.tipo=-1;
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    c[sala]=cons;
    upSEM();
}
int iniciaConsulta(Consulta cons){
    int temp=0;
    int status;
    mensagem m;
    pid_t child = fork();
    if ( child == 0 ) {
        int sala=getSala();
        if(sala==-1){
            printf("Lista de consultas cheia\n");
            recusaConsulta(cons);
        }else{
            doConsulta(cons,sala);
            while(temp<DURACAO){
                status = msgrcv(msgId, &m, sizeof(m.consulta),cons.pid_consulta, IPC_NOWAIT);
                    if(status>0){
                        printf("Consulta cancelada pelo utilizador %d\n",m.consulta.pid_consulta);
                        exit(0);  
                }
                sleep(1);
                temp++;
            }            
            cleanSala(sala);
            printf("Consulta terminada na sala %d\n",sala);
            cons.status=3;
            enviaMensagem(cons);
            exit(0);
        }
    }
}
void receberConsulta(Consulta cons){
    printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",cons.tipo,cons.descricao,cons.pid_consulta);
    iniciaConsulta(cons);
}
void trata_sinalINT(int sinal){
    downSEM();
    Contador* memoria=(Contador*) shmat(shmId2, NULL, 0);
    printf("\nNº de Consultas Normais: %d\n",memoria->tipo1);
    printf("Nº de Consultas COVID19: %d\n",memoria->tipo2);
    printf("Nº de Consultas Urgente: %d\n",memoria->tipo3);
    printf("Nº de Consultas Perdidas: %d\n",memoria->perdidas);
    upSEM();
    printf("Servidor a terminar...\n");
    sleep(1);
    exit(0);
}


int main(int argc, char const *argv[]){
    int status;
    printf("Servidor Cliniq-IUL...\n");
    startMSGQ();
    startSEM();
    iniciaSHM();
    mensagem m;
    signal(SIGINT,trata_sinalINT);
    while (n==1){
        status = msgrcv(msgId, &m, sizeof(m.consulta), MSGTYP1, 0);
        receberConsulta(m.consulta);
        
        //pause();
    }
}