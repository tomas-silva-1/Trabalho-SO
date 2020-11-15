#!/bin/bash
if [ $# == 4 ] ; then 
    if [ -e ./medicos.txt ] ; then
        if   echo $1 | grep "[0-9]" ; then
            echo "Não inseriu um nome válido"
        elif  echo $2 | grep "[a-z]" ; then
            echo "Não inseriu um número válido"
        elif  echo $3 | grep "[0-9]" ; then
            echo "Não inseriu uma especialidade válida"
        elif  echo $4 | grep "[a-z0-9]@[a-z0-9]*\.[a-z]" ; then
            if  [ 1 != $(grep -c "^$2$" medicos.txt) ] ; then
                echo "$1;$2;$3;$4;0;0;0" >> medicos.txt
                echo "  Lista de médicos: "
                cat medicos.txt         
            else 
                echo "Esse médico já existe"
            fi 
        else
            echo "Não inseriu um e-mail válido"
        fi
    else
    echo "$1;$2;$3;$4;0;0;0" > medicos.txt
    echo "  Lista de médicos: "
    cat medicos.txt
    fi
else 
echo "Não inseriu o número correto de argumentos"
fi