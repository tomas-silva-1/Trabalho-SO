#!/bin/bash
if [ -e medicos.txt ] ;then
    if cat medicos.txt | awk -F ';' '{ if( $5 < 5 && $6 > 6 ) print}' > lista_negra_medicos.txt ; then
         m=$( cat lista_negra_medicos.txt | wc -l )
         echo "Lista negra de médicos criada com $m médicos"
         cat lista_negra_medicos.txt
    fi
else 
        echo "Não existem médicos registados"
    fi
