#include <thread>
#include <iostream>



void do_nothing(std::stop_token st) {
  int count = 0;
  std::cout << "-nothing?" << std::endl;
  while(!st.stop_requested()) {
    std::cout << "-yo " << count ++ << std::endl;
  }
  puts("-stop");
}


int main()
{
  std::jthread t{do_nothing};
  std::cout << "join" << std::endl;
  // t.join();
  int count = 5;
  while (count) {
    std::this_thread::yield();
    std::cout << "bro "  << count << std::endl;
    count --;
  }
  std::cout << "the end" << std::endl;
}
