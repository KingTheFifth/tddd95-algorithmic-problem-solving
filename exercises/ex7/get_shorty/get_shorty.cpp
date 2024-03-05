#include <iomanip>
#include <iostream>
#include <ios>
#include <deque>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using namespace std;

double VERY_LARGE_DISTANCE = -1;

struct Node {
  int index;
  int previous;
  double distance;
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

  bool operator<(const Node &other) const {
    return this->distance < other.distance;
  }
};

void dijkstra(vector<Node> &nodes, vector<vector<pair<int, double>>> &neighbours, int source, double start_size) {
  priority_queue<Node, vector<Node>, less<Node>> to_visit;

  // Set distance from source to itself to 0 and set it as the first node 
  // to visit
  nodes[source].distance = start_size; 
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

    for (pair<int, double> &neighbour : neighbours[curr_node.index]) {
      Node &n_node = nodes[neighbour.first];
      // Update distances to unvisited neighbour nodes for which we have encountered 
      // a cheaper path 
      // Also push such neighbour nodes to the priority queue of nodes to visit
      // Note: this may cause duplicates in the priority queue
      if (!n_node.visited) {
        if (curr_node.distance * neighbour.second > n_node.distance) {
          n_node.distance = curr_node.distance * neighbour.second;
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

  double total_cost = curr_node.distance;
  path.push_back(destination);
  while (curr_node.previous != -1) {
    path.push_back(curr_node.previous);
    curr_node = nodes[curr_node.previous];
  }
  return path;
}

double shortest_path_cost(vector<Node> &nodes, int destination) {
  Node &dest_node = nodes[destination];
  return nodes[destination].distance;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int N, M, u, v;
  double f;
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
    vector<vector<pair<int, double>>> neighbours = vector<vector<pair<int, double>>>(N);
    for (int e = 0; e < M; e++) {
      cin >> u >> v >> f;
      neighbours[u].push_back({v, f});
      neighbours[v].push_back({u, f});
    }

    dijkstra(nodes, neighbours, 0, 1);
    double shortest_cost = shortest_path_cost(nodes, N-1);
    if (shortest_cost > VERY_LARGE_DISTANCE) {
      cout << std::fixed << setprecision(4) << shortest_cost << "\n";
    }
    else {
      cout << "Impossible\n";
    }
  }
  cout.flush();
}

