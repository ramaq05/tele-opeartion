#include<bits/stdc++.h>
#include<iostream>
using namespace std;
#define UPPER_BS_LIMIT 3001
#define UPPER_DRS_LIMIT 501
#define INF 0x3f3f3f3f 
float Latency;
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

set<int> CandidateDrivers() {
	set<int> C;
	for(int D=0;D<total_drivers; D++)
	{
		for(int B=0;B<total_BS; B++)
		{

			if(dist(D,B) < Latency)
			{
				C.insert(D);
				break;
			}
		}
	}
	return C;
}

float distAvg(set<int> F, set<int> B) {
	float dis = 0;
	for(auto b: B)
	{
		float curr = INF;
		for(auto d: F)
		{
			curr = min(curr, dist(d,b));
		}
		dis+=curr;
	}
	//cout<<"dis "<<dis<<" "<<B.size()<<endl;
	return dis/(int)B.size();
}


set<int> computeRDSMP(set<int> P) {
	set<int> C = CandidateDrivers();
	set<int> F;
	/*cout<<"Candidate Drivers: ";
	for(auto c:C)
		cout<<c<<" ";
	cout<<endl;*/
	while(P.size()) {
		map<int, set<int>> cover;
		map<int, int> number_of_bs_covers;
		int mx = 0, d = -1;
		for(auto D: C) {
			for(auto B:P)
			{
				if(dist(D,B) < Latency)
				{
					cover[D].insert(B);
					number_of_bs_covers[D]++;
					if(number_of_bs_covers[D]>mx)
					{
						mx=number_of_bs_covers[D];
						d=D;
					}
				}				
			}
		}
		for(auto B:cover[d]) {
			P.erase(B);
		}
		F.insert(d);
		C.erase(d);
	}
	return F;
}

int main(int agrc, char ** argv) {
	// Input Latency
	//cout<<"Please Enter Value for Latency:"<<endl ;
	Latency = atoi(argv[1]);
	// saving data to txt file data.txt
	//fstream data;
	//data.open("data.txt");
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
	//cout<<"RDSMP: Computing ...."<<endl;
	set<int> F = computeRDSMP(BaseStationsOnAllPaths);

	cout<< "drivers"<< "  "<< Latency <<" " << F.size() << " ";
	//cout<<"Optimal set of drivers by max base stations covered:\n";
	//for(auto D:F) 
	//cout<<D<<" ";
	//cout<<endl;
	
	set <int> B = BaseStationsOnAllPaths;
	float Md = distAvg(F, B);
	cout<< " " << Md << " ";

	// PART 2
	//cout<<"RDSMP_Extended: Computing ...."<<endl;
	int B1 = *B.begin();
	set <int> A; //A: all the drivers that can cover B1
	for(int D=0;D<total_drivers; D++) {
		// calculate distace b/w B1 and the driver
		if(dist(D,B1) <= Latency)
			A.insert(D);
	}
	//cout<<"No of drivers that cover B1: "<<A.size()<<endl;
	for(auto v:A)
	{
		set<int> Bdash=B, cover;
		for(auto b:B)
		{
			if(dist(v,b) <= Latency)
				{
					cover.insert(b);
					Bdash.erase(b);
				}
		}
		set <int> Fdash= computeRDSMP(Bdash);
		float md = distAvg(Fdash, B);
		if(Fdash.size()==F.size() and Md > md)
		{
			Md = md;
			F = Fdash;
		}
	}
       
	cout << " " << F.size() << " ";
	//data << F.size() <<endl;
	//cout<<"Optimal set of drivers by shortest avg distance:\n";
	//for(auto D:F) //cout<<D<<" ";
	//data << D <<endl;
	//cout<<endl;
	cout<< " " << Md << " "<<endl;
	//data << Md <<endl;
	//data.close();
	return 0;
}
