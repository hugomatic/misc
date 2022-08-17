#include <time.h>
#include <cassert>
#include <set>
#include <iomanip>
#include <algorithm>

#include "mine.h"

void initMap(map_t &map, int bombCount) {
  using namespace std;

  size_t rows = map.size();
  size_t cols = map[0].size();

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
  size_t cols = map[0].size();
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

std::vector<std::tuple<size_t, size_t> > getNeighbors(size_t r,
                                                      size_t c,
                                                      size_t row,
                                                      size_t cols) {
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
  size_t rows = map.size();
  size_t cols = map[0].size();

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
      for (auto [rx, cx] : getNeighbors(r,c, rows, cols)) {
          if (map[rx][cx]  == 'x') {
            bombs ++;
          }
      }
      char v = '0' + bombs;
      changes.push_back(std::make_tuple(r,c,v));
      if (bombs == 0) {
        for (auto &p : getNeighbors(r, c, rows, cols)) {
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
    pr(os, map, true);
    return os;
}

void applyChanges(map_t &map, const changes_t &changes) {

  for(auto [r, c, v]: changes) {
    map[r][c] = v;
    std::cout << map;
  }
}

bool layout_t::inside(double x, double y, size_t &row, size_t &col) {
  for (size_t r=0; r < rows; ++r) {
    for (size_t c=0; c < cols; ++c) {
      double topX, topY, botX, botY;
      topLeft(r, c, topX, topY);
      bottomRight(r, c, botX, botY);
      if (x > topX && x < botX && y > botY && y < topY) {
        row = r;
        col = c;
        return true;
      }
    }
  }
  return false;
}

void layout_t::topLeft(size_t row, size_t col, double &x, double &y) {
  assert(row < rows && col < cols);
  x = x_topLeft + col * (dx + x_inter);
  y = y_topLeft + row * (dy + y_inter);
}

void layout_t::bottomRight(size_t row, size_t col, double &x, double &y) {
  double topX, topY;
  topLeft(row, col, topX, topY);
  x = topX + dx;
  y = topY + dy;
}

void layout_t::color(char value, bool gameOver, double &r, double &g, double &b) {
  r = 0;
  g = 0;
  b = 0;

  switch(value) {
    case '_':
      r = 0.5;
      g = 0.5;
      b = 0.5;
      break;
    case 'x':
      if (gameOver) {
        r = 1.0;
        g = 0.0;
        b = 0.0;
      }
      else {
        r = 0.5;
        g = 0.5;
        b = 0.5;
      }
      break;
    case '0':
        r = 0.125;
        g = 0.125;
        b = 0.125;
      break;
    case '1':
        r = 1.0;
        g = 0.25;
        b = 0.25;
      break;
    case '2':
        r = 0.375;
        g = 0.375;
        b = 0.375;
      break;
    case '3':
        r = 0.5;
        g = 0.5;
        b = 0.5;
      break;
    case '4':
        r = 0.625;
        g = 0.625;
        b = 0.625;
      break;
    case '5':
        r = 0.750;
        g = 0.750;
        b = 0.750;
      break;
    case '6':
        r = 0.875;
        g = 0.875;
        b = 0.875;
      break;
    case '7':
        r = 0.9;
        g = 0.9;
        b = 0.9;
      break;
    case '8':
        r = 1.0;
        g = 1.0;
        b = 1.0;
      break;
     case '!':
        r = 0.0;
        g = 0.0;
        b = 0.0;
      break;
   default:
      std::cout << "unkown value [" << value << "]" << std::endl;
      assert(0);
  }
}

