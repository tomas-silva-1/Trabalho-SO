if [ -e pacientes.txt ]
    then
    awk -F ';' '{ print $3}' pacientes.txt | grep $1 | wc -l
fi
if [ -e medicos.txt ] 
    then
    cat medicos.txt | awk -F ';' '{ if( $7 >= '$2' ) print $7}' | wc -l    
fi   