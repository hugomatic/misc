#include <iostream>
#include <queue>
#include <tuple>
#include <set>
#include <array>
#include <future>
#include <fstream>

using namespace std;
using std::cout;
using std::endl;

#define BitVal(data,y) ( (data>>y) & 1)      /** Return Data.Y value   **/
#define SetBit(data,y)    data |= (1 << y)    /** Set Data.Y   to 1    **/
#define ClearBit(data,y)  data &= ~(1 << y)   /** Clear Data.Y to 0    **/
#define TogleBit(data,y)     (data ^=BitVal(y))     /** Togle Data.Y  value  **/
#define Togle(data)   (data =~data )         /** Togle Data value     **/


int square(int x) {
  return x * x;
}

int main() {

  cout << "yo" << endl;

  future<int> asyncFn = async(&square, 12);
  cout << "square of 12 " << asyncFn.get() <<  endl;

  std::queue<tuple<int,int>> q;
  q.push(std::make_tuple(3,3));
  cout << "empty? " << q.empty() << endl;
  auto &f = q.front();
  auto [x,y] = f;
  q.pop();
  cout << "pop [" << x << "," << y <<  "] empty? " << q.empty() << endl;


  std::set<tuple<int,int>> vis;
  auto t = make_tuple(4,4);
  vis.insert(t);
  cout << "found: " <<  (vis.find(t) != vis.end()) << endl;

  auto found = [&vis](tuple<int,int> t) -> bool {
    auto [a,b] = t;
    return  vis.find(t) != vis.end();
  };
  cout << "found lambda yes: " << found(t) << endl;
  cout << "found lambda no: " << found(make_tuple(0,0)) << endl;
  cout << "erase t: " << vis.erase(t) << endl;
  cout << "found lambda rmoved: " << found(t) << endl;

  array<int, 3>  aa = {1,2,3};

  cout << "aa ";
  for (auto &i: aa) {
    cout << i << " ";
  }
  cout << endl;

  std::string s { "yo yo yo"};
  for(size_t i=0; i < s.length(); i++) {
    cout << i << " " << s[i] << endl;
  }

  uint8_t b = 0b1010101;
  cout << "\nbytes " << int(b) << endl;

  for (size_t i=0; i<8; i++) {
    bool v = ((b >> i) &1);
    cout << i << " "; // << " mask: [" << int(mask) << "] v "
    cout << v << endl;
  }

  b = 0;
  b |= (1<< 2);
  cout << int(b) << endl;

  b &= ~(1<< 2);
  cout << int(b) << endl;
  cout << endl;

  cout << "write file: " << endl;
  std::ofstream ofs("/tmp/toto", std::ofstream::out);
  ofs << "yo" << endl;
  ofs << "yo" << endl;
  ofs << endl;
  ofs.close();
  cout << "read file" << endl;
  std::ifstream ifs("/tmp/toto", std::ifstream::in);
  if (ifs.is_open()) {
    string line;
    int i = 0;
    while(getline(ifs, line)) {
      cout << i++ << ": "  << line << endl;
    }
  }
  ifs.close();
}
