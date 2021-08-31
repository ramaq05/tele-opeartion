make

#Step 1: Data Generation
seedlist=(500)
Latency=(300 350 400 450 500)

for seed in ${seedlist[@]}
do
	for n in ${Latency[@]}
	do
		echo "Simulation " $n $seed
		./rdsmp.exe $n $seed > outputdat.$n.$seed
	done
done
#Step 2: Data Analysis      
echo "Start Analysis"

cat out* | grep drivers | awk '{print $2, $3, $5}' > drivers.dat
cat out* | grep drivers | awk '{print $2, $4, $6}' > distance.dat

#Step 3: Data Representation 
echo "Data Representation"
cat graph.gnu | gnuplot
cat graaph.gnu | gnuplot

ls -l *.pdf
