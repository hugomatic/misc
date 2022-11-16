#include <iostream>
#include <future>


int square(int x) {
  int r  = x * x;
  std::cout << " - " << r << std::endl;
  return r;
}


int main() {
  std::future<int> asyncFunction = std::async(&square, 12);

  for (int i=0; i< 10; i++) {
    std::cout << square(i) << std::endl;
  }

  std::cout << "====" << std::endl;
  int res = asyncFunction.get();
  std::cout << res <<  " the end" << std::endl;
  return 0;
}
