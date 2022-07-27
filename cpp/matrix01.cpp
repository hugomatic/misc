#include <iostream>
#include <vector>
#include <set>
#include <array>
#include <tuple>
#include <queue>

using namespace std;

// Given an m x n binary matrix mat, return the distance of the nearest 0 for each cell.
//
// The distance between two adjacent cells is 1.
//
// Example 1:
//
// Input: mat = [[0,0,0],[0,1,0],[0,0,0]]
// Output: [[0,0,0],[0,1,0],[0,0,0]]
//

struct pos_t{
  int r;
  int c;
};

struct compare_t {
  bool operator() (pos_t a, pos_t b) const {
    bool res = a.r < b.r && a.c < b.c;
    // cout << "compare " << a.r << ", " << a.c << " with " << b.r << ", " << b.c << " = " << res<< endl;
    return res;
  }
};

typedef set<pos_t, compare_t> visited_t;


typedef array<array<int, 3>, 3> matrix_t;

void pr(matrix_t const & mat ) {
   for (auto &row : mat) {
      cout << "[";
      for (auto &col: row) {
          cout << col << ", ";
      }
      cout << "]" << endl;
   }
}


bool found(set<tuple<int,int> > const & visited, tuple<int,int> e) {
  auto found = visited.find(e);
  bool r = found == visited.end();
  return r;
}
/*
int closestZero_(matrix_t const& mat, int r, int c) {
  set<tuple<int, int> > visited;

  visited.insert({0,0});
  visited.insert({1,1});

  cout << "xx1 " << found(visited, {1,1} ) << endl;
  cout << "xx0 " << found(visited, {0,1} ) << endl;
  cout << "visited" << endl;
  for (auto &[r,c]: visited) {
    cout <<  "   " << r << ", " << c << endl;
  }
  return 42;
}
*/
void pr(set<tuple<int,int> > const &visited) {
  cout << "visited list ";
  for (auto &[r,c]: visited) {
    cout << "[" << r << "," << c << "] ";
  }
  cout << endl;
}

void pr(queue< tuple<pos_t, double> > const &toVisit) {
  queue< tuple<pos_t, double> > x =  toVisit;
  cout << "to visit: ";
  while(!x.empty()) {
    auto node = x.front();
    auto [pos, d] = node;
    cout << "[" << pos.r << ", " << pos.c << "] d = " << d << ", ";
    x.pop();
  }
  cout << endl;
}

int closestZero(matrix_t const& mat, int r, int c) {
  pos_t root{r,c};
  queue< tuple<pos_t, double> >to_visit;
  to_visit.push(make_tuple(root,0)) ;
  set<tuple<int, int> > visited;
  while (!to_visit.empty()) {
    auto[pos,d] = to_visit.front();
    to_visit.pop();
    visited.insert(make_tuple(pos.r, pos.c));
    int val = mat[pos.r][pos.c];
    // pr(to_visit);
    // pr(visited);
    if (val == 0){
      return d;
    }
    // not zero, look at the neighbors:
    std::vector<pos_t> newsNeighbors={ {pos.r-1, pos.c}, {pos.r, pos.c-1}, {pos.r, pos.c+1}, {pos.r+1, pos.c}};
    for (auto &p : newsNeighbors) {
      if ( (p.r >=0 ) && (p.r < int(mat.size()))  && (p.c >= 0 ) && (p.c < int(mat[0].size() ))) {
        if (visited.find(make_tuple(p.r, p.c)) == visited.end() ) {
           to_visit.push(make_tuple(p, d +1));
        }
        else {
        }
      }
      else {
      }
    }
  }
  return -99;
}

matrix_t closestZeros(matrix_t const&  mat) {
  matrix_t res;
  for (size_t i=0; i < mat.size(); ++i) {
    auto & row = mat[i];
    for (size_t j=0; j< row.size(); ++j) {
        res[i][j] = closestZero(mat, i,j);
    }
  }
  return res;
}

void example(matrix_t mat) {
  cout << "example " << endl;
  pr(mat);
  cout << "result" << endl;
  pr(closestZeros(mat));
  cout << "-------------" << endl;
}

int main() {
  example( { {{0,0,0}, {0,1,0},{1,1,1}} });

}
