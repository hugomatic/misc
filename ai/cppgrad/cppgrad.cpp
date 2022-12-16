#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <queue>
#include <set>
#include <functional>
#include <iterator>

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

  Data() {
  }

  Data(const std::vector<double> &vals) {
    std::copy(vals.begin(),
              vals.end(),
              std::back_inserter(values));
  }

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

class Tensor;
std::ostream& operator<< (std::ostream &out, Tensor &t);


class Tensor {
  public:
  Data data;
  Data grad;
  Shape shape;
  Operation op;
  std::vector<Tensor*> prevs;
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


 Tensor(std::vector<double> const &vals,
        Operation op_=Operation::X,
        std::string label_="" )
    :data(vals),
    op(op_),
    label(label_)
  {
      for (size_t i=0; i < vals.size(); i++) {
        grad.values.push_back(0);
      }
      shape.dims.push_back(vals.size());
  }

  std::vector<double> item() const {
    return data.values;
  }

  void set(double v) {
    data.values.push_back(v);
  }

  void repr(std::ostream &o) {
    o << "tensor ";
    if(label != "") {
      o << "\"" << label << "\"";
    }
    if (op != Operation::X) {
      o << " " << op;
    }
    o << " (" << data << ")";
  }

  void backward() {
    std::cout << "backward" << std::endl;
    grad.values[0] = 1.0;
    std::vector<Tensor *> sequence;
    std::queue<Tensor *> to_visit;
    std::set<Tensor *> visited;

    to_visit.push(this);
    while(to_visit.empty()==false) {
      // pick next one
      Tensor *node = to_visit.front();
      to_visit.pop();
      if (visited.find(node) == visited.end()) {
        visited.insert(node);
        for(auto next: node->prevs) {
          to_visit.push(next);
        }
        std::cout << "!!!! " << node << std::endl;
        sequence.push_back(node);
      }
    }
    std::cout << sequence.size()  << " nodes. Order of compute" << std::endl;
    int i =0;
    for (Tensor* n: sequence) {
      std::cout << " " << (i++) << " " << *n << std::endl;
      n->_backward();
    }
  }
};

std::ostream& operator<< (std::ostream &out, Tensor &t) {
  t.repr(out);
  return out;
}

class Graph {
  std::vector<Tensor*> tensors;
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
      std::cout << a << " + " << b << std::endl;
      std::vector<double> sum;
      for(size_t i = 0; i < a.data.values.size(); i++) {
        sum.push_back(a.data.values[i] + b.data.values[i]);
      }
      tensors.push_back(new Tensor(sum, Operation::ADD, label));
      Tensor &c = *tensors[tensors.size()-1];

      // set the _bacward on a
      c._backward = [&a, &b, &c](){
        std::cout << "ADD BACK to back!" << std::endl;
        std::cout << c << + " = " << a  << " + " << b << std::endl;
        c.prevs.push_back(&a);
        c.prevs.push_back(&b);
      };
      return c;
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

