cat /etc/passwd | awk -F ':' '/^a[0-9]/ { gsub(/\,,,/, "", $5);  print $1";"$5";;;"$1"@iscte-iul.pt;100"}' | head | sed -E "s/a//" > pacientes.txt
# | sed "s/,,,//"