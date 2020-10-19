if cat medicos.txt | awk -F ';' '{ if( $5 < 5 && $6 >6 ) print}' > lista_negra_medicos.txt 
    then
    echo"Lista negra criada"
fi