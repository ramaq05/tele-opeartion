#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

#define UPPER_BS_LIMIT 3001
#define UPPER_DRS_LIMIT 500
#define INF 0x3f3f3f3f 
float Latency = INF;
int total_drivers, total_BS;
// Coordinates of Drivers
vector<pair<float,float>> Drivers(UPPER_DRS_LIMIT);
// Coordinates of Base stations
vector<pair<float,float>> BaseStations(UPPER_BS_LIMIT);
// Ajacency list matrix - Topology
vector<vector<pair<int,float>>> graph(0);
//Paths
//Using djikstra, we will find all the base station for each src, dest path
map<pair<int,int>,vector<int>> shortest_path;

set<int> BaseStationsOnAllPaths;
int K; // no of drivers
vector<int> djikstra(int src, int dest) {
	typedef pair<float,int> iPair;
	priority_queue< iPair, vector <iPair> , greater<iPair> > pq; 
	vector<float> dist(total_BS, INF);
	vector<int> parent(total_BS, -1); 
	pq.push(make_pair(0, src)); 
    dist[src] = 0;
    while (!pq.empty()) 
    { 
        int u = pq.top().second; 
        pq.pop(); 
        vector<pair<int, float>>::iterator i; 
        if(u==dest) break;
        for (i = graph[u].begin(); i != graph[u].end(); ++i) 
        { 
            int v = (*i).first; 
            float weight = (*i).second; 
            if (dist[v] > dist[u] + weight) 
            {
                dist[v] = dist[u] + weight; 
                parent[v] = u; 
                pq.push(make_pair(dist[v], v)); 
            } 
        }
    }
    vector<int> bs;
    bs.push_back(dest);
    while(parent[dest]!=-1) {
    	dest = parent[dest];
    	bs.push_back(dest);
    }
    reverse(bs.begin(),bs.end());
    BaseStationsOnAllPaths.insert(bs.begin(),bs.end());
    return bs; 
}

float dist(int Driver, int BaseStation){
    float dis = 0.0;
    float X_1 = Drivers[Driver].first;
    float Y_1 = Drivers[Driver].second;
    float X_2 = BaseStations[BaseStation].first; 
    float Y_2 = BaseStations[BaseStation].second;
    float x_distance = pow( (X_2 - X_1), 2 );
    float y_distance = pow( (Y_2 - Y_1), 2 );
    dis = float (sqrt( x_distance + y_distance));
    return dis;
}

// find the from a driver to the set of base stations
float findShortestDistance(int driver_index, int &best_index)
{
	best_index = -1;
	float best_distance = INF;
	for (int i = 0; i < BaseStationsOnAllPaths.size(); i++)
	{
		float di = dist(driver_index, i);
		if (di < best_distance)
		{
			best_distance = di;
			best_index = i;
		}
	}
	return best_index;
}

float distAvg(vector<int> &D, vector<int> &BaseStationsOnAllPaths) {
	float dis = 0;
	for(auto b: BaseStationsOnAllPaths)
	{
		float curr = INF;
		for(auto d: D)
		{
			curr = min(curr, dist(d,b));
		}
		dis+=curr;
	}
	//cout<<"dis "<<dis<<" "<<B.size()<<endl;
	return dis/(int)BaseStationsOnAllPaths.size();
}


float probability(float f1, float f2, double temperature) 
{
	float value = exp((f2 - f1) / temperature);
	//cout << "P = " << value << endl;
	return value;
}

// base station B0, B1, ... Bb
// important, D by value
vector<int> compute_func(vector <int> &BaseStationsOnAllPaths, vector <int> D) {
	int b = BaseStationsOnAllPaths.size();
	int n = D.size();
	vector <int> best_subset;
	vector <int> current_subset;
	//cout<<"Number of Drivers: "<<D.size()<<endl;
	//for(auto x:D)
		//cout << x << " ";
	//cout<<endl;

	//we select a random K unique elements
	while (current_subset.size() < K)
	{
		int index = rand() % D.size();
		current_subset.push_back(D[index]);
		// important...this is the SUBSET of UNUSED drivers
		D.erase(D.begin() + index);
	}

	// we calculate the distance from current_subset to B
	float best_distance = distAvg(current_subset, BaseStationsOnAllPaths);
	float current_distance = INF;
	best_subset = current_subset;

	int nIter = 0;
	const int MAX_ITER = 10000;
	// here the algoritm
	const float temperature = 10000;
	float alp = 0.0001 / temperature;
	float alt = 1 / MAX_ITER;
	float alpha = pow (alp,  alt);
	//const float coolingFactor = 0.995;
	float t = temperature; 
	do
	{
		// we select a random driver of current_subset
		int i = rand() % current_subset.size();

		// we select another driver in D (subslist of unused drivers)
		int j = rand() % D.size();

		// we swap
		swap(current_subset[i], D[j]);

		// we recompute the average distance
		current_distance = distAvg(current_subset, BaseStationsOnAllPaths);

		// if better we replace
		if (current_distance < best_distance || 
			rand()/float(RAND_MAX) > probability(current_distance, best_distance,t))
		{
			if (current_distance < best_distance)
			{
				best_distance = current_distance;
				best_subset = current_subset;
			}
			//cout << best_distance << " ";
			//for (int i = 0; i < K; i++)
				//cout << best_subset[i] << " ";
			//cout << endl;
		}
		else // roll back the last swap
			swap(current_subset[i], D[j]);
		t *= alpha;
	} while (++nIter < MAX_ITER && t > 1);
	
	return best_subset;
}

int main(int argc, char ** argv) {
	// Input number of drivers
	//cout<<"Number of drivers:";
	K = atoi(argv[1]);
	srand(time(NULL));
	
	// Input data - Coordinates of Drivers
	fstream Dr_coords;
	Dr_coords.open("ddata.txt");
	int dr_id = 0;
	float dr_x_coord = 0.0, dr_y_coord = 0.0;
	while (Dr_coords.eof() == 0) {
		if (dr_id == UPPER_DRS_LIMIT)
			break;
		Dr_coords >> dr_x_coord >> dr_y_coord;
		Drivers[dr_id] = {dr_x_coord, dr_y_coord};
		dr_id++;
	}
	Dr_coords.close();
	total_drivers = dr_id;

	// Input data - Coordinates of Base stations
	int bs_id = 0;
	float bs_x_coord = 0.0, bs_y_coord = 0.0;
	fstream bs_coord;
	bs_coord.open("bdata.txt");
	while (!bs_coord.eof()) {
		if (bs_id == UPPER_BS_LIMIT)
			break;
		bs_coord >> bs_x_coord >> bs_y_coord;
		BaseStations[bs_id] = {bs_x_coord, bs_y_coord};
		bs_id++;
	}
	bs_coord.close();
	total_BS = bs_id;
	graph.resize(total_BS);

	//Input data - Ajacency list matrix - Topology
	fstream topology_file;
	int v1, v2;
	float cost;
	topology_file.open("graph.txt");
	while (topology_file.eof() == 0) {
		topology_file >> v1 >> v2 >> cost;
		graph[v1].push_back({v2, cost});
		graph[v2].push_back({v1, cost});
	}
	topology_file.close();

	//Paths
	//Using djikstra, we will find all the base station for each src, dest path
	int src, dest;
	fstream srcdestpair;
	srcdestpair.open("srcdestpair.txt");
	while(!srcdestpair.eof()) {
		srcdestpair >> src >> dest;
		if((src==-1)&&(dest==-1))
			break;
		//cout<<"Source = "<<src<<", Destination = "<<dest<<endl;
		//cout<<"Shortest Path: ";
		shortest_path[{src, dest}] = djikstra(src, dest);
		//for(auto b: shortest_path[{src, dest}])
			//cout<<b<<" ";
		//cout<<endl;
	}
	srcdestpair.close();

	//cout<<"Number of Base stations that occur in Paths P: "<<BaseStationsOnAllPaths.size()<<endl;
	//cout<<"Base stations that occur in Paths P: "<<endl;
	//for(auto b: BaseStationsOnAllPaths)
	//{
		
			//cout<<b<<" ";
	//}
	//cout<<endl;

	// PART 1
	//cout<<"Computing the best subset of drivers"<<endl;
	vector<int> B;
	vector <int> D;
	// all base station indices
	for (int i = 0; i < BaseStationsOnAllPaths.size(); i++)
		B.push_back(i);
	// all drivers indices
	for (int i = 0; i < Drivers.size(); i++)
		D.push_back(i);
	// result = best subset of drivers
	vector<int> result = compute_func(B,D);
	// computing the distance between the resulting subset of drivers
	// and the base stations
	float best_distance = distAvg(result, B);

	// printing
	cout<< "drivers" << " " << K << " "<< best_distance  << endl;

	// printing indices of best drivers
	//for(auto x:result) 
		//cout << x << " ";
	//cout<<endl;

	return 0;
}
