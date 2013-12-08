for i in {1..27}
do
	make N=$i
done

gnuplot plotter.plot
gnuplot plotterlog.plot

rm -f data.dat
