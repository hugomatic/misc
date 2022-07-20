#include <iostream>

#include <vector>
#include <array>

#include <thread>
#include <future>

using namespace std;

struct MsgA {
  int val;
  MsgA(int v) {
    val = v;
    cout << "MsgA() " << val << endl;
  }
};

struct MsgB {
  int val;
  MsgB(int v) {
    val = v;
    cout << "MsgB() " << val << endl;
  }
};

struct MsgC {
  int a;
  int b;
  int val;
  MsgC(int va, int vb) {
    a = va;
    b = vb;
    val = a + b;
  }
};

void  work(future<MsgA> &fa, future<MsgB> &fb, promise<MsgC>& outC ) {

  try {
      cout << "-in work" << endl;
      MsgA a = fa.get();
      cout << "-  got a" << endl;
      MsgB b = fb.get();
      cout << "-  got b" << endl;
      cout << "-  [" << a.val << ", " << b.val << "]\n";

      MsgC c(a.val, b.val);
      outC.set_value(c);
      cout << "- c set" << endl;
  }
  catch(const std::future_error &e) {
    std::cout << "Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    throw;
  }
}



int main() {

  promise<MsgA> promA;
  promise<MsgB> promB;

  future<MsgA> futA = promA.get_future();
  future<MsgB> futB = promB.get_future();

  promise<MsgC> promC;
  thread t1([&futA, &futB, &promC](){cout << "in TH1\n"; work(futA, futB, promC); } );

  future<MsgC> futC = promC.get_future();
  vector toto = {1,2,3,4};
  for (auto &v: toto) {
      cout << "Loop " << v << "\n";
      promA = promise<MsgA>();
      MsgA a(v);
      promA.set_value(a);
      promB = promise<MsgB>();
      MsgB b(v+1);
      promB.set_value(b);
      MsgC resC = futC.get();
      cout << "C result " << resC.val << endl;
  }

  t1.join();

  cout << "yo lo\n" << endl;
}
