make all

seedlist=(500)
K=(10 11 12 13 14 15 16 17 18 19 20)

# Step 1: Data Generation
echo "Data Generation"
for seed in ${seedlist[@]}
do
	for n in ${K[@]}
	do
		echo "Simulation " $n $seed
		./randm $n $seed > rsoutputdat.$n.$seed
		./sa $n $seed > saoutputdata.$n.$seed
	done
done

#Step 2: Data Analysis
echo "Start Analysis"
cat rsout* | grep drivers | awk '{print $2, $3}' > rs.dat
cat saout* | grep drivers | awk '{print $2, $3}' > sa.dat
#cat rdsmpout* | grep drivers | awk '{print $}'

#Step 3: Data Representation
echo "Data Representation"
cat graph.gnu | gnuplot
ls -l *.pdf
