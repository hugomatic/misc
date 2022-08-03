#include <iostream>
#include <map>
#include <vector>

std::map<char, std::vector<std::string>> mnemonic_map{
  {'0',{"0"}},
  {'1',{"1"}},
  {'2',{"a", "b", "c"}},
  {'3',{"d", "e", "f"}},
  {'4',{"g", "h", "k"}},
  {'5',{"j", "k", "l"}},
  {'6',{"m", "n", "o"}},
  {'7',{"p", "q", "r", "s"}},
  {'8',{"t", "u", "v"}},
  {'9',{"w", "x", "y", "z"}},
};

void add( std::vector<std::string> &mnems, std::string const & prefix,
          std::vector<std::vector<std::string>> const &variations) {

    using namespace std;

    if (variations.size() == 0) {
      cout <<  " !!! "  << endl;
      return;
    }
    if (variations.size() == 1) {
      for (auto &variation: variations[0]) {
        string mnemonic =  prefix + variation;
        mnems.push_back(mnemonic);
      }
      return;
    }
    // remove one set of variations
    std::vector<std::vector<std::string>> vars;
    std::copy(variations.begin()+1,  variations.end(), std::back_inserter(vars));
    for (auto &variation: variations[0]) {
      string newPrefix =  prefix + variation;
      add(mnems, newPrefix, vars );
    }
}

std::vector<std::string> phoneMnemonics(std::string const &phoneNb) {
  using namespace std;

  vector<vector<string>> variations;
  for (auto &c :phoneNb) {
    variations.push_back(mnemonic_map[c]);
  }
  std::vector<std::string> mnems;
  add(mnems, "", variations);

  return mnems;
}

int main() {
  using namespace std;
  string number = "6467161873";
  auto mnems = phoneMnemonics(number);
  cout << "the " << mnems.size() << " menmonic(s) for [" << number << "] are: " << endl;

  int counter = 0;
  for (auto &number: mnems) {
    counter ++;
    cout << counter << ") " << number << endl;
  }

}
