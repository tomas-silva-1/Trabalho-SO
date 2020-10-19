if [ -e pacientes.txt ]
    then
    grep $1 pacientes.txt | wc -l
fi
if [ -e medicos.txt ] 
    then
    cat medicos.txt | awk -F ';' '{ if( $7 >= '$2' ) print $7}' | wc -l    
fi  