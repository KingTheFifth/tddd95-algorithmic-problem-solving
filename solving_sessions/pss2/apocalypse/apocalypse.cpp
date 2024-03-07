#include <algorithm>
#include <deque>
#include <iostream>
#include <ios>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using Edge = tuple<int, int, int>;
using State = tuple<int, int ,int>;
using Adj_list = vector<vector<Edge>>;

int solve(Adj_list &adj_list, vector<bool> &is_goal, int start, int group_size, int time_limit) {
  int total_saved = 0;
  deque<State> q = {{start, group_size, 0}};
  while (!q.empty()) {
    State s = q.front();
    q.pop_front();

    int node = get<0>(s);
    int p = get<1>(s);
    int t = get<2>(s);

    if (is_goal[node]) {
      total_saved += p;
      continue;
    }

    if (t+1 <= time_limit) {
      q.push_back({node, p, t+1});
    }

    for (Edge e : adj_list[node]) {
      int next = get<0>(e);
      int p_next = get<1>(e);
      int t_next = t + get<2>(e);
      if (t_next <= time_limit) {
        q.push_back({next, p_next, t_next});
      }
    }
  }

  return min(total_saved, group_size);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases;
  int nodes, start, group, time_limit, goals, x, edges, u, v, p ,t;
  cin >> cases;
  while (cases > 0) {
    cin >> nodes >> start >> group >> time_limit >> goals;
    if (cin.eof()) {
      break;
    }

    vector<bool> is_goal = vector<bool>(nodes);
    for (int g = 0; g < goals; ++g) {
      cin >> x;
      is_goal[x-1] = true;
    }

    cin >> edges;
    Adj_list adj_list = Adj_list(nodes);
    for (int e = 0; e < edges; ++e) {
      cin >> u >> v >> p >> t;
      adj_list[u-1].push_back({v-1, p, t});
    }

    cout << solve(adj_list, is_goal, start-1, group, time_limit) << "\n";
    cases--;
  }
  cout.flush();
}
