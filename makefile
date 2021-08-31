all: sa randm kmeans

sa:	sa.cpp
	g++ -Wall -o sa sa.cpp

randm:	rs.cpp
	g++ -Wall -o randm rs.cpp
		
clean:
	rm  -f randm sa rdsmp

cleanall:
	rm -f randm sa rsout* rs.dat saout* sa.dat firstgraph.pdf 

                                     	
