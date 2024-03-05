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

bool withinBounds(int x, int y, vector<vector<Tile>> &map) {
  return x >= 0 && x < map[0].size() && y >= 0 && y < map.size();
}

bool surrounded_by_trap(int x, int y, vector<vector<Tile>> &map) {
  return (withinBounds(x, y-1, map) && map[y-1][x].type == TileType::trap) ||
    (withinBounds(x, y+1, map) && map[y+1][x].type == TileType::trap) ||
    (withinBounds(x-1, y, map) && map[y][x-1].type == TileType::trap) ||
    (withinBounds(x+1, y, map) && map[y][x+1].type == TileType::trap);

}

int solve(int start_x, int start_y, vector<vector<Tile>> &map) {
  // Base case: trying to walk outside of map => stop!
  // (Should not happen, how did we get here?)
  if (!withinBounds(start_x, start_y, map)) {
    return 0;
  }

  // Base case: trying to revisit a cell => stop!
  // No more gold to be found here
  if (map[start_y][start_x].visited) {
    return 0;
  }

  // We now visit this tile
  map[start_y][start_x].visited = true;

  // Tally up the gold!
  int gold = 0;
  switch (map[start_y][start_x].type) {
    case TileType::empty: // Looking eerily empty in gold town...
      break;
    case TileType::wall:  // Inside a wall => stop!
    case TileType::trap:  // Should not happen! Cease this activity at once
      return 0;
    case TileType::gold:  // Here be gold!
      gold++;
      break;
  }

  // Stop the traversal if we are surrounded by traps
  // Tread lightly i.e. not at all
  if (surrounded_by_trap(start_x, start_y, map)) {
    return gold;
  }

  // No traps ahead, try all directions for gold 
  // Let us make it big!
  int sub_gold = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      // Exclude diagonal directions and tiles outside the map
      if (abs(dx+dy) == 1 && withinBounds(start_x+dx, start_y+dy, map)) {
        sub_gold += solve(start_x+dx, start_y+dy, map);
      }
    }
  }

  return gold + sub_gold;
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
