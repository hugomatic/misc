#include <cstring>
#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <queue>

void pr(int const (&grid)[10][10]) {
  using namespace std;
  cout << "   ";
  for (size_t i=0; i < 10; i++) {
    cout << i << " ";
  }
  cout << endl;
  for (size_t i=0; i < 10; i++) {
    cout << i << ": ";
    for (int j = 0; j < 10; j++ ) {
      cout << grid[i][j] << " ";
    }
    cout << endl;
  }
}

int shortest_path(int const (&grid)[10][10], int x0, int y0) {
  using namespace std;

  queue<tuple<int,int,int>> to_visit;
  set<tuple<int,int>> visited;
  to_visit.push(make_tuple(x0, y0, 0));
  while (!to_visit.empty()) {
    auto &pos = to_visit.front();
    auto [x, y, d] = pos;
    to_visit.pop();
    tuple<int, int> coords = make_tuple(x,y);
    if (visited.find(coords) == visited.end()) {
      visited.insert(coords);
      if (grid[x][y] == 1) {
        return d;
      }

      tuple<int, int, int> news[4] = {make_tuple(x-1, y, d+1),
                                      make_tuple(x+1,y, d+1),
                                      make_tuple(x, y-1, d+1),
                                      make_tuple(x, y+1, d+1)};
      for (auto nb : news) {
        auto [x,y,d] = nb;
        if (x >=0 && x < 10 && y >=0 && y < 10) {
          to_visit.push(nb);
        }
      }
    }
  }

  return 0;
}

int main(int argc, char **argv)
{
  using namespace std;
  int grid[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
  };


  cout << "yo" << endl;

  int x0, y0;
  for (size_t i=0; i < argc; i++) {
    string arg = argv[i];

    cout << i <<  ") "<<  arg << endl;

    if (i > 0) {
      char s[2];
      s[0] = arg[0];
      s[1] = '\0';
      int x = atoi(s);
      s[0] = arg[2];
      int y = atoi(s);
      if (i == 1) {
        grid[x][y] = 2;
        x0 = x;
        y0 = y;
      }
      else {
        grid[x][y] = 1;
      }
    }
  }


  pr(grid);

  int sp = shortest_path(grid, x0, y0);
  cout << "shortest path form " << x0 << "," << y0 << " is " << sp << endl;
  return 0;
}
