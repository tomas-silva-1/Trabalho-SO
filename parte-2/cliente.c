#include "Consulta.h"

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
    FILE* file = fopen( "PedidoConsulta.txt", "a");
    fprintf( file, "%d;%s;%d\n", c.tipo, c.descricao, c.pid_consulta );
    fclose(file);
}
int main(int argc, char const *argv[]){
Consulta cons = getInfo();
criaConsulta(cons);
printf("%d   %s  %d \n",cons.tipo,cons.descricao,cons.pid_consulta);
}


