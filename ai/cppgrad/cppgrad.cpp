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

/*

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
*/

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

  Tensor(double v, std::string label_="", Operation op_=Operation::X)
    :data(v),
    grad(0),
    op(op_),
    label(label_)
  {
      shape.dims.push_back(1);
  }

  Tensor(std::vector<double> const &vals,
                            std::string label_="",
                            Operation op_=Operation::X)
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
        sequence.push_back(node);
      }
    }
    std::cout << sequence.size()  << " nodes. Compute graph" << std::endl;
    int i =0;
    for (Tensor* n: sequence) {
      std::cout << " " << (i++) << " " << *n << " grad: " <<  n->grad << std::endl;
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

    Tensor & tensor(double v, std::string label="", Operation op=Operation::X) {
      tensors.push_back(new Tensor(v, label, op));
      Tensor &t = *tensors[tensors.size()-1];
      return t;
    }

    Tensor & mul(Tensor &a, Tensor &b, const std::string &label="") {
      std::cout << &a << " " << a << " * " << &b << " " << b<< std::endl;
      std::vector<double> mul;
      for(size_t i = 0; i < a.data.values.size(); i++) {
        mul.push_back(a.data.values[i] * b.data.values[i]);
      }
      tensors.push_back(new Tensor(mul, label, Operation::MUL));
      Tensor &c = *tensors[tensors.size()-1];
      c.prevs.push_back(&a);
      c.prevs.push_back(&b);
      // set the _bacward on a
      c._backward = [&a, &b, &c](){
        //std::cout << R"EOF(
        //
        //  c = a * b -> populate a.grad and b.grad from c's backward
        //  dc/da = (((a+h) * b) - (a * b)) /h -> b
        //  dc/db = (((a+h)+b) - (a+b)) /h -> a
        // )EOF" << std::endl;
        for(size_t i = 0; i < a.data.values.size(); i++) {
          a.grad.values[i] += c.grad.values[i] * b.data.values[i];
          b.grad.values[i] += c.grad.values[i] * a.data.values[i];
        }
      };
      return c;
    }


    Tensor & add(Tensor &a, Tensor &b, const std::string &label="") {
      std::cout << &a << " " << a << " + " << &b << " " << b<< std::endl;
      std::vector<double> sum;
      for(size_t i = 0; i < a.data.values.size(); i++) {
        sum.push_back(a.data.values[i] + b.data.values[i]);
      }
      tensors.push_back(new Tensor(sum, label, Operation::ADD));
      Tensor &c = *tensors[tensors.size()-1];
      c.prevs.push_back(&a);
      c.prevs.push_back(&b);
      // set the _bacward on a
      c._backward = [&a, &b, &c](){
        //std::cout << R"EOF(
        //
        //  c = a + b
        //  dc/da = (((a+h)+b) - (a+b)) /h -> 1
        //  dc/db = (((a+h)+b) - (a+b)) /h -> 1
        // )EOF" << std::endl;
        for(size_t i = 0; i < a.data.values.size(); i++) {
          a.grad.values[i] += c.grad.values[i] * 1.0;
          b.grad.values[i] += c.grad.values[i] * 1.0;
        }
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

void test_add() {
  using std::cout;
  using std::endl;

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
}

void test_mul() {
  using std::cout;
  using std::endl;

  grad::Graph g;
  grad::Tensor &w = g.tensor(42., "w");
  grad::Tensor &x = g.tensor(13., "x");
  grad::Tensor &b = g.tensor(33, "b");
  grad::Tensor &wx = g.mul(w,x, "wx");
  grad::Tensor &o = g.add(wx, b, "o");
  o.backward();
  cout << g << endl;
}


int main(int argc, char* argv[]){
  using std::cout;
  using std::endl;

  for (int i =0; i < argc; i++) {
    cout << " " << i << "|" << argv[i] << "|" << endl;
  }
  cout << "\nTest add" << endl;
  test_add();
  cout << "\nTest mul" << endl;
  test_mul();
  return 0;
}

