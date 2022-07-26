#include <iostream>
#include <vector>
using namespace std;

// https://www.techinterviewhandbook.org/grind75

// Example 1:
//
// Input: intervals = [[1,3],[6,9]], newInterval = [2,5]
// Output: [[1,5],[6,9]]
// Example 2:
//
// Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
// Output: [[1,2],[3,10],[12,16]]
// Explanation: Because the new interval [4,8] overlaps with [3,5],[6,7],[8,10].
//

void pr(string const &msg,  vector<vector<int>> const &intervals) {
  cout << msg << ": ";
  for (auto &v : intervals) {
    cout << "[" << v[0] << ", " << v[1] << "], ";
  }
  cout << endl;

}

std::vector< std::vector<int> > insert(vector<vector<int> >& intervals, vector<int>& newInterval) {

  // find where the interval starts
  size_t i;
  for (i=0; i < intervals.size(); ++i) {
    auto &interval = intervals[i];

    // insert before?
    if ( newInterval[1] < interval[0]) {
      // cout << " - insert before @ " << i << endl;
      intervals.insert(intervals.begin() + i, newInterval);
      return intervals;
    } else if (newInterval[0] > interval[1]) {
      continue;
    }
    else {
       int start = min(newInterval[0], interval[0]);
       int end = max(newInterval[1], interval[1]);
       auto it_start = intervals.begin() + i;
       auto it = intervals.begin() + i;
       while (it != intervals.end()) {
          auto r = *it;
          // cout <<  " - start/end " << start  << ", " << end << " inter ["  << r[0] << ", " << r[1] << "] "  <<endl;
          if (r[0] > end) {
            break;
          }
          if (r[1] >= end) {
            end = r[1];
          }

          ++ it;
       }

       vector<int> finalInterval = {start, end};
       intervals.erase(it_start, it);
       intervals.insert(it_start, finalInterval);
       return intervals;
    }

  }

  // insert after?
  intervals.push_back(newInterval);
  return intervals;
}

void example1 () {
  vector<vector<int>> intervals {{1,3}, {6,9}};
  vector<int> newInterval {2,5};

  pr("intervals", intervals);
  auto results = insert(intervals, newInterval);
  cout << "newInterval [" << newInterval[0] << ", " << newInterval[1] << "]" << endl;
  pr("result", results);
  cout << endl;
}

void example2 () {
  vector<vector<int>> intervals {{1,3}, {6,9}};
  vector<int> newInterval {20,25};

  pr("intervals", intervals);
  auto results = insert(intervals, newInterval);
  cout << "newInterval [" << newInterval[0] << ", " << newInterval[1] << "]" << endl;
  pr("result", results);
  cout << endl;
}

void example3 () {
  vector<vector<int>> intervals {{1,3}, {6,9}, {10,12}, {13,15}};
  vector<int> newInterval {5,13};

  pr("intervals", intervals);
  auto results = insert(intervals, newInterval);
  cout << "newInterval [" << newInterval[0] << ", " << newInterval[1] << "]" << endl;
  pr("result", results);
  cout << endl;
}


int main() {

  example1();
  example2();
  example3();


}
