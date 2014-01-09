rm -f data.dat

for i in {1..32}
do
	make N=$i
done

gnuplot plotter.plot
gnuplot plotterlog.plot
