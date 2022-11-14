#include <iostream>
#include <tuple>
#include <set>

using namespace std;

int main() {
  set<tuple <int, int> > visited;

  cout << "empty: " << visited.empty() << endl;
  visited.insert(make_tuple(3,3));

  cout << "add [3,3] empty: " << visited.empty() << endl;

  auto found = [](set<tuple<int,int>> &visited, tuple<int, int>p)  {return visited.find(p) != visited.end();};
  cout << "found [5,5] " << found(visited, make_tuple(5,5)) << endl;
  cout << "found [3,3] " << found(visited, make_tuple(3,3)) << endl;
  cout << "set size " << visited.size() << endl;
  visited.insert(make_tuple(3,3));
  cout << "set size " << visited.size() << endl;
  visited.insert(make_tuple(5,5));
  cout << "set size " << visited.size() << endl;
  cout << "yo" << endl;

}
