/**
 * Johannes Kung johku144
 *
 * Finding an eulerian path in a directed graph.
 *
 * Time complexity: O(|E|), see eulerian_path()
 */
#include <algorithm>
#include <iostream>
#include <ios>
#include <vector>
#include <deque>

using namespace std;
using Adj_list = vector<vector<int>>;

/**
 * Finds and returns a Eulerian path in a given graph if there is one. This 
 * path will actually be a Eulerian cycle if there is one in the graph.
 *
 * Input: an adjacency list for the graph and the number of edges
 * Output: a vector of node indices representing the Eulerian path/cycle,
 *         empty list of there is no such path/cycle
 *
 * Time complexity: O(|E|)
 */
vector<int> eulerian_path(Adj_list &adj_list, int edges) {
  // Calculate the in and out degrees of every node
  // O(|E|)
  vector<int> deg_in = vector<int>(adj_list.size());
  vector<int> deg_out = vector<int>(adj_list.size());
  for (int u = 0; u < adj_list.size(); ++u) {
    deg_out[u] = adj_list[u].size();
    for (int v : adj_list[u]) {
      deg_in[v]++;
    }
  }

  // Check the requirements for the existence of an eulerian path in the graph:
  // (This must hold for the existance but does not guarantee it)
  //
  // There should be at most one node with indegree + 1 == outdegree 
  // and at most one with outdegree + 1 == indegree for the graph to contain
  // an eulerian path. Save the indices of these two nodes
  //
  // Also all other nodes should have indegree == outdegree
  //
  // O(|V|)
  int an = -1;
  int bn = -1;
  for (int n = 0; n < deg_in.size(); ++n) {
    if (deg_in[n] + 1 == deg_out[n]) {
      if (an == -1) {
        an = n;
      }
      else {
        return {};
      }
    }
    else if (deg_out[n] + 1 == deg_in[n]) {
      if (bn == -1) {
        bn = n;
      }
      else {
        return {};
      }
    }
    else if (deg_in[n] != deg_out[n]) {
      return {};
    }
  }


  // Try to find an eulerian path by traversing every edge on all the paths 
  // from the starting node exactly once .This basically finds all simple cycles
  // (and perhaps also a path) and joins them into a big cycle (path)
  // This is sort of similar to Hierholzer's algorithm
  //
  // If there is a node with outdegree = indegree + 1, a.k.a. the node 'an',
  // then the path needs to start from this node and take the outgoing edge 
  // so that the the remaining edges of 'an' make for an indegree equal to the 
  // outdegree. There will not be any other way to traverse all edges of 'an' 
  // as we enter and leave any node an equal amount of times
  //
  // If there is no such node then it will be that all nodes have indegree == outdegree 
  // so we can start from any node (assuming there is an eulerian path)
  //
  // O(|E|) as a node is pushed for every edge incident with the node
  vector<int> cycle;
  deque<int> stack = {max(0, an)};
  while (!stack.empty()) {
    int node = stack.back();

    if (adj_list[node].empty()) {
      // The end of a simple cycle (or the single path in the graph) has been reached
      // Add this cycle to the the eulerian path and backtrack
      cycle.push_back(node);
      stack.pop_back();
    }
    else {
      int i = adj_list[node].size() - 1;
      stack.push_back(adj_list[node][i]);
      adj_list[node].pop_back();
    }
  }

  reverse(cycle.begin(), cycle.end());

  // If the found cycle is an eulerian cycle, then all edges of the graph must
  // have been used
  // If less edges have been used then there is no eulerian cycle
  if (cycle.size() < edges+1) {
    return {};
  }

  return cycle;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int nodes, edges, u, v;

  while (true) {
    cin >> nodes >> edges;
    if (cin.eof() || (nodes == 0 && edges == 0)) {
      break;
    }

    Adj_list adj_list = Adj_list(nodes);
    for (int e  = 0; e < edges; ++e) {
      cin >> u >> v;
      adj_list[u].push_back(v);
    }

    vector<int> path = eulerian_path(adj_list, edges);
    if (path.empty()) {
      cout << "Impossible\n";
    }
    else {
      for (int i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) {
          cout << " ";
        }
        else {
          cout << "\n";
        }
      }
    }
  }

  cout.flush();
}
