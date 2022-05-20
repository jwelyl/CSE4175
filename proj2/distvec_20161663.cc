#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

//  input file pointers
FILE* fp_t;  //  topologyfile pointer
FILE* fp_m;  //  messagefile pointer
FILE* fp_c;  //  changesfile pointer

//  output file pointer
FILE* fp_o;  //  output_ls.txt pointer

int n;  //  # of nodes
vector<vector<pair<int, int> > > graph;

void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
  exit(-1);
}

void print_graph() {
  for(int i = 0; i < n; i++) {
    cout << "graph[" << i << "] : ";
    for(int j = 0; j < (int)graph[i].size(); j++) {
      cout << "(" << graph[i][j].first << ", " << graph[i][j].second << ") ";
      if(j != (int)graph[i].size() - 1)
        cout << "-> ";
    }
    cout << "\n";
  }
}

int main(int argc, char* argv[]) {
  if(argc != 4)
    err_exit("usage: ./distvec_20161663 topologyfile messagefile changefile\n");

  fp_t = fopen(argv[1], "r");
  fp_m = fopen(argv[2], "r");
  fp_c = fopen(argv[3], "r");

  if(!fp_t || !fp_m || !fp_c)
    err_exit("Error: open input file.\n");

  fscanf(fp_t, "%d", &n);
  graph.assign(n, vector<pair<int, int> >());

  while(true) {
    int from, to, cost;
    
    if(fscanf(fp_t, "%d %d %d", &from, &to, &cost) == EOF) break;
    graph[from].push_back(make_pair(to, cost));
    graph[to].push_back(make_pair(from, cost));
  }
  //
  print_graph();
  //
  return 0;
}
