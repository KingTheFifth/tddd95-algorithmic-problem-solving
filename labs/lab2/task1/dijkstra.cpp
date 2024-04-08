/**
 * Johannes Kung johku144
 *
 * Single sorce shortest path with positive costs using Dijkstra's algorithm.
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

// Adjacency list of graph where a neighbour node is represented as a pair 
// consisting of the neighbour node index and cost to the neighbour node
using adj_list = vector<vector<pair<int, int>>>;

int VERY_LARGE_DISTANCE = 999999999;

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
 * all nodes in a graph with non-negative edge costs. Uses a Node struct to 
 * record reverse paths through a previous-node pointer and the cost to reach 
 * a node from the source. The node struct is also used to mark if a node has 
 * been visited.
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
void dijkstra(vector<Node> &nodes, adj_list &neighbours, int source) {
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

    // Mark this node as visited, thus setting its cost in stone as it will 
    // not be visited or considered again
    nodes[curr_node.index].visited = true;

    // Update distances to unvisited neighbour nodes for which we have encountered 
    // a cheaper path 
    // Also push such neighbour nodes to the priority queue of nodes to visit
    // Note: this may cause duplicates in the priority queue, which is solved
    // by checking if nodes have been marked as visited or not
    for (pair<int, int> neighbour : neighbours[curr_node.index]) {
      Node &n_node = nodes[neighbour.first];
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
  int N, M, S, Q, u, v, w, q;
  while (true) {
    cin >> N >> M >> Q >> S;
    if (cin.eof() || (N == 0 && M == 0 && S == 0 && Q == 0)) {
      break;
    }

    // Initialize nodes used for the graph representation
    vector<Node> nodes;
    for (int n = 0; n < N; n++) {
      nodes.push_back(Node(n));
    }

    // Construct an adjacency list for the graph 
    // A neighbour is represented as a pair of neighbour index and cost to the 
    // neighbour
    adj_list neighbours = adj_list(N);
    for (int e = 0; e < M; e++) {
      cin >> u >> v >> w;
      neighbours[u].push_back({v, w});
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
