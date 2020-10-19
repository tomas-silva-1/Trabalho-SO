if cat medicos.txt | awk -F ';' '{ if( $5 < 5 && $6 >6 ) print}' > lista_negra_medicos.txt 
    then
    m=$( cat lista_negra_medicos.txt | wc -l )
    echo "Lista negra de médicos criada com $m médicos"
fi