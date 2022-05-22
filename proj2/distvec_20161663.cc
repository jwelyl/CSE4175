#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

#define NONE -999         //  연결되어 있다가 끊어진 상태
#define INF  987654321

using namespace std;

ifstream fp_t, fp_m, fp_c;  //  input file stream
ofstream fp_o;  //  output_file stream

int n;  //  # of nodes
vector<pair<pair<int, int>, int> > edges;  //  그래프 edges (from, to, cost)
vector<vector<int> > dist;      //  거리 벡터(dist[i][j]는 i에서 출발했을 때, j까지의 거리)
vector<vector<int> > parents;   //  parents[i][j]는 i번 node로부터 Shortest Path Tree를 그렸을 때 j번 node의 부모 
vector<pair<pair<int, int>, string> > message_list; //  message 목록

void err_exit(string err_msg);
void change_cost(int from, int to, int cost);
void bellman_ford(int start, vector<int>& s_dist, vector<int>& s_parents);
int find_next(int start, int dest); 
string find_path(int start, int dest);
void print_routing_tables();
void simulate_message();

int main(int argc, char* argv[]) {
  if(argc != 4)
    err_exit("usage: ./distvec_20161663 topologyfile messagesfile changesfile\n");

  fp_t.open(argv[1]);
  fp_m.open(argv[2]);
  fp_c.open(argv[3]);

  if(fp_t.fail() || fp_m.fail() || fp_c.fail())
    err_exit("Error: open input file.\n");

  fp_o.open("output_dv.txt");
  if(fp_o.fail())
    err_exit("Error: open output file.\n");

  fp_t >> n;
  dist.assign(n, vector<int>(n, INF));
  parents.assign(n, vector<int>(n, NONE));

  //  read topology file
  while(true) {
    int from, to, cost;
    fp_t >> from >> to >> cost;
    
    if(fp_t.eof()) break;
    edges.push_back(make_pair(make_pair(from, to), cost));
    edges.push_back(make_pair(make_pair(to, from), cost));
  }

  //  read message file
  while(true) {
    string input;
    int node[2], pos = 0;
    string message;
  
    getline(fp_m, input);
    
    //cout << input << "\n";
    if(fp_m.eof()) break;

    for(int i = 0; i < 2; i++) {
      int start = pos;
      for(; pos < (int)input.size(); pos++) {
        if(input[pos] == ' ') {
          node[i] = stoi(input.substr(start, pos - start));
          pos = pos + 1;
          break;
        }
      }
    }
    message = input.substr(pos);
    //cout << node[0] << " " << node[1] << " " <<  message << "\n";
    message_list.push_back(make_pair(make_pair(node[0], node[1]), message));
  }

  //  step 0
  for(int i = 0; i < n; i++)
    bellman_ford(i, dist[i], parents[i]);

  print_routing_tables();
  simulate_message();

  //  read changes file
  //  step 1 ~
  while(true) {
    int from, to, cost;

    fp_c >> from >> to >> cost;
    if(fp_c.eof()) break;
    change_cost(from, to, cost);

    for(int i = 0; i < n; i++)
      bellman_ford(i, dist[i], parents[i]);
    
    print_routing_tables();
    simulate_message();
  }

  return 0;
}

//  err_msg 출력 후 비정상 종료
void err_exit(string err_msg) {
  cout << err_msg;
  exit(-1);
}

//  start에서 dest로 이동할 때 다음 node를 반환
int find_next(int start, int dest) {
  int cur = dest;

  while(parents[start][cur] != start) {
    if(parents[start][cur] == NONE) return NONE;  //  dest와 연결 x
    cur = parents[start][cur];
  }

  return cur;
}

//  from node와 to node의 연결 상태 변화
void change_cost(int from, int to, int cost) {
  bool conn1 = false, conn2 = false;

  for(int i = 0; i < (int)edges.size(); i++) {
    int vf = edges[i].first.first;
    int vr = edges[i].first.second;

    if(vf == from && vr == to) {
      conn1 = true;
      edges[i].second = cost;
    }
    else if(vf == to && vr == from) {
      conn2 = true;
      edges[i].second = cost;
    } 
  }

  if(conn1 != conn2)
    err_exit("Graph Error!\n");
  else if(!conn1) { //  graph에 edge 새로 삽입
    edges.push_back(make_pair(make_pair(from, to), cost));
    edges.push_back(make_pair(make_pair(to, from), cost));
  }
}

void bellman_ford(int start, vector<int>& s_dist, vector<int>& s_parents) {
  for(int i = 0; i < (int)s_dist.size(); i++) {
    s_dist[i] = INF;
    s_parents[i] = NONE;
  }
  s_dist[start] = 0;
  s_parents[start] = start;

  for(int i = 1; i < n; i++) {
    for(int j = 0; j < (int)edges.size(); j++) {
      int vf = edges[j].first.first;
      int vr = edges[j].first.second;
      int w = edges[j].second;

      if(w == NONE) continue; //  끊어진 간선

      if(s_dist[vf] + w < s_dist[vr]) {
        s_dist[vr] = s_dist[vf] + w;
        s_parents[vr] = vf;
      }
      else if(s_dist[vf] + w == s_dist[vr]) {
        int cur_parent = s_parents[vr];       //  현재 vr의 parent
        int cur_next = find_next(start, vr);  //  현재 vr의 parent일 때 next
        int new_next;                         //  vr의 parent를 vf로 바꾸었을 때 next

        s_parents[vr] = vf;
        new_next = find_next(start, vr);

        if(new_next > cur_next) //  next가 작아질 경우에만 parent를 변경
          s_parents[vr] = cur_parent;
        
      }
    }
  }

  for(int i = 0; i < (int)edges.size(); i++) {
    int vf = edges[i].first.first;
    int vr = edges[i].first.second;
    int w = edges[i].second;

    if(w == NONE) continue;

    if(s_dist[vf] + w < s_dist[vr])
      err_exit("negative cost cycle\n");
  }
}

//  start에서 dest로의 경로 출력(dest는 제외)
string find_path(int start, int dest) {
  stack<int> st;
  string path = "";
  int cur = dest;

  while(parents[start][cur] != start) {
    st.push(parents[start][cur]);
    cur = parents[start][cur];
  }
  st.push(start);

  while(!st.empty()) {
    path += to_string(st.top());
    path += " ";
    st.pop();
  }

  return path;
}

//  routing table 출력
void print_routing_tables() {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int next = find_next(i, j);
      if(next != NONE)
        fp_o << j << " " << next << " " << dist[i][j] << "\n";
    }
    fp_o << "\n";
  }
}

//  message를 simulation함
void simulate_message() {
   for(int i = 0; i < (int)message_list.size(); i++) {
    int start = message_list[i].first.first;
    int dest = message_list[i].first.second;
    int cost = dist[start][dest];
    string message = message_list[i].second;
    string path;

    if(cost == INF) //  두 node 사이의 경로가 없을 경우 
      fp_o << "from " << start << " to " << dest << " cost infinite hops unreachable message " << message << "\n";
    else {
      path = find_path(start, dest);
      fp_o << "from " << start << " to " << dest << " cost " << cost << " hops " << path << "message " << message << "\n";
    }
  }
  fp_o << "\n";
}
