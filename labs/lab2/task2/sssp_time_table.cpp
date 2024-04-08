/**
 *
 * Johannes Kung johku144
 *
 * Single sorce shortest path: time table graphs
 * (Directed graph)
 *
 * Time complexity: O(|E|*log(|E|)), see dijkstra()
 */
#include <iostream>
#include <ios>
#include <deque>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int VERY_LARGE_DISTANCE = 999999999;

/**
 * A struct for edges to be stored in an adjacency list. Used to more neatly 
 * represent the relevant info for an edge to a neighbour, that being 
 * the index of the neighbour and the information needed to determine at what 
 * time the edge can be traversed
 */
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

/**
 * A struct for nodes of a graph. This Node struct contains information needed 
 * for the implementation of Dijkstra's algorithm.
 */
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

/**
 * Implementation of Dijkstra's algorithm for single source shortest path to 
 * all nodes in a time table graph. Uses a Node struct to record reverse paths
 * through a previous-node pointer and the cost to reach a node from the source. 
 * The node struct is also used to mark if a node has been visited.
 *
 * Input: A graph in the form of a vector of the nodes of the graph together 
 * with an adjacency list, and the node index of the source node. Note that the 
 * adjacency list records neighbour node index as well as the edge cost to the 
 * neighbour
 *
 * Output: Shortest path from source node to every node, recorded by modifying 
 * the nodes in the node vector. Use shortest_path() and shortest_path_cost()
 * on the modified vector to retrieve the path and cost to a destination node.
 *
 * Time complexity:
 *  - O(|E|*log(|E|))
 */
void dijkstra(vector<Node> &nodes, vector<vector<Edge>> &neighbours, int source) {
  // Min-heap (priority queue) for visiting nodes
  priority_queue<Node, vector<Node>, greater<Node>> to_visit;

  // Set distance from source to itself to 0 and set it as the first node 
  // to visit
  nodes[source].distance = 0; 
  to_visit.push(nodes[source]);

  // O(|E|*log(|E|)) as every node is pushed at most for each edge incident to it 
  // and insertion/extraction is O(log(|E|))
  //
  // For simple graphs it holds that |E|=O(|V|^2) so the overall time complexity 
  // would be O(|E|*log(|V|)). This implementation is however for multigraphs 
  // so the complexity is then O(|E|*log(|E|)) as there is no bound for |E|
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
        // Variables for if the neighbour can ever be reached and for how long
        // we must wait to reach it if possible
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

/**
 * Returns the shortest path to the destination node, given by index, from a 
 * source node in the graph given as a list of nodes. This assumes that the 
 * dijkstra() function has been used on the graph with the source node.
 * Time complexity: O(|V|) 
 *  - this is because the longest shortest path never visits any node more than 
 *  once given positive edge costs, so the longest path can at most be |V| nodes long
 */
vector<int> shortest_path(vector<Node> &nodes, int destination) {
  vector<int> path;
  Node &curr_node = nodes[destination];

  // There is no calculated path from the source node to the destination node
  if (curr_node.previous == -1) {
    return {};
  }

  // Construct path backwards by following prevoius-pointer stored in the nodes
  int total_cost = curr_node.distance;
  path.push_back(destination);
  while (curr_node.previous != -1) {
    path.push_back(curr_node.previous);
    curr_node = nodes[curr_node.previous];
  }
  return path;
}

/**
 * Returns the cost of the shortest path to the destination node, given by index, 
 * from the source node in a graph for which the dijkstra() function has been called.
 * Time complexity: O(1)
 */
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

    // Construct and adjacency list for the graph
    // See the Edge struct above
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

