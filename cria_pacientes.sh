cat /etc/passwd | awk -F ':' '/^a[0-9]/ { print $1";"$5";;;"$1"@iscte-iul.pt;100"}' | head | sed -E "s/a//" | sed "s/,,,//" > pacientes.txt
