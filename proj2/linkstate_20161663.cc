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
vector<vector<pair<int, int> > > graph;
vector<vector<int> > dist;      //  거리 벡터(dist[i][j]는 i에서 출발했을 때, j까지의 거리)
vector<vector<int> > parents;   //  parents[i][j]는 i번 node로부터 Shortest Path Tree를 그렸을 때 j번 node의 부모 
vector<pair<pair<int, int>, string> > message_list; //  message 목록

void err_exit(string err_msg);
void print_graph(); 
void change_cost(int from, int to, int cost); 
void dijkstra(int start, vector<int>& s_dist, vector<int>& s_parents); 
int find_next(int start, int dest); 
string find_path(int start, int dest);
void print_routing_tables();
void simulate_message();

int main(int argc, char* argv[]) {
  if(argc != 4)
    err_exit("usage: ./linkstate_20161663 topologyfile messagesfile changesfile\n");

  fp_t.open(argv[1]);
  fp_m.open(argv[2]);
  fp_c.open(argv[3]);

  if(fp_t.fail() || fp_m.fail() || fp_c.fail())
    err_exit("Error: open input file.\n");

  fp_o.open("output_ls.txt");
  if(fp_o.fail())
    err_exit("Error: open output file.\n");

  fp_t >> n;
  graph.assign(n, vector<pair<int, int> >());
  dist.assign(n, vector<int>(n, INF));
  parents.assign(n, vector<int>(n, NONE));

  //  read topology file
  while(true) {
    int from, to, cost;
    fp_t >> from >> to >> cost;
    
    if(fp_t.eof()) break;
    graph[from].push_back(make_pair(to, cost));
    graph[to].push_back(make_pair(from, cost));
  }
  // print_graph();

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
    dijkstra(i, dist[i], parents[i]);

  print_routing_tables();
  simulate_message();

  //  read changes file
  //  step 1 ~
  while(true) {
    int from, to, cost;

    fp_c >> from >> to >> cost;
    if(fp_c.eof()) break;

    change_cost(from, to, cost);
    //print_graph();
    //cout << "\n";

    for(int i = 0; i < n; i++)
      dijkstra(i, dist[i], parents[i]);
    
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

//  그래프 인접 리스트 형태로 출력(Debugging)
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

//  from node와 to node의 연결 상태 변화
void change_cost(int from, int to, int cost) {
  bool conn1 = false, conn2 = false;

  for(int i = 0; i < (int)graph[from].size(); i++) {
    if(graph[from][i].first == to) {
      graph[from][i].second = cost;
      conn1 = true;
    }
  }
  for(int i = 0; i < (int)graph[to].size(); i++) {
    if(graph[to][i].first == from) {
      graph[to][i].second = cost;
      conn2 = true;
    }
  }

  if(conn1 != conn2)
    err_exit("Graph Error!\n");
  else if(!conn1) { //  graph에 edge 새로 삽입
    graph[from].push_back(make_pair(to, cost));
    graph[to].push_back(make_pair(from, cost));
  }
  else return;
}

//  start node부터 다른 node까지의 거리 계산
void dijkstra(int start, vector<int>& s_dist, vector<int>& s_parents) {
  priority_queue<pair<int, int> > pq;
  
  for(int i = 0; i < (int)s_dist.size(); i++) {
    s_dist[i] = INF;
    s_parents[i] = NONE;
  }
  s_dist[start] = 0;
  s_parents[start] = start;

  pq.push(make_pair(-s_dist[start], -start));

  while(!pq.empty()) {
    int cdist = -pq.top().first;   //  현재 노드까지의 알려진 최소거리
    int cnode = -pq.top().second;  //  현재 노드
    pq.pop();

    //  거리가 더 짧아질 수 없을 경우
    if(s_dist[cnode] < cdist) continue;

    //  cnode의 인접노드 확인
    for(int i = 0; i < (int)graph[cnode].size(); i++) {
      int next = graph[cnode][i].first;
      int new_dist;
      
      if(graph[cnode][i].second == NONE) continue;  //  연결 끊어진 경우 skip
      new_dist = cdist + graph[cnode][i].second;

      if(new_dist < s_dist[next]) {
        s_dist[next] = new_dist;
        pq.push(make_pair(-s_dist[next], -next));
        s_parents[next] = cnode;
      }
      else if(new_dist == s_dist[next]) {
        if(s_parents[next] > cnode)
          s_parents[next] = cnode;
      }
    }
  } 
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
