#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <ios>
#include <queue>
#include "union_find.cpp"

using namespace std;

using WeightedEdge = tuple<double, double, double>;
using Edge = tuple<double, double>;

double mst(vector<WeightedEdge> &edge_set, int vertices, vector<Edge> &result) {
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
  sort(edge_set.begin(), edge_set.end());

  double total_cost = 0;
  UnionSet connected_vertices = UnionSet(vertices);

  // Loop through the vertices in order of weight
  for (WeightedEdge e : edge_set) {
    double weight = get<0>(e);
    int v1 = get<1>(e);
    int v2 = get<2>(e);

    // Add edge e if it does not create a cycle, i.e. the end points of 
    // the edge are not in the same "equivalence" set
    if (!connected_vertices.same(v1, v2)) {
      connected_vertices.join(v1, v2);
      total_cost += pow(weight, 0.5);
      result.push_back({v1, v2});
    }
  }
  return total_cost;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int cases, islands;
  double x, y;
  cin >> cases;
  while (cases > 0) {
    cin >> islands;
    vector<tuple<double, double>> island_positions;
    vector<WeightedEdge> edge_set;

    for (int i = 0; i < islands; i++) {
      cin >> x >> y;
      island_positions.push_back({x, y});
      for (int j = 0; j < i; j++) {
        tuple<double, double> prev_island = island_positions[j];
        double delta_x = x - get<0>(prev_island); 
        double delta_y = y - get<1>(prev_island); 
        double distance_squared = delta_x*delta_x + delta_y*delta_y;
        edge_set.push_back({distance_squared, j, i});
      }
    }

    // for (int i = 0; i < islands; i++) {
    //   for (int j = i+1; j < islands; j++) {
    //     tuple<double, double> island_1 = island_positions[i];
    //     tuple<double, double> island_2 = island_positions[j];

    //     double delta_x = get<0>(island_1) - get<0>(island_2);
    //     double delta_y = get<1>(island_1) - get<1>(island_2);
    //     double distance_squared = delta_x*delta_x + delta_y*delta_y;

    //     edge_set.push_back({distance_squared, i, j});
    //   }
    // }

    vector<Edge> mst_result;
    double total_cost = mst(edge_set, islands, mst_result);
    cout << std::fixed << setprecision(2) << total_cost << "\n";
    cases--;
  }

  cout.flush();
}

