#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <ios>
#include <limits>
#include <vector>

using namespace std;

using Edge_set = vector<tuple<int, int>>;
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

void bellman_ford(Edge_set &edge_set, Adj_list &adj_list, vector<int> &energy,
    vector<int> &parent, vector<i64> &distance, int start, int start_energy) {
  int nodes = parent.size();
  distance[start] = start_energy;

  for (int i = 0; i < nodes; i++) {
    for (tuple<int, int> &edge : edge_set) {
      int u = get<0>(edge);
      int v = get<1>(edge);
      int w = energy[v];

      // if (distance[u] + w >= 0) {
      //   // We have reached non-negative distance, which corresponds to 
      //   // a non-positive negative energy level => cannot go this way
      //   continue;
      // }
      i64 new_dist = distance[u] + w;
      if (new_dist >= 0) {
        new_dist = INFTY;
      }

      if (distance[u] < INFTY && new_dist < distance[v]) {
        // We can indeed go this way and we have energy left going this way
        // Moreover, going to this room through this path leaves us with more 
        // energy than the previously found path to this room
        distance[v] = max(-INFTY, new_dist);
        parent[v] = u;
      }
    }
  }

  // Look for nodes which may be part of or reachable from a negative cycle
  vector<int> bad_nodes;
  for (tuple<int, int> &edge : edge_set) {
    int u = get<0>(edge);
    int v = get<1>(edge);
    int w = energy[v];

    if (distance[v] == -INFTY) {
      bad_nodes.push_back(v);
    }

    i64 new_dist = distance[u] + w;
    if (new_dist >= 0) {
      new_dist = INFTY;
    }

    else if (distance[u] < INFTY && new_dist < distance[v]) {
      distance[v] = max(-INFTY, new_dist);
      parent[v] = u;
      bad_nodes.push_back(v);
    }
  }

  // Handle negative cycles like a pro
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
  int nodes, e, edges, edge;
  while (true) {
    cin >> nodes;
    if (cin.eof() || nodes == -1) {
      break;
    }

    vector<int> energy = vector<int>(nodes);
    Edge_set edge_set;
    Adj_list adj_list = Adj_list(nodes);
    for (int n = 0; n < nodes; ++n) {
      cin >> e >> edges;
      energy[n] = -e;
      for (int e = 0; e < edges; ++e) {
        cin >> edge;
        edge_set.push_back({n, edge-1});
        adj_list[n].push_back(edge-1);
      }
    }


    vector<i64> distance = vector<i64>(nodes, INFTY);
    vector<int> parent = vector<int>(nodes, -1);
    bellman_ford(edge_set, adj_list, energy, parent, distance, 0, -100);
    pair<int, vector<i64>> result = shortest_path(parent, distance, nodes-1);
    if (result.first == -INFTY || result.first < 0) {
      cout << "winnable\n";
    }
    else {
      cout << "hopeless\n";
    }
  }
  cout.flush();
}
