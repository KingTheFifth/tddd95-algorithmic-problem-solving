/**
 * Johannes Kung johku144
 *
 * All pairs shortest path using the Floyd-Warhsall algorithm.
 *
 * Time complexity: O(|V|^3)
 */
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ios>
#include <limits>
#include <vector>

using namespace std;
using i64 = int64_t;
using vec_i64_2d = vector<vector<i64>>;

i64 INFTY = numeric_limits<int>::max();

/**
 * A straightforward implementation of the floyd-warshall algorithm for the 
 * all pairs shortest path problem.
 *
 * Input: A graph respresented by a matrix of distances between pairs of nodes.
 *        This matrix is initially an adjacency matrix.
 *
 * Output: The cost of the shortest path between all pairs of nodes, stored 
 *         in the given distance matrix
 *
 * Time complexity: O(|V|^3)
 */
void floyd_warshall(vec_i64_2d &dist) {
  int nodes = dist.size();
  for (int k = 0; k < nodes; ++k) {
    for (int u = 0; u < nodes; ++u) {
      for (int v = 0; v < nodes; ++v) {
        if (dist[u][k] < INFTY && dist[k][v] < INFTY) {
          dist[u][v] = max(-INFTY, min(dist[u][v], dist[u][k] + dist[k][v]));
        }
      }
    }
  }

  // Handle negative cycles
  for (int u = 0; u < nodes; ++u) {
    for (int v = 0; v < nodes; ++v) {
      for (int t = 0; t < nodes; ++t) {
        if (dist[u][t] < INFTY && dist[t][v] < INFTY && dist[t][t] < 0) {
          dist[u][v] = -INFTY;
        }
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int N, M, Q, u, v, s, g;
  i64 w;
  while (true) {
    cin >> N >> M >> Q;
    if (cin.eof() || (N == 0 && M == 0 && Q == 0)) {
      break;
    }

    // Initialize the distance matrix
    // Note that the distance for a node to itself is directly set to 0 here
    vec_i64_2d dist = vec_i64_2d(N);
    for (int u = 0; u < N; u++) {
      dist[u].resize(N);
      for (int v = 0; v < N; v++) {
        if (u == v) {
          dist[u][v] = 0;
        }
        else {
          dist[u][v] = INFTY;
        }
      }
    }

    for (int e = 0; e < M; e++) {
      cin >> u >> v >> w; 

      // Kattis uses multigraphs, but it is only necessary to store the lowest 
      // cost between two vertices since a higher cost repeated edge will 
      // not be used in the shortest path solution
      // This nicely handles the case of loops too
      if (w < dist[u][v]) {
        dist[u][v] = w;
      }
    }

    floyd_warshall(dist);

    for (int i = 0; i < Q; i++) {
      cin >> s >> g;
      if (dist[s][g] == -INFTY) {
        cout << "-Infinity\n";
      }
      else if (dist[s][g] == INFTY) {
        cout << "Impossible\n";
      }
      else {
        cout << dist[s][g] << "\n";
      }
    }
    cout << "\n";
  }
  cout.flush();
}
