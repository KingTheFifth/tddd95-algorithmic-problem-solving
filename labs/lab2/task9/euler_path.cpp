#include <algorithm>
#include <iostream>
#include <ios>
#include <vector>
#include <deque>

using namespace std;
using Adj_list = vector<vector<int>>;

vector<int> eulerian_path(Adj_list &adj_list, vector<int> &deg_in, vector<int> &deg_out, int edges) {
  // There should be at most one node with indegree + 1 == outdegree 
  // and at most one with outdegree + 1 == indegree for the graph to contain
  // an eulerian path
  // Also all other nodes should have indegree == outdegree
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


  // Try to find an eulerian cycle
  vector<int> cycle;
  deque<int> stack = {max(0, an)};
  while (!stack.empty()) {
    int node = stack.back();

    if (adj_list[node].empty()) {
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

  // All edges of the graph have to be used or else the result is not eulerian 
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

    vector<int> deg_in = vector<int>(nodes);
    vector<int> deg_out = vector<int>(nodes);
    Adj_list adj_list = Adj_list(nodes);
    for (int e  = 0; e < edges; ++e) {
      cin >> u >> v;
      deg_out[u]++;
      deg_in[v]++;
      adj_list[u].push_back(v);
    }

    vector<int> path = eulerian_path(adj_list, deg_in, deg_out, edges);
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
