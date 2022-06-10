#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <stdio.h>
// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//for cusomer
#include <unistd.h>  //close open
#include <vector>
#include "sfmt/SFMT.h"
#include <fstream>
#include <unistd.h>
#include <string>

#include <queue>

#define ASSERT(v) {if (!(v)) {cerr<<"ASSERT FAIL @ "<<__FILE__<<":"<<__LINE__<<endl; exit(1);}}

using namespace std;

int n;
int m;

void readNM(string attribute_path) {
	ifstream cin(attribute_path);
	ASSERT(!cin == false);
	string s;
	while (cin >> s)
	{
		if (s.substr(0, 2) == "n=")
		{
			n = atoi(s.substr(2).c_str());
			continue;
		}
		if (s.substr(0, 2) == "m=")
		{
			m = atoi(s.substr(2).c_str());
			continue;
		}
		ASSERT(false);
	}	
	cin.close();
}

vector<vector<double> > weights;

vector<vector<int> > readFile(string graph_file, bool has_weight) {
	vector<vector<int> > in_edge(n, vector<int>());

	fstream fin;
	fin.open((graph_file).c_str(), ios::in);
	int readCnt=0;
	string tp;
	while (getline(fin, tp)) {
		stringstream s(tp);
		readCnt ++;
        int a, b;
        if (has_weight) {
            double p;
            s >> a >> b >> p;
            weights[a].push_back(p);
        } else {
            s >> a >> b;
        }
		
		ASSERT( a<n );
		ASSERT( b<n );
		in_edge[a].push_back(b);
	}
	fin.close();

	return in_edge;
}

int readSeed(string seed_file) {
	// vector<vector<int> > in_edge(n, vector<int>());

	fstream fin;
	fin.open((seed_file).c_str(), ios::in);
	int readCnt=0;
	string tp;
	getline(fin, tp);
    stringstream s(tp);
    readCnt ++;
    int seed;
    s >> seed;
	fin.close();

	return seed;
}

vector<vector<int> > graph;
vector<vector<int> > realization;
vector<bool> visited;
vector<bool> visitedInRealization;
int visitedCount;

/**
 * @brief Runs a BFS on a realization to calculate a seed's influence
 * 
 * @param seed: the index of a newly selected seed node
 */
void BFS(int seed) {

    priority_queue<int> queue;
    queue.push(seed);
    visited[seed] = true;
    visitedCount += 1;

    while (queue.size() > 0) {
        int node = queue.top();
        queue.pop();

        for (int neighbor: realization[node]) {

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                visitedCount += 1;
                queue.push(neighbor);
            }
        }
    }
}

vector<vector<double> > initializeWeights(int n) {
    vector<vector<double> > w(n, vector<double>());
    return w;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
		cout << "Usage: ./bfs path realization" << endl;
		exit(0);
	}

    string folder = argv[1];
    string realization_no = argv[2];

    string attribute_file = folder+"/attribute.txt";
    string graph_file = folder+"/graph_ic.inf";
    string realization_file = folder+"/realization_"+realization_no;
    string seeds_file = folder+"/seed.txt";
    string visited_file = folder+"/visited.txt";

    readNM(attribute_file);

    visited.resize(n);
    weights = initializeWeights(n);

    graph = readFile(graph_file, true);
    realization = readFile(realization_file, false);
    int seed = readSeed(seeds_file);

    BFS(seed);

    ofstream graphfile;
    graphfile.open(graph_file);
    if (!graphfile) {
        cout << "File " << graph_file << " could not be opened" << endl;
    }
    int num_edges = 0;
    for (int i=0; i<n; i++) {
        for (int j=0; j<graph[i].size(); j++) {
            if (!visited[graph[i][j]] && !visited[i]) {
                graphfile << i << " " << graph[i][j] << " " << weights[i][j] << endl;
                num_edges += 1;
            }
        }
    }
    graphfile.close();

    ofstream realizationfile;
    realizationfile.open(realization_file);
    if (!realizationfile) {
        cout << "File " << realization_file << " could not be opened" << endl;
    }
    for (int i=0; i<n; i++) {
        for (int j=0; j<realization[i].size(); j++) {
            if (!visited[realization[i][j]] && !visited[i]) {
                realizationfile << i << " " << realization[i][j] << endl;
            }
        }
    }
    realizationfile.close();

    int num_nodes = n - visitedCount;

    // cout << visitedCount << endl;

    ofstream visitedCountfile;
    visitedCountfile.open(folder+"/visited_count.txt");
    visitedCountfile << visitedCount << endl;
    visitedCountfile.close();

    ofstream visitedfile;
    visitedfile.open(visited_file, ios_base::app);
    if (!realizationfile) {
        cout << "File " << visited_file << " could not be opened" << endl;
    }
    for (int i=0; i<n; i++) {
        if(visited[i]) {
            visitedfile << i << endl;
        }
    }
    visitedfile.close();

    // ofstream attributefile;
    // attributefile.open(attribute_file);
    // if (!attributefile) {
    //     cout << "File " << attribute_file << " could not be opened" << endl;
    // }
    // attributefile << "n=" << num_nodes << endl;
    // attributefile << "m=" << num_edges << endl;
    // attributefile << endl;
    // attributefile.close();

}