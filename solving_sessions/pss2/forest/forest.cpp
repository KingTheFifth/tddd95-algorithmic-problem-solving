#include <cstdint>
#include <iostream>
#include <ios>
#include <deque>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using i64 = int64_t;

i64 VERY_LARGE_DISTANCE = numeric_limits<int>::max();

struct Node {
  int index;
  int previous;
  i64 distance;
  bool visited;

  Node() {
    previous = -1;
    distance = VERY_LARGE_DISTANCE;
    visited = false;
  }
  Node(int index) {
    this->index = index;
    previous = -1;
    distance = VERY_LARGE_DISTANCE;
    visited = false;
  }

  bool operator>(const Node &other) const {
    return this->distance > other.distance;
  }
};

void dijkstra(vector<Node> &nodes, vector<vector<pair<int, i64>>> &neighbours, int source) {
  priority_queue<Node, vector<Node>, greater<Node>> to_visit;

  // Set distance from source to itself to 0 and set it as the first node 
  // to visit
  nodes[source].distance = 0; 
  to_visit.push(nodes[source]);

  while (!to_visit.empty()) {
    const Node curr_node = to_visit.top();
    to_visit.pop();

    // We have encountered a duplicate in the priority queue, 
    // one which was reached through a worse path => safe to skip
    if (nodes[curr_node.index].visited) {
      continue;
    }

    // Mark this node as visited
    nodes[curr_node.index].visited = true;

    for (pair<int, i64> neighbour : neighbours[curr_node.index]) {
      Node &n_node = nodes[neighbour.first];
      // Update distances to unvisited neighbour nodes for which we have encountered 
      // a cheaper path 
      // Also push such neighbour nodes to the priority queue of nodes to visit
      // Note: this may cause duplicates in the priority queue
      if (!n_node.visited) {
        if (curr_node.distance + neighbour.second < n_node.distance) {
          n_node.distance = curr_node.distance + neighbour.second;
          n_node.previous = curr_node.index;
          to_visit.push(n_node);
        }
      }
    }
  }
}

vector<int> shortest_path(vector<Node> &nodes, int destination) {
  vector<int> path;
  Node &curr_node = nodes[destination];

  // There is no calculated path from the source node to the destination node
  if (curr_node.previous == -1) {
    return {};
  }

  int total_cost = curr_node.distance;
  path.push_back(destination);
  while (curr_node.previous != -1) {
    path.push_back(curr_node.previous);
    curr_node = nodes[curr_node.previous];
  }
  return path;
}

i64 shortest_path_cost(vector<Node> &nodes, int destination) {
  Node &dest_node = nodes[destination];
  return nodes[destination].distance;
}

i64 dfs(vector<Node> &nodes, vector<vector<pair<int, i64>>> &adj_list) {
  i64 res = 0;
  vector<bool> used = vector<bool>(nodes.size());
  deque<int> q = {0};
  while (!q.empty()) {
    int n = q.back();
    Node &n_node = nodes[n];
    q.pop_back();
    used[n] = true;

    for (pair<int, i64> next : adj_list[n]) {
      Node &next_node = nodes[next.first];
      if (next.first == 1) {
        res++;
        break;
      }
      else if (!used[next.first] && next_node.distance < n_node.distance) {
       q.push_back(next.first); 
      }
    }
  }

  return res;
}

i64 solve(vector<Node> &nodes, vector<vector<pair<int, i64>>> &adj_list) {
    dijkstra(nodes, adj_list, 1);
    return dfs(nodes, adj_list);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int N, M, u, v;
  i64 w;
  while (true) {
    cin >> N >> M;
    if (cin.eof() || (N == 0 && M == 0)) {
      break;
    }
    vector<Node> nodes;
    for (int n = 0; n < N; n++) {
      nodes.push_back(Node(n));
    }
    // Represent edges as a list of neighbours for every node index
    // A neighbour is represented as a pair containing neighbour index and cost 
    vector<vector<pair<int, i64>>> neighbours = vector<vector<pair<int, i64>>>(N);
    for (int e = 0; e < M; e++) {
      cin >> u >> v >> w;
      // Undirected
      neighbours[u-1].push_back({v-1, w});
      neighbours[v-1].push_back({u-1, w});
    }
    cout << solve(nodes, neighbours) << "\n";
  }
  cout.flush();
}
