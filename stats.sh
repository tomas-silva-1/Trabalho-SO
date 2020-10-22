#!/bin/bash
if [ $# == 2 ] ; then
    if [ -e pacientes.txt ] ;
        then
        if echo $1 | grep "[0-9]" ; then
            echo "A localidade que inseriu não é válida"
            else 
            echo "Numero de pacientes de(o) $1:"
            awk -F ';' '{ print $3}' pacientes.txt | grep $1 | wc -l
        fi
    else 
        echo "Não existem pacientes registados"    
    fi
    if [ -e medicos.txt ] ;then
        if echo $2 | grep "[a-z]" ; then
            echo "Não inseriu um número válido" 
            else
            echo "Numero de médicos com saldo superior a $2:"
            cat medicos.txt | awk -F ';' '{ if( $7 >= '$2' ) print $7}' | wc -l
        fi  
    else 
        echo "Não existem médicos registados"
    fi        
else 
    echo "Não inseriu o número correto de argumentos"
 fi     