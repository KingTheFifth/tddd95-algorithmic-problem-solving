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
  int d;

  Edge(int node_index, int t_0, int d) {
    this->node_index = node_index;
    this->t_0 = t_0;
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

void dijkstra(vector<Node> &nodes, vector<vector<Edge>> &neighbours, int source, int start_time) {
  priority_queue<Node, vector<Node>, greater<Node>> to_visit;

  // Set start time of the start node
  nodes[source].distance = start_time; 
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
      int d = neighbour.d;
      Node &n_node = nodes[n];

      // Update distances to unvisited neighbour nodes for which we have encountered 
      // a cheaper path 
      // Also push such neighbour nodes to the priority queue of nodes to visit
      // Note: this may cause duplicates in the priority queue
      if (!n_node.visited) {
        int waiting_time = 0;

        if (t_0 == -1 || curr_node.distance < t_0 || curr_node.distance >= t_0 + d) {
          // Mister George does not visit this street
          // or Luka enters the street before George
          // or Luka enters the street after George has left it
          // => No waiting time
          waiting_time = 0;
        }

        else if (curr_node.distance >= t_0 && curr_node.distance < t_0 + d) {
          // Wait until George has left the street
          waiting_time = t_0 + d - curr_node.distance;
        }

        // Calculate the time that we will reach the neighbour at 
        // This is the current time + the time spent waiting for the neighbour 
        // to be reacheable + time of travel to neighbour
        int time_reached = curr_node.distance + waiting_time + d;
        if (time_reached < n_node.distance) {
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
  int nodes, edges, start, goal, K, G, u, v, d;
  while (true) {
    cin >> nodes >> edges >> start >> goal >> K >> G;
    if (cin.eof()) {
      break;
    }

    // Keep track of the node indices, in this case nodes are intersections
    vector<Node> node_set;
    for (int n = 0; n < nodes+1; n++) {
      node_set.push_back(Node(n));
    }

    // Keep track of the order of nodes that George visits
    // This is used to figure which edge (street) at which time that George 
    // is traversing
    vector<int> george_visit;
    for (int n = 0; n < G; n++) {
      cin >> u;
      george_visit.push_back(u);
    }

    // Keep track of edges between nodes, i.e. streets between intersections
    vector<vector<Edge>> neighbours = vector<vector<Edge>>(nodes+1);
    for (int e = 0; e < edges; e++) {
      cin >> u >> v >> d;
      // Undirected graph => need edges going both ways
      // Assume George does not traverse the edge, represented as start time -1
      neighbours[u].push_back({v, -1, d});
      neighbours[v].push_back({u, -1, d});
    }

    // Calculate and update start times of the edges that George traverses
    // George starts his traversal at time 0
    int start_time = 0;
    for (int i = 0; i < george_visit.size()-1; i++) {
      for (Edge &v : neighbours[george_visit[i]]) {
        if (v.node_index == george_visit[i+1]) {
          v.t_0 = start_time;
          start_time += v.d;
        }
      }
    }
    for (int i = george_visit.size() -1; i > 0; i--) {
      for (Edge &v : neighbours[george_visit[i]]) {
        if (v.node_index == george_visit[i-1]) {
          start_time -= v.d;
          v.t_0 = start_time;
        }
      }
    }

    dijkstra(node_set, neighbours, start, K);
    int shortest_cost = shortest_path_cost(node_set, goal);
    if (shortest_cost < VERY_LARGE_DISTANCE) {
      cout << shortest_cost - K << "\n";
    }
    else {
      cout << "Impossible\n";
    }
    cout << "\n";
  }
  cout.flush();
}
