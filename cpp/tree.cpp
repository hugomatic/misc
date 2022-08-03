#include <iostream>
#include <filesystem>
#include <vector>
//  " ├─";

//  " │ ";

//  " └─";

namespace fs = std::filesystem;

std::ostream &pr(std::ostream &os,
                 const fs::path root, const std::string &prefix,
                 bool is_last) {

  std::string t = "";
  switch(fs::status(root).type())
    {
        case fs::file_type::none: t = "n/a"; break;
        case fs::file_type::not_found: t = "???"; break;
        case fs::file_type::regular: t = "fil"; break;
        case fs::file_type::directory: t = "dir"; break;
        case fs::file_type::symlink: t = "lnk"; break;
        case fs::file_type::block: t = "blk"; break;
        case fs::file_type::character: t = "chr"; break;
        case fs::file_type::fifo: t = "pip"; break;
        case fs::file_type::socket: t = "soc"; break;
        case fs::file_type::unknown: t = "unk"; break;
        default: std::cout << "!$!"; break;
    }

  std::string d = " ├─";
  if (is_last) {
    d = " └─";
  }
  os << prefix << d << "[" << t << "] " << root << " " << is_last << std::endl;

  if (t == "dir"){
    std::vector<fs::path> children;
    for(auto const& entry : fs::directory_iterator(root)) {
        // os << entry << std::endl;
        children.push_back(entry);
    }
    for (size_t i =0; i < children.size(); ++i) {
      auto const &p = children[i];
      std::string new_prefix = prefix;
      bool is_last = false;
      if (i==children.size() -1) {
        is_last = true;
        new_prefix += "   ";
      }
      new_prefix = prefix;
      if (!is_last) {
        new_prefix +=  " │ ";
      }
      // std::cout << "!!! " << i << "/" << children.size() << ": " <<  p << std::endl;
      pr(os, p, new_prefix, is_last );
    }
  }
  return os;
}

int main(int argc, char *argv[]) {
  using namespace std;
  const fs::path pathToShow{argc >= 2 ? argv[1] : fs::current_path()};
  auto root = fs::canonical(pathToShow);
  if (fs::status(root).type() != fs::file_type::directory) {
    std::cout << "[" << root << "] is not a directory" << endl;
    return -1;
  }
  pr(std::cout, root, "", false);
}
