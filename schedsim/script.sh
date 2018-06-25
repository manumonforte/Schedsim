#/!bin/bash

#comprobacion carpeta resultados
if [ -d resultados  ]
then
	rm -r resultados
fi
mkdir resultados

#comprobacion fichero
echo "Introducir ruta fichero de ejemplo: "
read fichero

while [ ! -f $fichero ]; do
	echo "El fichero introducido no es valido, Introduzca otro fichero: "
	read fichero
done

#comprobacion maxCPUs
echo "Introducir maxCPUs: "
read maxCPUs

while [ $maxCPUs -lt 1 ] && [ $maxCPUs -gt 4 ]; do
	echo "El valor de maxCPUs no es valido, Introduzca otro valor: "
	read maxCPUs
done

echo "CPUS: "$maxCPUs" para el fichero--> "$fichero

#recorremos array de planificadores
listaDeSchedulersDisponibles=("FCFS" "PRIO" "RR" "SJF" "MULTIRR")
for nameSched in "${listaDeSchedulersDisponibles[@]}"; do
	for ((cpus = 1; $cpus <= $maxCPUs; cpus++)) do
	#ejecutamos  el simulador de la planificacion
	./schedsim -i "$fichero" -n "$cpus" -s "$nameSched" 
		#movemos los resultados a la carpeta de resultados		
		for ((i=0;$i<$cpus;i++)) do
			mv CPU_$i.log resultados/"$nameSched"-CPU-$i.log
		done
	done

	#generamos la grafica
	cd ../gantt-gplot

	for ((i=0;$i<$maxCPUs;i++)) do
		../gantt-gplot/generate_gantt_chart ../schedsim/resultados/"$nameSched"-CPU-"$i".log
	done
	cd ../schedsim
done
