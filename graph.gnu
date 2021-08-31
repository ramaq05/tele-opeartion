set term pdf color 

#Different line color style..
set style line 1  lc rgb '#0000FF' lt 1 lw 2.5 pt 1 ps 1.5			#that is Blue color
set style line 2  lc rgb '#8A2BE2' lt 1 lw 1.5 pt 2 ps 1.5 			#that is Blue violet
set style line 3  lc rgb '#006400' lt 1 lw 1.5 pt 3 ps 1.5 			#that is Dark green color
set style line 4  lc rgb '#808000' lt 1 lw 1.5 pt 4 ps 1.5 			#that is Olive color
set style line 5  lc rgb '#8B0000' lt 1 lw 1.5 pt 5 ps 1.5 			#that is dark red color
set style line 6  lc rgb '#FF00FF' lt 1 lw 1.5 pt 6 ps 1.5			#that is magenta color
set style line 7  lc rgb '#FFD700' lt 1 lw 1.5 pt 7 ps 1.5 			#that is Gold color
set style line 8  lc rgb '#DAA520' lt 1 lw 1.5 pt 8 ps 1.5 			#that is goldenrod  color
set style line 9  lc rgb '#CD853F' lt 1 lw 1.5 pt 9 ps 1.5 			#that is peru Color
set style line 10  lc rgb '#8B4513' lt 1 lw 1.5 pt 10 ps 1.5 		#that is saddlebrown Color


set key top right

set grid #GNU FILE
set yrange [0:200]
set xtics (10,11,12,13,14,15,16,17,18,19,20)
set xlabel "Number of Drivers (K)"
set ylabel "Distance"
set output "firstgraph.pdf" #GNU FILE
plot "rs.dat" using 1:2 with linespoint lt 13 pt 13 title "Random",\
 "ddataa.txt" using 1:2 with linespoint lt 2 pt 6  title 'BS',\
 "ddataa.txt" using 1:3 with linespoint lt 7 title "K-Means",\
 "sa.dat" using 1:2 with linespoint lt 11 pt 11 title "SA"

#reset
#fontSpec(s) = sprintf(", %d", s)

#set term pdf enhanced fontSpec(16)
#set terminal pdf
#set output 'distance.pdf'
#set title 'Hello, world'                       
#set xlabel 'Number of Drivers (K)'                              
#set ylabel 'Distance'                          

#set yrange [1:200]
#set xtics (10, 11,12,13,14,15,16,17,18,19,20)
# labels


# key/legend
#set key top right



# arrow
#set arrow from 1,1 to 5,10
#plot 'distance.txt' using 1:2 w lp lt 13 pt 13  title 'Random', 'distance.txt' using 1:3 w lp lt 2 pt 6  title 'BS', 'distance.txt' using 1:4 w lp lt 7 title 'K-#Means', 'distance.txt' using 1:5 w lp lt 11 pt 11 title 'SA' 
