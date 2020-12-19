#include "Consulta.h"

int n=1;
int shmId;
int shmId2;
int msgId;
int semId;

void initializer(Consulta* c){
    for(int i=0;i<LISTA_SIZE;i++){
        c[i].tipo=-1;
    }
}
int checkSHM1(){        //verifica se existe shm1
    int temp =  shmget( IPC_KEY, LISTA_SIZE*sizeof(Consulta) , 0 | PERM );
     if(temp>0){
         return temp;
     } 
        return 0;
}
int checkSHM2(){        //verifica se existe shm2
    int temp =  shmget( IPC_KEY2, sizeof(Contador) , 0 | PERM );
    if(temp>0) {
        printf(" existe shm2\n");
        return temp;
    }
        return 0;
}
void iniciaSHM(){       //inicia shm1 e shm2 se nao existirem
    int temp1=checkSHM1();
    int temp2= checkSHM2();
    if(temp1==0){
        shmId = shmget( IPC_KEY, LISTA_SIZE*sizeof(Consulta) , IPC_CREAT | PERM );      
            exit_on_error(shmId, "shmget");
        Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
            exit_on_null(c, "shmat");
        initializer(c);
    }
    else shmId=temp1;

    if(temp2==0){
        shmId2= shmget( IPC_KEY2, sizeof(Contador) , IPC_CREAT | PERM );
            exit_on_error(shmId2, "shmget");
        Contador* tipo=(Contador*) shmat(shmId2, NULL, 0);
            exit_on_null(tipo, "shmat");
            tipo->tipo1=0;
            tipo->tipo2=0;
            tipo->tipo3=0;
            tipo->perdidas=0;
    }
    else shmId2= temp2;
    
    printf("Foi iniciada a SHM\n");
}
void startSEM(){            // inicia semaforo
    semId = semget(SEMKEY, 2, IPC_CREAT | PERM );
    exit_on_error(semId, "algo correu mal com o semget");
    
    int status = semctl(semId, 0, SETVAL, 1);
    exit_on_error(status, "semctl SETVAL failed");
}
void upSEM(){ //sobe valor do semaforo
    int res = semop(semId, &UP, 1);
    exit_on_error(res, "erro no UP");
}
void downSEM(){     //desce semaforo
    int res = semop(semId, &DOWN, 1);
    exit_on_error(res, "erro no DOWN");
}
void startMSGQ(){  //inicia messagequeue
    msgId = msgget( MSGKEY, PERM | IPC_CREAT );
        exit_on_error(msgId, "Erro no msgget");
}
void counter(int tipo_status){     //conta o tipo das consultas
    downSEM();
    Contador* memoria=(Contador*) shmat(shmId2, NULL, 0);
    switch (tipo_status){
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
int getSala(){      // vai buscar a primeira sala vazia da shm1
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
void enviaMensagem(Consulta cons){      //envia mensagem ao cliente
    int status;
    mensagem m;
    msgId = msgget( MSGKEY, 0 );
        exit_on_error(msgId, "Erro no msgget.");
    m.tipo= cons.pid_consulta;
    m.consulta=cons;
    status = msgsnd(msgId, &m, sizeof(m.consulta), 0);
        exit_on_error(status, "erro ao enviar");
}
void recusaConsulta(Consulta cons){     //recusa a consulta se não houver vaga
    cons.status=4;
    enviaMensagem(cons);
    counter(cons.status);
}
void doConsulta(Consulta cons, int sala){ //coloca a consulta na sala
    downSEM();
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    c[sala]=cons;
    upSEM();
    printf("Consulta agendada para a sala %d\n",sala);
    counter(cons.tipo);
    cons.status=2;
    enviaMensagem(cons);
}
void cleanSala(int sala){ // limpa a sala(tira a consulta)
    Consulta cons;
    cons.tipo=-1;
    downSEM();
    Consulta* c= (Consulta*) shmat(shmId, NULL, 0);
    c[sala]=cons;
    upSEM();
}
int iniciaConsulta(Consulta cons){ // inicializa a consulta
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
            while(temp<DURACAO){  //3.3.3 iterador corre ate 10 e cada iteraçao chama o megrcv com o IPC_NOWAIT o que nao bloqueia o processo e a seguir um sleep de 1s
                status = msgrcv(msgId, &m, sizeof(m.consulta),cons.pid_consulta, IPC_NOWAIT);
                    if(status>0){
                        if(m.consulta.status==5){
                            printf("Consulta cancelada pelo utilizador %d\n",m.consulta.pid_consulta);
                            cleanSala(sala);
                            exit(0);
                        }else{
                            printf("Consulta não cancelada com sucesso %d\n",m.consulta.pid_consulta);
                        }                       
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
    else signal (SIGCHLD, SIG_IGN);//mata zombies
}
void receberConsulta(Consulta cons){        //trata da inicializaçao da consulta
    printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",cons.tipo,cons.descricao,cons.pid_consulta);
    iniciaConsulta(cons);
}
void trata_sinalINT(int sinal){         //trata do sinal e imprime as estatisticas
    Contador* memoria=(Contador*) shmat(shmId2, NULL, 0);
    printf("\nNº de Consultas Normais: %d\n",memoria->tipo1);
    printf("Nº de Consultas COVID19: %d\n",memoria->tipo2);
    printf("Nº de Consultas Urgente: %d\n",memoria->tipo3);
    printf("Nº de Consultas Perdidas: %d\n",memoria->perdidas);
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
    }
}