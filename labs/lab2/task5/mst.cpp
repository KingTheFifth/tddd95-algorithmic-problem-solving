/**
 * Johannes Kung johku144
 *
 * Minimal spanning tree construction using Kruskall's algorithm.
 *
 * Time complexity: O(|E|*log(|E|)), see mst().
 *
 */
#include <algorithm>
#include <iostream>
#include <ios>
#include "union_find.cpp"
#include <tuple>

using namespace std;

using WeightedEdge = tuple<int, int, int>;
using Edge = tuple<int, int>;

/**
 * An implementation of Kruskall's algorithm using the data structure for 
 * Union-Find to keep track of which edges that create cycles.
 *
 * Input: A graph as an edge set and the number of vertices,
 *        a vector of edges to store the resulting MST in.
 *
 * Output: A MST of the given graph stored as a list of the edges of the MST.
 *         The edges are stored in the given result vector.
 *
 * Time complexity: O(|E|*log(|E|)+|E|*alpha(|V|)) = O(|E|*log(|E|))
 */
int mst(vector<WeightedEdge> &edge_set, int vertices, vector<Edge> &result) {
  // total cost = 0
  // While #num edges chosen < |V(G)|-1
  //  Pop chepeast edge 
  //  If both end points in same union set 
  //    => cycle if add => skip
  //  Else 
  //    add edge to MST
  //    add cost to total cost
  //    Join union sets of both endpoints
  // Return used edges & total cost

  // Sort edges lexicographically
  // Since weight is the first value in the edge tuple, this will sort 
  // on edge weights in ascending order
  // O(|E|*log(|E|))
  sort(edge_set.begin(), edge_set.end());

  int total_cost = 0;
  UnionSet connected_vertices = UnionSet(vertices);

  // Loop through the vertices in order of weight
  // => always pick the chepeast edge that does not cause a cycle
  // O(|E|*alpha(|V|))
  for (WeightedEdge e : edge_set) {
    int weight = get<0>(e);
    int v1 = get<1>(e);
    int v2 = get<2>(e);

    // Add edge e if it does not create a cycle, i.e. the end points of 
    // the edge are not in the same "equivalence" set
    // Time complexity of Union-Find same operation: O(alpha(|V|))
    if (!connected_vertices.same(v1, v2)) {
      // Union-Find join operation with time complexity O(alpha(|V|))
      connected_vertices.join(v1, v2);
      total_cost += weight;
      result.push_back({v1, v2});
    }
  }
  return total_cost;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int nodes, edges, n1, n2, w;
  while (true) {
    cin >> nodes >> edges;
    if (cin.eof() || (nodes == 0 && edges == 0)) {
      break;
    }

    // Save edges in nice format - tuple (weight, u, v)
    // Keep this in a priority queue sorted on lowest weight 
    vector<WeightedEdge> edge_set;
    for (int e = 0; e < edges; e++) {
      cin >> n1 >> n2 >> w;
      edge_set.push_back({w, min(n1, n2), max(n1, n2)});
    }

    // Special case: A graph needs at least |V(G)|-1 edges to be connected
    // and thus have a minimal spanning tree
    if (edges < nodes - 1) {
      cout << "Impossible\n";
    }

    else {
      vector<Edge> resulting_mst;
      int total_cost = mst(edge_set, nodes, resulting_mst);
      sort(resulting_mst.begin(), resulting_mst.end());

      // A minimal spanning tree has exactly |V(G)| - 1 edges 
      // If the algorithm returns an MST with less edges, then the original
      // graph was not connected => impossible to find MST
      if (resulting_mst.size() != nodes - 1) {
        cout << "Impossible\n";
      }

      else {
        cout << total_cost << "\n";
        for (Edge e : resulting_mst) {
          cout << get<0>(e) << " " << get<1>(e) << "\n";
        }
      }
    }

  }

  cout.flush();
}
