#!/bin/bash
#cat /etc/passwd | awk '{print "<",$1,">;",$3}'
#tail /etc/passwd | cut -d ':' -f1,5
tail /etc/passwd | awk -F ':' ' {print $1";"$5";;;"$1"@iscte-iul.pt;100"}' | sed -E "s/a//" | sed "s/,,,//" > pacientes.txt
#<Nº registo (nº identificação)>;<nome>;<localidade>;<nº telemóvel>;<e-mail>;<saldo>
#echo "<>;<>;<>;<>;<>;<>"