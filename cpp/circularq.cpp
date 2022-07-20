/*
# Goal
---

Write a generic C++ class CircularQueue<T>, which is a queue holding elements
of type T that has a **fixed** maximal size N, and only keeps the most recent N
enqueued objects. Once the queue is full, pushing new elements will overwrite
the oldest ones.

You need to implement these methods:
- Constructor that takes the maximal size and pre-allocates the memory.
- `enqueue` adds one element to the queue, replacing the oldest element if the queue is full.
- `dequeue` returns the oldest element and removes it from the queue
- `count` returns the number of elements in the queue

Constraints:
- You should never allocate new memory outside of the constructor.
- Don't use `std::deque` or advanced STL containers (`std::vector` is fine though if you want to use the STL).

We evaluate the solution based on:

*   Whether it works :) We expect you to compile and run it. (We'll also test with our own `main` function at the end)
*   Clarity of the code
*   Attention to details
*   Efficiency (but we recommend writing a simple solution first, it's ok to just discuss how we could make it faster if it's not optimal)

**Questions are very welcome (and even expected!) if something is not clear or not well specified.**

```
Example with N = 4 (X means uninitialized, can be any value)
  Initial     X X X X
  Enqueue 1   1 X X X
  Enqueue 2   1 2 X X
  Dequeue     X 2 X X (returns 1)
  Enqueue 3   X 2 3 X
  Enqueue 4   X 2 3 4
  Enqueue 5   5 2 3 4
  Enqueue 6   5 6 3 4
  Dequeue     5 6 X 4 (returns 3)
  Dequeue     5 6 X X (returns 4)
  Dequeue     X 6 X X (returns 5)
  Dequeue     X X X X (returns 6)
  Enqueue 7   X 7 X X (other options are possible though)
```

writefile circularQueue.cpp

*/

#include <iostream>
#include <cassert>
#include <vector>

template<typename T>
class CircularQueue
{
  std::vector<T> elements;
  int buffer_size;
  int newest;
  int oldest;
  int elemCount;
public:
    /// Initialize the queue with a given size
    CircularQueue (int buffer_size)
     :buffer_size(buffer_size),
      newest(0),
      oldest(),
      elemCount(0)

    {
      elements.resize(buffer_size);
    }

    ~CircularQueue ()
    {
        // TODO
    }

    /// Adds one element to the queue, overwriting the oldest element if the queue is full.
    void push(const T& element)
    {
        std::cout << "push " << element << std::endl;
        if (count() == 0) {
          newest = 0;
          oldest = 0;
          elements[0] = element;
          elemCount = 1;
          pr();
          return;
        }

        newest ++;
        if (newest > buffer_size -1) {
          newest = 0;
        }
        elements[newest] = element;
        if (elemCount < buffer_size) {
          elemCount ++;
        }
        else {
          oldest ++;
          if (oldest > buffer_size -1) {
            oldest = 0;
          }
        }
        pr();
    }

    void pr() {
      // std::cerr << "# newest " << newest   << std::endl;
      // std::cerr << "# oldest " << oldest   << std::endl;
      std::cerr << "# buffer " << buffer_size << " count " << count() << " n,o[" << newest << ", " << oldest << "]"  << std::endl;
      std::cerr << "# ";
      for (auto v: elements) {
        std::cerr << v << ",  ";
      }

      std::cerr << std::endl;
      std::cerr << "# ";
      for (size_t i=0; i < elements.size(); ++i) {
        char c = ' ';
        if (i == newest) c = 'n';
        if (i == oldest) c = 'o';
        if (i ==oldest  && i == newest) c = 'x';
        std::cerr << c << "   ";
      }

      std::cerr << std::endl;
      std::cerr << std::endl;
    }

    /// Returns the oldest element and removes it from the queue
    T pop()
    {
        std::cout << "pop! ";
        if (count() == 0) {
          std::cout << "Error" << std::endl;
          pr();
          return T();
        }

        T r = elements[oldest];
        elemCount --;
        if (count() == 0){
          newest =0;
          oldest = 0;
          std::cout << r << " (last elem)" <<  std::endl;
          pr();
          return r;
        }

        oldest ++;
        std::cout << "? oldest " << oldest <<  " buffer size " << buffer_size<< std::endl;
        if (oldest > buffer_size -1) {
          std::cout << "? reset" << std::endl;
          oldest = 0;
        }

        std::cout << r << std::endl;
        pr();
        return r;
    }

    /// Return the number of elements currently in the queue
    int count() const
    {
      return elemCount;
    }

private:
    /// Add any variables needed to complete this challenge here.
};


void t1() {
    CircularQueue<int> queue(4);
    queue.pr();
    queue.pop();
    queue.push(1);
    queue.pop();
    queue.pop();
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.push(6);
    queue.push(7);
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();

}

void t2() {
    CircularQueue<int> queue(4);
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);
    queue.pop();
    queue.push(6);
//    queue.push(7);
//    queue.push(8);
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();
    queue.pop();


}

int main(void)
{ // t1();
  t2();
}

