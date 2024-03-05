#include <deque>
#include <iostream>
#include <ios>
#include <valarray>
#include <vector>

using namespace std;

enum TileType {
  wall,
  gold,
  trap,
  empty
};

struct Tile {
  bool visited = false;
  TileType type = TileType::empty;
  Tile () {
    visited = false;
    type = TileType::empty;
  }
  Tile (TileType type) {
    this->type = type;
  }
};

bool within_bounds(int x, int y, vector<vector<Tile>> &map) {
  return x >= 0 && x < map[0].size() && y >= 0 && y < map.size();
}

bool surrounded_by_trap(int x, int y, vector<vector<Tile>> &map) {
  return (within_bounds(y-1, x, map) && map[y-1][x].type == TileType::trap) ||
    (within_bounds(y+1, x, map) && map[y+1][x].type == TileType::trap) ||
    (within_bounds(y, x-1, map) && map[y][x-1].type == TileType::trap) ||
    (within_bounds(y, x+1, map) && map[y][x+1].type == TileType::trap);

}

vector<pair<int, int>> neighbours_of(int x, int y, vector<vector<Tile>> &map) {
  vector<pair<int, int>> res;
  if (within_bounds(x-1, y, map)) {
    res.push_back({x-1, y});
  }
  if (within_bounds(x+1, y, map)) {
    res.push_back({x+1, y});
  }
  if (within_bounds(x, y-1, map)) {
    res.push_back({x, y-1});
  }
  if (within_bounds(x, y+1, map)) {
    res.push_back({x, y+1});
  }
  // for (int dx = -1; dx <= 1; dx++) {
  //   for (int dy = -1; dy <= 1; dy++) {
  //     if (abs(dx+dy) == 1 && within_bounds(x, y, map)) {
  //       res.push_back({x+dx, y+dy});
  //     }
  //   }
  // }
  return res;
}

int solve(int start_x, int start_y, vector<vector<Tile>> &map) {
  int gold = 0;
  deque<pair<int, int>> to_visit = {{start_x, start_y}};
  while (!to_visit.empty()) {
    pair<int, int> coord = to_visit.front();
    Tile &tile = map[coord.second][coord.first];
    to_visit.pop_front();

    tile.visited = true;

    if (tile.type == TileType::trap || tile.type == TileType::wall) {
      continue;
    }

    if (tile.type == TileType::gold) {
      gold++;
    }

    if (!surrounded_by_trap(coord.first, coord.second, map)) {
      // Visit neighbours of this coord
      for (pair<int, int> n : neighbours_of(coord.first, coord.second, map)) {
        if (!map[n.second][n.first].visited) {
          to_visit.push_back(n);
        }
      }
    }

  }
  return gold;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int W, H;
  while (true) {
    cin >> W >> H;
    if (cin.eof()) {
      break;
    }
    vector<vector<Tile>> map;
    char in;
    int start_x, start_y;
    for (int h = 0; h < H; h++) {
      map.push_back(vector<Tile>());
      for(int w = 0; w < W; w++) {
        cin >> in;
        TileType type;
        if (in == '#') {
          type = TileType::wall;
        }
        else if (in == '.') {
          type = TileType::empty;
        }
        else if (in == 'T') {
          type = TileType::trap;
        }
        else if (in == 'G') {
          type = TileType::gold;
        }
        else if (in == 'P') {
          start_x = w;
          start_y = h;
          type = TileType::empty;
        }
        map[h].push_back(Tile(type));
      }
    }
    cout << solve(start_x, start_y, map) << "\n";
  }
  cout.flush();
}
