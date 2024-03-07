#include <algorithm>
#include <iostream>
#include <ios>
#include <unordered_set>
#include <vector>

using namespace std;
using Adj_list = vector<vector<int>>;

void dfs1(Adj_list &adj_list, vector<int> &order, vector<bool> &used, int node) {
  used[node] = true;
  for (int next : adj_list[node]) {
    if (!used[next]) {
      dfs1(adj_list, order, used, next);
    }
  }
  order.push_back(node);
}

void dfs2(Adj_list &adj_list_rev, vector<int> &component, vector<bool> &used, int node) {
  used[node] = true;
  component.push_back(node);
  for (int next : adj_list_rev[node]) {
    if (!used[next]) {
      dfs2(adj_list_rev, component, used, next);
    }
  }
}

int solve(int nodes, Adj_list &adj_list, Adj_list &adj_list_rev) {
  vector<bool> used = vector<bool>(nodes);

  vector<int> order;
  for (int n = 0; n < nodes; ++n) {
    if (!used[n]) {
      dfs1(adj_list, order, used, n);
    }
  }

  used.assign(nodes, false);
  reverse(order.begin(), order.end());

  vector<int> roots(nodes, 0);
  vector<int> root_nodes;
  vector<int> component;
  for (int n : order) {
    if (!used[n]) {
      dfs2(adj_list_rev, component, used, n);

      int root = component.front();
      for (int u : component) {
        roots[u] = root;
      }
      root_nodes.push_back(root);

      component.clear();
    }
  }

  // Create adjacency list of the graph where vertices are strongly connected 
  // component
  Adj_list adj_list_scc(nodes);
  for (int v = 0; v < nodes; ++v) {
    for (int u : adj_list[v]) {
      int root_u = roots[u];
      int root_v = roots[v];

      if (root_u != root_v) {
        adj_list_scc[root_v].push_back(root_u);
      }
    }
  }

  if (root_nodes.size() == 1) {
    return 0;
  }

  unordered_set<int> leaves;
  unordered_set<int> top_components;

  for (int u : root_nodes) {
    top_components.insert(u);
    leaves.insert(u);
  }

  for (int i = 0; i < adj_list_scc.size(); ++i) {
    for (int u : adj_list_scc[i]) {
      leaves.erase(i);
      top_components.erase(u);
    }
  }

  return max(leaves.size(), top_components.size());
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases, nodes, edges, u, v;
  cin >> cases;
  while (cases > 0) {
    cin >> nodes >> edges;

    Adj_list adj_list = Adj_list(nodes);
    Adj_list adj_list_rev = Adj_list(nodes);
    for (int e = 0; e < edges; ++e) {
      cin >> u >> v;
      adj_list[u-1].push_back(v-1);
      adj_list_rev[v-1].push_back(u-1);
    }
    
    cout << solve(nodes, adj_list, adj_list_rev) << "\n";
    cases--;
  }

  cout.flush();
}
