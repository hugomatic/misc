#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <list>


std::tuple<char, char> how(std::string const &first, std::string const &second) {
  // compare 2 ordered words letter by letter, to find which is first.
  // Return which 2 letters determine the order, or ('x' ,'x').
  using namespace std;
  // cout << " how is [" << first << "] < " << "[" << second << "]?" << endl;
  for (size_t i=0; i < first.length(); ++i) {
    char before = first.c_str()[i];
    char after = second.c_str()[i];
    if (before != after) {
      return std::make_tuple(before, after);
    }
  }
  // should not happen unles comparing a word to itself
  return std::make_tuple('x', 'x');
}

void pr(std::vector<std::string> const &words) {
  // print a list of words with their characters indices
  using namespace std;
  for (auto &word: words) {
    cout << word << " (" << word.length() << ") " << endl;
    for (size_t i = 0; i < word.length(); ++i) {
      char c = word.c_str()[i];
      cout << " " << i << ": " << c << endl;
    }
  }
}

void pr(std::string const& msg, std::list<char> const &order) {
  // print a list of ordered letters
  using namespace std;
  cout << msg << " [";
  for (auto &c :order) {
    cout << c << ", ";
  }
  cout << "]" << endl;
}

int main() {
  // find the custom  ordering of characters from a list of ordered words
  // input: [pbb, bpku, bpkb, kbu]
  // solution : [p, u, b, k]
  using namespace std;
  std::vector<std::string> words {"pbb", "bpku", "bpkb", "kbu"};
  for(auto &w: words){
    cout << w << " ";
  }
  cout << endl;
  // pr(words);
  // build our alphabet
  std::list<char> order;
  int count = 0;
  for (size_t i =0; i < words.size() -1; ++i) {
    auto [before, after] = how(words[i], words[i+1]);
    cout << i << " comp: '" << words[i] << "' '" << words[i+1] << "'? "
         << " [" << before << "< " << after << "]" << endl;
    // is the before (smallest value) letter in our alphabet?
    auto ib = std::find(order.begin(), order.end(), before);
    // is the after (bigger value) letter in our alphabet?
    auto ia = std::find(order.begin(), order.end(), after);
    // both there already?
    if (ib != order.end() && ia != order.end()) {
      continue;
    }

    // add the before (smallest) letter at the end if not already there
    if (ib == order.end()) {
      if (ia == order.end()) {
        order.push_back(before);
      }
      else {
        // insert before the after letter
        order.insert(ia, before);
      }

    }
    // add after if not there
    if (ia == order.end()) {
      if (ib == order.end()) {
        order.push_back(after);
      }
      else {
        // insert after the before
        ib ++;
        order.insert(ib, after);
      }
    }

    pr(" step "+ std::to_string(count) + ") " , order);
    count ++;
  }

  pr("final", order);
}

