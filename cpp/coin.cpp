#include <iostream>
#include <array>
#include <tuple>
#include <vector>
#include <queue>
#include <set>

using namespace std;

/*
char  map[6][6] ={{ '.', 'x', '.', '.', '.', '.'},
                  { '.', '.', '.', '.', '.', '.'},
                  { '.', '.', '.', '.', '.', '.'},
                  { '.', '.', '.', '.', '.', '.'},
                  { '.', '.', '.', '.', '.', '.'},
                  { '.', '.', '.', '.', '.', '.'}};

void pr(char map[][6], int nbR, nbC ) {
  cout << "pr" << endl;
  for (size_t r=0; r < r, ++r)
}


*/

typedef std::array<char, 6> row_t;
typedef std::array<row_t, 6> map_t;

map_t map =  {{
  { '.', 'x', '.', '.', '.', '.'},
  { '.', '.', '.', 'o', '.', '.'},
  { 'o', '.', '.', '.', '.', '.'},
  { '.', '.', '.', '.', '.', '.'},
  { '.', '.', '.', '.', 'o', '.'},
  { '.', '.', '.', '.', '.', '.'}
}};

void pr(map_t const& map) {
  using namespace std;

  for (auto &row: map) {
    for (auto &col: row) {
      cout << col << " ";
    }
    cout << endl;
  }
}

typedef std::tuple<int, int> pos_t;
typedef std::vector<pos_t > coins_t;

coins_t brute_all_coins(const map_t& map) {
  using namespace std;

  coins_t coins;
  std::tuple<size_t, size_t> start;

  for (size_t r=0; r < map.size(); ++r) {
    auto &row = map[r];
    for (size_t c=0; c < row.size(); ++c) {
      if (map[r][c] == 'x') {
        cout << "start [" << r << ", " << c << "]" << endl;
        start = make_tuple(r,c);
      }
      else if (map[r][c] == 'o') {
        coins.push_back(make_tuple(r,c));
      }
    }
  }

  for (auto &coin: coins) {
    get<0>(coin) -= get<0>(start);
    get<1>(coin) -= get<1>(start);
  }
  return coins;
}


pos_t closest_coin(const pos_t &start, const map_t& map) {

  typedef std::tuple<int, int, int> node_t;
  std::queue< node_t > to_visit;
  auto [r,c] = start;
  to_visit.push(std::make_tuple(r,c, 0));

  std::set<node_t> seen;

  while (!to_visit.empty()) {
     node_t node = to_visit.front();
     auto[r, c, dist] = node;
     to_visit.pop();
     seen.insert(node);
     std::cout << " - visit [" << r << ", " << c << "] " << dist  << "\n";
     if (map[r][c] == 'o') {
        std::cout << "found coin not too far " << dist << std::endl;
        return std::make_tuple(r,c);
     }
     node_t north = std::make_tuple(r-1,c, dist+1);
     if (r-1 >= 0 && seen.find(north) == seen.end()) {
        std::cout << " -- add north" << std::endl;
        to_visit.push(north);
     }
     node_t south = std::make_tuple(r+1,c, dist+1);
     if (r+1 < int(map.size()) -1 && seen.find(south) == seen.end()) {
        std::cout << " -- add south" << std::endl;
        to_visit.push(south);
     }
     node_t west = std::make_tuple(r, c -1 , dist+1);
     if (c >= 0 && c < int(map[0].size()) -1 && seen.find(west) == seen.end()) {
        std::cout << " -- add west" << std::endl;
        to_visit.push(west);
     }
     node_t east = std::make_tuple(r, c +1 , dist+1);
     if (c >= 0 && c < int(map[0].size()) -1 && seen.find(east) == seen.end()) {
        std::cout << " -- add east" << std::endl;
        to_visit.push(east);
     }

  }
  std::cout << "No coins found!" << std::endl;
  return std::make_tuple(-1, -1);
}

int main() {
  std::cout << "hello" << endl;
  pr(map);

  auto coins = brute_all_coins(map);
  std::cout << coins.size() << " coins" << std::endl;
  for (auto &coin: coins) {
     int d = std::abs(std::get<0>(coin)) + std::abs(std::get<1>(coin));
     std::cout << "coin [" << std::get<0>(coin) << ", " << std::get<1>(coin) << "]"
       << " " << d << std::endl;
  }
  auto closest = closest_coin(std::make_tuple(0, 1), map);
  auto [r,c] = closest;
  std::cout << "closest coin [" << r << ", " << c << "]" << std::endl;
}


