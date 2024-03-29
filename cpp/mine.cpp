#include <iostream>
#include <array>
#include <time.h>
#include <cassert>
#include <tuple>
#include <set>
#include <queue>
#include <iomanip>
#include <algorithm>

const size_t rows=  20;
const size_t cols = 40;
const size_t bombsCount = 100;

typedef std::array<char, cols> row_t;
typedef std::array<row_t, rows> map_t;
typedef std::vector< std::tuple<size_t, size_t, char> > changes_t;

void initMap(map_t &map, int bombCount);
std::ostream& operator<<(std::ostream& os, const map_t& map);
std::tuple<bool, changes_t> click(map_t &map, size_t clickR, size_t clickC);

// Place a flag to mark it as dangerous

bool isWon(map_t const& map);

void initMap(map_t &map, int bombCount) {
  using namespace std;

  for (auto &row: map) {
    for(auto &c: row) {
      c = '_';
    }
  }
  int bombs = 0;
  while (bombs < bombCount) {
    size_t location = rand() % (rows * cols);
    size_t r = location / cols;
    size_t c = (location) % cols;
    assert(r < rows);
    assert(c < cols);
    if (map[r][c] != 'x') {
      map[r][c] = 'x';
    }
    bombs ++;
  }
}

// Reveal all safe squares without mines to win the game.
bool isWon(map_t const& map) {
  for (auto &row: map) {
    for(auto &c: row) {
      if (c == '_') {
        return false;
      }
    }
  }
  return true;
}

void pr(std::ostream& os, map_t const &map, bool gameOver) {
  os << "\n   ";
  for (size_t i=0; i < cols; ++i) {
    char c = ' ';
    if (i % 10 == 0) {
      c = size_t(i / 10) + '0';
    }
    os << c << " ";
  }
  os << "\n   ";
  for (size_t i=0; i < cols; ++i) {
    os << i % 10 << " ";
  }
  os << "\n";
  int count = 0;
  for (auto &row : map) {
    os << std::setw(2) << std::setfill('0') << count;
    os << " ";
    count ++;
    for(size_t i=0; i < row.size(); ++i) { // std::cout << row.size() << std::endl;
      char c = row[i];
      char x = c;

      if (!gameOver && c == 'x') {
        x = '_';
      }
      os << x << " ";
    }
    os << std::endl;
  }
}

void pr(map_t const &map, bool gameOver) {
  pr(std::cout, map, gameOver);
}

std::vector<std::tuple<size_t, size_t> > getNeighbors(size_t r, size_t c) {
  std::vector<std::tuple<size_t, size_t> > positions;
   if (r>0 ) {
    positions.push_back(std::make_tuple(r-1, c));
  }
  if (r< (rows-1)) {
    positions.push_back(std::make_tuple(r+1, c));
  }
  if (c< (cols-1)) {
    positions.push_back(std::make_tuple(r, c+1));
  }
  if (c >0) {
    positions.push_back(std::make_tuple(r, c-1));
  }
  if (c >0 && r >0) {
    positions.push_back(std::make_tuple(r-1, c-1));
  }
  if (c < (cols -1)  && r >0) {
    positions.push_back(std::make_tuple(r-1, c+1));
  }
  if (r < (rows -1) && c > 0) {
    positions.push_back(std::make_tuple(r+1, c-1));
  }
  if (r < (rows-1) && c < (cols -1)) {
    positions.push_back(std::make_tuple(r+1, c+1));
  }
  return positions;
}


std::tuple<bool, changes_t> click(map_t &map, size_t clickR, size_t clickC) {
  changes_t changes;

  if (map[clickR][clickC] == 'x') {
    map[clickR][clickC] = '!';
    return std::make_tuple(true, changes);
  }

  std::vector<std::tuple<size_t, size_t>>toVisit;
  std::set<std::tuple<size_t, size_t>> visited;

  toVisit.push_back(std::make_tuple(clickR, clickC));
  while (!toVisit.empty()) {
    auto [r,c] = toVisit.front();
    toVisit.erase(toVisit.begin());
    if (visited.find(std::make_tuple(r,c)) != visited.end()) {
      // already visited
      continue;
    }
    visited.insert(std::make_tuple(r,c));
    {
      size_t bombs = 0;
      for (auto [rx, cx] : getNeighbors(r,c)) {
          if (map[rx][cx]  == 'x') {
            bombs ++;
          }
      }
      char v = '0' + bombs;
      changes.push_back(std::make_tuple(r,c,v));
      if (bombs == 0) {
        for (auto &p : getNeighbors(r,c)) {
          auto [rx, cx] = p;
          if (map[rx][cx]  == '_') {
              if (visited.find(p) == visited.end()) {
                toVisit.push_back(p);
              }
          }
        }
      }
    }
  }
  return std::make_tuple(false, changes);
}

std::ostream& operator<<(std::ostream& os, const map_t& map)
{
    pr(os, map, false);
    return os;
}

void applyChanges(map_t &map, const changes_t &changes) {

  for(auto [r, c, v]: changes) {
    map[r][c] = v;
    std::cout << map;
  }
}


int main() {
  using namespace std;
  srand(1);
  map_t map;
  initMap(map, bombsCount);
  pr(map, true);

  while ( true) {
    size_t r = 0;
    size_t c = 0;
    std:: cout << "enter row: ";
    std::cin >> r;

    // print gameover board as a hint
    if (r > map.size()-1 ) {
      pr(map, true);
      continue;
    }
    std:: cout << "enter column: ";
    std::cin >> c;
    std::cout << "click [" << r << "," << c << "]" <<  std::endl;

    auto [gameOver, changes] = click(map, r, c);

    for (auto& [r,c,v]: changes) {
      std::cout << "[" << r << "," << c << "] " << v << std::endl;
    }

    for(auto [r, c, v]: changes) {
      map[r][c] = v;
      std::cout << map;
    }

    if (gameOver) {
      pr(map, true);
      std::cout << "BOOM!! Game over" << std::endl;
      break;
    }

    if (isWon(map)) {
      pr(map, true);
      std::cout << "Congratulations! You have won." << std::endl;
      break;
    }

    std::cout << map;
  }
}
