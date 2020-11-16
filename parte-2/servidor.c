#include "Consulta.h"

Consulta lista_consultas[10];
void initializer(){
    for(int i=0;i<lista_consultas;i++){
        Consulta c;
        c.tipo=-1;
        lista_consultas[i]=c;
    }
}
int getNumberCon1(Consulta c){
    int total;
    for(int i=0;i<lista_consultas;i++){
        if(lista_consultas[i].tipo == 1) total++;
    }
    return total;
}
int getNumberCon2(Consulta c){
    int total;
    for(int i=0;i<lista_consultas;i++){
        if(lista_consultas[i].tipo == 2) total++;
    }
    return total;
}
int getNumberCon3(Consulta c){
    int total;
    for(int i=0;i<lista_consultas;i++){
        if(lista_consultas[i].tipo == 3) total++;
    }
    return total;
}
int getPerdidas(Consulta c){
    int total;
    for(int i=0;i<lista_consultas;i++){
        if(lista_consultas[i].tipo == 0) total++;
    }
    return total;
}

int main(int argc, char const *argv[]){

}