#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

#define NONE -999         //  연결되어 있다가 끊어진 상태
#define INF  987654321

using namespace std;

//  input file pointers
FILE* fp_t;  //  topologyfile pointer
FILE* fp_m;  //  messagefile pointer
FILE* fp_c;  //  changesfile pointer

//  output file pointer
FILE* fp_o;  //  output_ls.txt pointer

int n;  //  # of nodes
vector<vector<pair<int, int> > > graph;
vector<vector<int> > dist;      //  거리 벡터(dist[i][j]는 i에서 출발했을 때, j까지의 거리)
vector<vector<int> > parents;   //  parents[i][j]는 i번 node로부터 Shortest Path Tree를 그렸을 때 j번 node의 부모 
vector<pair<pair<int, int>, string> > message_list; //  message 목록

//  err_msg 출력 후 비정상 종료
void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
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
  bool conn1 = false; //  from->to 존재
  bool conn2 = false; //  to->from 존재

  //  이미 from과 to가 연결되었나 확인
  for(int i = 0; i < n; i++) {
    if(i == from) {
      for(int j = 0; j < (int)graph[i].size(); j++) {
        if(graph[i][j].first == to) {
          graph[i][j].second = cost;
          conn1 = true;
        }
      }
    }
    else if(i == to) {
      for(int j = 0; j < (int)graph[i].size(); j++) {
        if(graph[i][j].first == from) {
          graph[i][j].first = cost;
          conn2 = true; 
        }
      }
    } 
  }

  if(conn1 != conn2)  //  양방향 그래프가 아닐 경우
    err_exit("Something was wrong in graph\n");
  else if(conn1 && conn2) return;  //  이미 연결된 두 node의 상태만 변경
  
  //  기존에 연결되지 않았을 경우, 새로운 연결 추가
  graph[from].push_back(make_pair(to, cost));
  graph[to].push_back(make_pair(from, cost));
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

  pq.push(make_pair(-s_dist[start], start));

  while(!pq.empty()) {
    int cdist = -pq.top().first;  //  현재 노드까지의 알려진 최소거리
    int cnode = pq.top().second;  //  현재 노드
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
        pq.push(make_pair(-s_dist[next], next));
        s_parents[next] = cnode;
      }
    }
  } 
}

int find_next(int start, int dest) {
  int cur = dest;

  while(parents[start][cur] != start) {
    if(parents[start][cur] == NONE) return NONE;  //  dest와 연결 x
    cur = parents[start][cur];
  }

  return cur;
}

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

int main(int argc, char* argv[]) {
  if(argc != 4)
    err_exit("usage: ./linkstate_20161663 topologyfile messagefile changefile\n");

  fp_t = fopen(argv[1], "r");
  fp_m = fopen(argv[2], "r");
  fp_c = fopen(argv[3], "r");

  if(!fp_t || !fp_m || !fp_c)
    err_exit("Error: open input file.\n");

  fp_o = fopen("output_ls.txt", "w");
  if(!fp_o)
    err_exit("Error: open output file.\n");

  fscanf(fp_t, "%d", &n);
  graph.assign(n, vector<pair<int, int> >());
  dist.assign(n, vector<int>(n, INF));
  parents.assign(n, vector<int>(n, NONE));

  //  read topology file
  while(true) {
    int from, to, cost;
    
    if(fscanf(fp_t, "%d %d %d", &from, &to, &cost) == EOF) break;
    graph[from].push_back(make_pair(to, cost));
    graph[to].push_back(make_pair(from, cost));
  }
  //print_graph();
  
  //  read message file
  while(true) {
    char input[1024];
    string s, message;
    int node[2];
    int pos = 0;

    if(!fgets(input, 1024, fp_m)) break;
    s = input;
    //cout << input;

    for(int i = 0; i < 2; i++) {
      int start = pos;
      for(; pos < (int)s.size(); pos++) {
        if(s[pos] == ' ') {
          node[i] = stoi(s.substr(start, pos - start));
          pos = pos + 1;
          break;
        }
      }
    }
    message = s.substr(pos);
    message_list.push_back(make_pair(make_pair(node[0], node[1]), message));
    //  cout << node[0] << " " << node[1] << " " << message;
  }

  //  step 0
  for(int i = 0; i < n; i++)
    dijkstra(i, dist[i], parents[i]);

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int next = find_next(i, j);
      cout << j << " " << next << " " << dist[i][j] << "\n";
    }
    cout << "\n";
   // cout << "parents in SPT " << i << "\n";
   // for(int j = 0; j < n; j++)
   //   cout << parents[i][j] << " ";
   // cout << "\n\n";
  }

  for(int i = 0; i < (int)message_list.size(); i++) {
    int start = message_list[i].first.first;
    int dest = message_list[i].first.second;
    int cost = dist[start][dest];
    string message = message_list[i].second;
    string path;

    if(cost == INF) 
      cout << "from " << start << " to " << dest << " cost infinite hops unreachable message " << message;
    else {
      path = find_path(start, dest);
      cout << "from " << start << " to " << dest << " cost " << cost << " hops " << path << "message " << message;
    }
  }


  return 0;
}
