#include <iostream>
#include <array>
#include <time.h>
#include <cassert>
#include <tuple>
#include <set>
#include <queue>
#include <iomanip>
#include <algorithm>

const size_t rows=20;
const size_t cols =40;
typedef std::array<char, cols> row_t;
typedef std::array<row_t, rows> map_t;

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


void pr(std::ostream& os, map_t const &map, bool gameOver) {

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

bool click(map_t &map, size_t clickR, size_t clickC) {

  std::cout << "Click [" <<  clickR << ", " << clickC <<"]\n";
  if (map[clickR][clickC] == 'x') {
    map[clickR][clickC] = '!';
    return true;
  }

  std::vector<std::tuple<size_t, size_t> >toVisit;

  toVisit.push_back(std::make_tuple(clickR, clickC));
  while (!toVisit.empty()) {
    auto [r,c] = toVisit.front();
    toVisit.erase(toVisit.begin());
    if (map[r][c] != '_') {
      continue;
    }
    else {
      size_t bombs = 0;
      for (auto [rx, cx] : getNeighbors(r,c)) {
          if (map[rx][cx]  == 'x') {
            bombs ++;
          }
      }
      map[r][c] = '0' + bombs;
      // pr(map, true);
      if (bombs == 0) {
        for (auto &p : getNeighbors(r,c)) {
          auto [rx, cx] = p;
          if (map[rx][cx]  == '_') {
            if ( std::find(toVisit.begin(), toVisit.end(), p) == toVisit.end())
              toVisit.push_back(p);
          }
        }
      }
    }
  }
  return false;
}

std::ostream& operator<<(std::ostream& os, const map_t& map)
{
    pr(os, map, false);
    return os;
}


int main() {
  using namespace std;
  srand(1);
  map_t map;
  initMap(map, 100);
  pr(map, true);

  while ( true) {
    size_t r = 0;
    size_t c = 0;
    std:: cout << "enter row: ";
    std::cin >> r;
    // print gameover board as a hint
    if (r > map.size() ) {
      pr(map, true);
      continue;
    }
    std:: cout << "enter column: ";
    std::cin >> c;
    std::cout << "click [" << r << "," << c << "]" <<  std::endl;

    if ( click(map, r, c) ) {
      pr(map, true);
      std::cout << "BOOM!! Game over" << std::endl;
      break;
    }
    std::cout << map;
  }
}
