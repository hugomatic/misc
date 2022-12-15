#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <map>
#include <functional>

#include "config.h" // in the cmake build dir

namespace grad {


enum class Operation { X, ADD, MUL, TANH, EXP, POW };

std::ostream& operator<<(std::ostream& stream, Operation op)
{
    switch (op)
    {
    case Operation::ADD:
        stream << "add";
        break;
    case Operation::MUL:
        stream << "mul";
        break;
    case Operation::TANH:
        stream << "tanh";
        break;
    case Operation::EXP:
        stream << "exp";
        break;
    case Operation::POW:
        stream << "pow";
        break;
    default:
        stream << "";
    }
    return stream;
}


class Value {
  public:

  Operation op;
  double data;
  double grad;
  std::array<Value*, 4> prev;

  void repr(std::ostream &o) {
     o << std::setprecision(4) << data << ", ";
  }
};

std::ostream& operator<< (std::ostream &out, Value &c) {
  c.repr(out);
  return out;
}

class Data {
  public:
  std::vector<double> values;
  Data(double v) {
    // std::cout << "Data(" << v <<  ")\n";
    values.push_back(v);
  }
  ~Data() {
    // std::cout << "~Data ";
    // repr(std::cout);
    // std::cout << std::endl;
  }
  void repr(std::ostream &o) {
    o << "[";
    std::string sep = "";
    for (double d: values) {
      o << sep << std::setprecision(4) << d;
      sep = ", ";
    }
    o << "]";
  }
};

std::ostream& operator<< (std::ostream &out, Data &c) {
  c.repr(out);
  return out;
}

class Shape {
  public:
    std::vector<size_t> dims;
    void repr(std::ostream &o) {
      o << "dim [";
      std::string sep ="";
      for (size_t dim: dims) {
        o << sep << dim;
        sep = ", ";
      }
      o << "]";
    }
};

std::ostream& operator<< (std::ostream &out, Shape &c) {
  c.repr(out);
  return out;
}

class Tensor {
  public:
  Data data;
  Data grad;
  Shape shape;
  Operation op;
  std::array<Tensor*, 4> prevs;
  std::string label;

  std::function<void()> _backward{[](){}};

  Tensor(double v, Operation op_=Operation::X, std::string label_="" )
    :data(v),
    grad(0),
    op(op_),
    label(label_)
  {
      shape.dims.push_back(1);
  }

  double val() const{
    return data.values[0];
  }

  void set(double v) {
    data.values.push_back(v);
  }

  void repr(std::ostream &o) {
    o << "tensor ";
    if(label != "") {
      o << "\"" << label << "\" ";
    }
    o << "(" << data << ")";
  }

  Tensor *add(Tensor &other) {
    return ;
  }

  void backward() {
    std::cout << "backward" << std::endl;
    grad.values[0] = 1.0;
    _backward();
  }
};

std::ostream& operator<< (std::ostream &out, Tensor &t) {
  t.repr(out);
  return out;
}

class Graph {
  std::vector<Tensor*> tensors;
  std::map<int, std::string> labels;
  public:
    Graph()
    {}

    ~Graph() {
      for (Tensor* t: tensors) {
        delete t;
      }
    }

    Tensor & tensor(double v, Operation op=Operation::X, std::string label="") {
      tensors.push_back(new Tensor(v, op, label));
      Tensor &t = *tensors[tensors.size()-1];
      return t;
    }

    Tensor & add(Tensor &a, Tensor &b, const std::string &label="") {
      std::cout << "add" << std::endl;
      return a.add(b, label=label);
      std::cout << a << " + " << b << std::endl;
      tensors.push_back(new Tensor(a.val() + b.val(), Operation::ADD, label));
      Tensor &t = *tensors[tensors.size()-1];
      return t;
    }

    void repr(std::ostream &o) {
      using std::endl;
      using std::setprecision;
      o << "Graph (" << tensors.size() << ") values" << endl;

      size_t count = 0;
      for (auto &t: tensors) {
        o << count << ": " << t << ":" << *t << endl;
      }
    }
};

std::ostream& operator<< (std::ostream &out, Graph &c) {
  c.repr(out);
  return out;
}


} // grad::

int main(int argc, char* argv[]){
  using std::cout;
  using std::endl;

  for (int i =0; i < argc; i++) {
    cout << " " << i << "|" << argv[i] << "|" << endl;
  }

  grad::Graph g;
  cout << "create graph " << g << endl;
  grad::Tensor &a = g.tensor(4.5);
  cout << "a: " << a << " data: " << a.data << endl;
  grad::Tensor &b = g.tensor(42);
  cout << "b: " << b << " data: " << b.data << endl;
  grad::Tensor &c = g.tensor(13);
  cout << "c: " << c << " data: " << c.data << endl;
  grad::Tensor &a_plus_b = g.add(a, b, "a_plus_b");
  cout << "a+b" << a_plus_b << endl;
  a_plus_b.backward();
  cout << g << endl;
  return 0;
}

