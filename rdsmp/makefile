rdsmp.exe: rdsmp.o
	g++ -o rdsmp.exe -g rdsmp.o
rdsmp.o: rdsmp.cpp
	g++ -c -g -Wall rdsmp.cpp
clean:
	rm -f rdsmp.exe rdsmp.o
cleanall:
	rm -f rdsmp.exe rdsmp.o output* drivers.dat distance.dat 2nd_graph.pdf 3rd_graph.pdf
                                     	
