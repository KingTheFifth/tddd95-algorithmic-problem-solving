#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <ios>
#include <limits>
#include <vector>

using namespace std;

using Edge_set = vector<tuple<int, int, int>>;
using Adj_list = vector<vector<int>>;
using i64 = int64_t;

i64 INFTY = numeric_limits<int>::max();

void handle_negative_cycles(Adj_list &adj_list, vector<int> &parent, vector<i64> &distance, vector<int> &bad_nodes) {
  int nodes = parent.size();
  vector<bool> visited = vector<bool>(nodes);

  for (int u : bad_nodes) {
    if (visited[u]) {
      continue;
    }

    int cycle_start = u;
    for (int v = 0; v < nodes; v++) {
      cycle_start = parent[cycle_start];
    }

    visited[cycle_start] = true;
    distance[cycle_start] = -INFTY;

    deque<int> to_visit = {cycle_start};
    int curr;
    while (!to_visit.empty()) {
      curr = to_visit.back();
      to_visit.pop_back();

      for (int next : adj_list[curr]) {
        if (!visited[next]) {
          visited[next] = true;
          distance[next] = -INFTY;
          to_visit.push_back(next);
        }
      }
    }
  }
}

void bellman_ford(Edge_set &edge_set, Adj_list &adj_list, vector<int> &parent, vector<i64> &distance, int start) {
  int nodes = parent.size();
  distance[start] = 0;
  vector<int> bad_nodes;

  for (int i = 0; i < nodes; i++) {
    for (tuple<int, int, int> &edge : edge_set) {
      int u = get<0>(edge);
      int v = get<1>(edge);
      int w = get<2>(edge);

      if (distance[u] < INFTY && distance[u] + w < distance[v]) {
        distance[v] = max(-INFTY, distance[u] + w);
        parent[v] = u;
      }
    }
  }

  for (tuple<int, int, int> &edge : edge_set) {
    int u = get<0>(edge);
    int v = get<1>(edge);
    int w = get<2>(edge);

    if (distance[v] == -INFTY) {
      bad_nodes.push_back(v);
    }

    else if (distance[u] < INFTY && distance[u] + w < distance[v]) {
      distance[v] = max(-INFTY, distance[u] + w);
      parent[v] = u;
      bad_nodes.push_back(v);
    }
  }

  handle_negative_cycles(adj_list, parent, distance, bad_nodes);
}

pair<i64, vector<i64>> shortest_path(vector<int> &parent, vector<i64> &distance, int destination) {
  if (distance[destination] == -INFTY || distance[destination] == INFTY) {
    return {distance[destination], {}};
  }
  
  vector<i64> path;
  int curr = destination;
  while (curr != -1) {
    path.push_back(curr);
    curr = parent[curr];
  }
  reverse(path.begin(), path.end());

  return {distance[destination], path};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int N, M, S, Q, u, v, w, q;
  while (true) {
    cin >> N >> M >> Q >> S;
    if (cin.eof() || (N == 0 && M == 0 && S == 0 && Q == 0)) {
      break;
    }

    Edge_set edge_set;
    Adj_list adj_list = Adj_list(N);
    for (int e = 0; e < M; e++) {
      cin >> u >> v >> w; 
      edge_set.push_back({u, v, w});
      adj_list[u].push_back(v);
    }

    vector<i64> distance = vector<i64>(N, INFTY);
    vector<int> parent = vector<int>(N, -1);
    bellman_ford(edge_set, adj_list, parent, distance, S);
    for (int i = 0; i < Q; i++) {
      cin >> q;
      pair<int, vector<i64>> result = shortest_path(parent, distance, q);
      if (result.first == -INFTY) {
        cout << "-Infinity\n";
      }
      else if (result.first == INFTY) {
        cout << "Impossible\n";
      }
      else {
        cout << result.first << "\n";
      }
    }
    cout << "\n";
  }
  cout.flush();
}
