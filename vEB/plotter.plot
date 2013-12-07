set term png
set output "plot.png" 

set xlabel "N"
set ylabel "durchschnittliche Suchdauer in µs"
#set log y

plot "data.dat" using 1:2 title "sortiert" with linespoints, \
	"data.dat" using 1:3 title "zufaellig" with linespoints, \
	"data.dat" using 1:4 title "level-weise sortiert" with linespoints, \
	"data.dat" using 1:5 title "van Emde Boas" with linespoints

