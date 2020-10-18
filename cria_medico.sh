if [ -e ./medicos.txt ]
    then
    if  ! grep "$1;$2;$3;$4;0;0;0" medicos.txt 
        then
        echo "$1;$2;$3;$4;0;0;0" >> medicos.txt
    fi 
else
    echo "$1;$2;$3;$4;0;0;0" > medicos.txt
fi