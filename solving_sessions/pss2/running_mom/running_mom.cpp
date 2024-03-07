#include <deque>
#include <iostream>
#include <ios>
#include "union_find.cpp"
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Adj_list = std::vector<std::vector<int>>;

bool dfs(int start, int nodes, Adj_list &adj_list, UnionSet &safe_cities, std::set<int> path) {
  if (safe_cities.get_depth(start) > 1) {
    return true;
  }

  if (path.find(start) != path.end()) {
    return true;
  }

  path.insert(start);
  for (int v : adj_list[start]) {
    if (dfs(v, nodes, adj_list, safe_cities, path)) {
      safe_cities.join(start, v);
      return true;
    }
  }

  return false;

  // deque<set<int>> q = {{start}};
  // while (!q.empty()) {
  //   set<int> p = q.back();
  //   q.pop_back();

  //   int end = *p.rbegin();
  //   for (int v : adj_list[end]) {
  //     // We have reached a city which is safe => all cities on this path 
  //     // are safe too => put in same union set of safe cities
  //     if (safe_cities.get_depth(v) > 1) {
  //         
  //     }
  //   }
  // }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string c1, c2;
  int edges;
  while (true) {
    std::cin >> edges;
    if (std::cin.eof()) {
      break;
    }

    std::unordered_map<std::string, int> city_to_node;
    Adj_list adj_list;
    int nodes = 0;
    for (int e = 0; e < edges; e++) {
     std::cin >> c1 >> c2; 
     // int space_pos = c1.find_first_of(" ");
     // c2 = c1.substr(space_pos+1, c1.length()-space_pos);
     // c1 = c1.substr(0, space_pos);
     

     if (city_to_node.find(c1) == city_to_node.end()) {
       city_to_node[c1] = nodes;
       adj_list.push_back({});
       nodes++;
     }

     if (city_to_node.find(c2) == city_to_node.end()) {
       city_to_node[c2] = nodes;
       adj_list.push_back({});
       nodes++;
     }

      adj_list[city_to_node.at(c1)].push_back(city_to_node.at(c2));
    }

    UnionSet safe_cities = UnionSet(nodes);
    while (true) {
      std::cin >> c1;
      if (std::cin.eof()) {
        break;
      }

      if (city_to_node.find(c1) == city_to_node.end()) {
        std::cout << c1 << " " << "trapped\n";
      }
      else {
        if (dfs(city_to_node.at(c1), nodes, adj_list, safe_cities, {})) {
          std::cout << c1 << " safe\n"; 
        }
        else {
          std::cout << c1 << " " << "trapped\n";
        }
      }
    }
  }
  std::cout.flush();
}
