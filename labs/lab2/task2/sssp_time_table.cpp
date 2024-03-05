#include <iostream>
#include <ios>
#include <deque>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int VERY_LARGE_DISTANCE = 999999999;

struct Edge {
  int node_index;
  int t_0;
  int P;
  int d;

  Edge(int node_index, int t_0, int P, int d) {
    this->node_index = node_index;
    this->t_0 = t_0;
    this->P = P;
    this->d = d;
  }
};

struct Node {
  int index;
  int previous;
  int distance;
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

void dijkstra(vector<Node> &nodes, vector<vector<Edge>> &neighbours, int source) {
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

    for (Edge neighbour : neighbours[curr_node.index]) {
      int n = neighbour.node_index;
      int t_0 = neighbour.t_0;
      int P = neighbour.P;
      int d = neighbour.d;
      Node &n_node = nodes[n];

      // Update distances to unvisited neighbour nodes for which we have encountered 
      // a cheaper path 
      // Also push such neighbour nodes to the priority queue of nodes to visit
      // Note: this may cause duplicates in the priority queue
      if (!n_node.visited) {
        bool can_reach = true;
        int waiting_time;

        // Calculate how much time we need to wait at the current node until 
        // we can reach the neighbour node, if we can reach it at all
        if (curr_node.distance <= t_0) {
          // Wait until the first point in time where the neighbour is reacheable
          waiting_time = t_0 - curr_node.distance;
        }
        else if (P == 0) {
          // We have already passed the single point in time where the neighbour 
          // was reacheable => unreachable
          can_reach = false;
        }
        else {
          // We have passed the start time at which we can reach the neighbour node 
          // and it is reacheable every P:th point in time after that
          // => Modulus arithmetic to figure out how long we need to wait
          waiting_time = (t_0 - curr_node.distance) % P;
          if (waiting_time < 0) {
            waiting_time += P;
          }
        }

        // Calculate the time that we will reach the neighbour at 
        // This is the current time + the time spent waiting for the neighbour 
        // to be reacheable + time of travel to neighbour
        int time_reached = curr_node.distance + waiting_time + d;
        if (can_reach && time_reached < n_node.distance) {
          // We can reach the neighbour and we can reach it at an earlier 
          // time than previously reached (time seen as distance)
          n_node.distance = time_reached;
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

int shortest_path_cost(vector<Node> &nodes, int destination) {
  Node &dest_node = nodes[destination];
  return nodes[destination].distance;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int N, M, S, Q, u, v, t_0, P, d, q;
  while (true) {
    cin >> N >> M >> Q >> S;
    if (cin.eof() || (N == 0 && M == 0 && S == 0 && Q == 0)) {
      break;
    }
    vector<Node> nodes;
    for (int n = 0; n < N; n++) {
      nodes.push_back(Node(n));
    }
    // Represent edges as a list of neighbours for every node index
    // A neighbour is represented as a pair containing neighbour index and cost 
    vector<vector<Edge>> neighbours = vector<vector<Edge>>(N);
    for (int e = 0; e < M; e++) {
      cin >> u >> v >> t_0 >> P >> d;
      neighbours[u].push_back({v, t_0, P, d});
    }
    dijkstra(nodes, neighbours, S);
    for (int i = 0; i < Q; i++) {
      cin >> q;
      int shortest_cost = shortest_path_cost(nodes, q);
      if (shortest_cost < VERY_LARGE_DISTANCE) {
        cout << shortest_cost << "\n";
      }
      else {
        cout << "Impossible\n";
      }
    }
    cout << "\n";
  }
  cout.flush();
}

