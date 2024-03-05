/**
 * Johannes Kung johku144
 *
 * Maximum flow using Edmonds-Karp
 *
 * Time complexity: O(|V|*|E|^2) (see comments for the function max_flow() )
 * Memory consumption: O(|V|^2+|V|+|E|+|V|) i.e. O(|V|^2+|E|)
 *  - Adjacency list O(|V|+|E|)
 *  - Capacities matrix O(|V|^2)
 *  - Augmenting path vector O(|V|)
 */
#include <iostream>
#include <ios>
#include <limits>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>
#include <deque>

using namespace std;
using adj_list = vector<vector<int>>;

// Capacities matrix of flow network
// First of pair is edge capacity, second is residual capacity
using net_capacities = vector<vector<pair<int, int>>>;

// List of flows between pairs of nodes, reresented using tuple
// First is node 1, second is node 2, third is flow from node 1 to node 2
using vec_flows = vector<tuple<int, int, int>>;

/**
 * Performs BFS on given flow network to find an augmenting path in the 
 * corresponding residual capacity network as well as the lowest minimal flow 
 * along this path
 *
 * Input: flow network as adjacency list, matrix of capacities, source and sink nodes
 * Output: Augmenting path and the flow to increase with along the path 
 *  - Path outputted by modifying given vector called "parent"
 *
 * Time complexity: O(|E|) as every edge may be visited once
 * Memory complexity: O(|V|) as the previous node in the path is stored for 
 *                    every node
 */
int find_flow_increase(
    adj_list &neighbour_set,
    net_capacities &capacities,
    vector<optional<int>> &parent,
    int s,
    int t
    ) {
  // Use BFS to find an augmenting path in the residual capacity network 
  // consisting of all edges with residual capacity > 0
  // An augmenting path is any path in the residual network from the source (s)
  // to the sink (t)
  // Any such path can increase the flow. The flow increase is bounded by the edge 
  // in the path with the lowest residual capacity
  // This flow increase is returned

  // Use a vector to keep track of the path from s to t backwards, i.e. starting 
  // at t and then following t's parent to the previous node in the path etc
  // Memory consumption O(|V|) as the previous node (if any) in the path is stored 
  // for every vertex
  fill(parent.begin(), parent.end(), optional<int>());
  parent[s] = {-1};

  // Use BFS to find a path from s to t, time complexity O(|E|) as every edge 
  // may be visited in the worst case
  // The lowest residual capacity seen on a path is kept track of together 
  // with the node to visit next
  deque<pair<int, int>> queue;
  queue.push_back({s, numeric_limits<int>::max()});
  while (!queue.empty()) {
    int node = queue.front().first;
    int flow = queue.front().second;
    queue.pop_front();

    for (int neighbour : neighbour_set[node]) {
      // Only visit the neighbour node through this edge if it has not been 
      // visited before (has no parent) AND its residual capacity is > 0
      if (!parent[neighbour].has_value() && capacities[node][neighbour].second > 0) {
        parent[neighbour] = node;

        // Update the lowest residual capacity seen on this path
        int new_flow = min(flow, capacities[node][neighbour].second);

        // We have reached the goal node t => return the lowest residual capacity 
        // found on this path
        if (neighbour == t) {
          return new_flow;
        }
        queue.push_back({neighbour, new_flow});
     }
    }
  }

  // No path to t was found => there is no augmenting path in the flow network 
  // => the flow cannot be increased, signified by return value 0
  return 0;
}

/**
 * Increases the flow of every edge on the path fom s to t stored in the "parent"
 * vector by the given flow. This modifies the given capacities vector.
 *
 * Input: capacities matrix of flow network, path, flow to increase with, 
 *        source and sink nodes
 *
 * Time complexity O(|E|) as the path traversed is at most |E| edges.
 */
void increase_flow(net_capacities &capacities, vector<optional<int>> &parent, int flow, int s, int t) {
  int curr = t;
  int prev;
  while (curr != s) {
   prev = parent[curr].value();
   capacities[curr][prev].second += flow;
   capacities[prev][curr].second -= flow;
   curr = prev;
  }
}

/**
 * Implements the Edmonds-Karp algorithm to solve the max flow problem for a 
 * given flow network. This modifies the given capacities matrix.
 *
 * Input: flow network as adjacency list and capacities matrix, 
 *        source and sink nodes
 * Output: Max flow of the network, list of edges and their flows for each edge 
 *         used in the max flow solution
 *
 * Time complexity: O(|V|*|E|^2)
 * Memory consumption: O(|E|) for a vector to store augmenting paths 
 *  - ( O(|V|^2) for the modifications of the capacities matrix)
 */
pair<int, vec_flows> max_flow(adj_list &neighbour_set, net_capacities &capacities, int s, int t) {
  int total_flow = 0;
  vector<optional<int>> parent = vector<optional<int>>(neighbour_set.size());

  // Edmonds-Karp: continuously find an augmenting path in the flow network together
  // with the increase in flow that can be done on that path, 
  // then icrease the flow on that path by that amount
  // Repeat until the flow cannot be increased any more, i.e. there is no 
  // augmenting path
  //
  // Any edge of the network can appear on an augmenting path at most O(|V|) times.
  // In the worst case exactly one edge on an augmenting path gets its flow set 
  // to max and disappears from the residual capacity graph. As such, this 
  // while-loop is executed O(|V|*|E|) times with a cost of O(|E|) each iteration.
  // This yields a time complexity of O(|V|*|E|^2).
  // A more detailed proof can be found at e.g. brilliant.org/wiki/edmonds-karp-algorithm
  int flow_increase = find_flow_increase(neighbour_set, capacities, parent, s, t);
  while (flow_increase > 0) {
    total_flow += flow_increase;
    increase_flow(capacities, parent, flow_increase, s, t);
    flow_increase = find_flow_increase(neighbour_set, capacities, parent, s, t);
  }

  // Loop through all edges in the (undirected) flow network to find 
  // the edges with a flow > 0
  // The flow is the same as the edge capacity subtracted by the residual (unused) capacity
  // Time and memory complexity O(|E|) as every edge is considered and may be stored
  vec_flows flows;
  for (int u = 0; u < neighbour_set.size(); u++) {
    for (int v = 0; v < neighbour_set.size(); v++) {
      int capacity = capacities[u][v].first;
      int flow = capacity - capacities[u][v].second;
      if (flow > 0) {
        flows.push_back({u, v, flow});
      }
    }
  }

  return {total_flow, flows};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int nodes, edges, source, sink, u, v, c;
  while(true) {
    cin >> nodes >> edges >> source >> sink;
    if (cin.eof()) {
      break;
    }

    // Initialise the 2D-vector of capacities and residual capacities between 
    // pairs of nodes in the flow network (this is sort of an adjacency matrix)
    // Memory consumption O(|V|^2)
    net_capacities capacities = net_capacities(nodes);
    for (int n = 0; n < nodes; n++) {
      capacities[n].resize(nodes);
      //fill(capacities[n].begin(), capacities[n].end(), pair<int, int>(0, 0));
    }
    
    // Construct an adjacency list of the network and set values in the 
    // capacities matrix
    // Memory consumption O(|V|+|E|) for adjacency list
    adj_list neighbour_set = adj_list(nodes);
    for (int e = 0; e < edges; e++) {
      cin >> u >> v >> c;
      // The adjacency representation of the graph needs to be undirected
      // for finding augmenting paths
      neighbour_set[u].push_back(v);
      neighbour_set[v].push_back(u);

      // The matrix of residual capacities is directed
      capacities[u][v].first = c;
      capacities[u][v].second = c;
    }

    // Calculate and output the max flow of the network
    pair<int, vec_flows> result = max_flow(neighbour_set, capacities, source, sink);
    cout << nodes << " " << result.first << " " << result.second.size() << "\n";
    for (tuple<int, int, int> &flow : result.second) {
      cout << get<0>(flow) << " " << get<1>(flow) << " " << get<2>(flow) << "\n";
    }
  }
  cout.flush();
}
