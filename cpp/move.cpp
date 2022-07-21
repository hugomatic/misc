#include <iostream>
#include <string>

class C {

  std::string name;

public:
  C(std::string s)
    :name(std::move(s))
  {
    std::cout << "C [" << name << "]" << std::endl;
  }
};

class D {

  std::string name;

public:
  D(std::string &&s)
    :name(std::move(s))
  {
    std::cout << "D [" << name << "]" << std::endl;
  }
};


int main() {

  C ca("litteral");
  std::string n = "moved string";
  C cx(std::move(n));

  D d1{"toto"};

}
