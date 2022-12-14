#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <cmath>
#include <bitset>

#include <list>
#include <set>
#include <queue>
#include <algorithm> // std::reverse

#include "config.h" // in the cmake build dir

namespace grad {

  class Value {

  public:
    Value(double data=0, std::string const&op="", std::vector<Value*> prev={} )
      :_data(data), _op(op), _prev(prev)
    {

    }

    double data() const {
      return _data;
    }

    double grad() const {
      return _grad;
    }

    void grad(double g) {
      _grad = g;
    }

    std::string op() {
      return _op;
    }

    std::string label() {
      return _label;
    }

    void label(std::string const& l) {
      _label = l;
    }

    Value operator + (Value & other) {
      double d = data() + other.data();
      Value out(d, "+", {this, &other});
      Value *p = this;
      Value *pout = &out;
      Value *pother = &other;
      out._backward = [p, pout, pother]() {
        double meg = p->grad();
        double outg = pout->grad();
        double otherg = pother->grad();
        std::cout << meg << "," << outg << "," << otherg << std::endl;
        pother->grad(otherg + outg);
        p->grad(meg + outg);
      };
      return out;
    }

    Value operator * (Value & other) {
      double d = data() * other.data();
      Value out(d, "*",  {this, &other});
      Value *p = this;
      Value *pout = &out;
      Value *pother = &other;
      out._backward = [p, pout, pother]() {
        double meg = p->grad();
        double outg = pout->grad();
        double otherg = pother->grad();
        std::cout << meg << "," << outg << "," << otherg << std::endl;
        pother->grad(otherg + outg * p->data());
        p->grad(meg + outg * pother->data());
      };
      return out;
    }



    Value tanh() {
      double d = ::tanh(data());
      Value out(d, "tanh", {this});
      Value *p = this;
      Value *pout = &out;
      out._backward = [p, pout, d]() {
       p->grad(p->grad() + (1 - d *d) * pout->grad());
      };
      return out;
    }

    // string represenation
    std::ostream& repr(std::ostream& out) const
    {
      out << "{ "
        << "\"label\":" << "\"" <<  _label << "\" " << ", "
        << "\"op\":" << "\"" <<  _op << "\" " << ", "
        << "\"data\": " << std::setprecision(6) << _data << ", "
        << "\"grad\": " << std::setprecision(6) << _grad << ", "
        << "\"id\": \"" << this << "\", "
        << "\"prev\": [";
        std::string sep = "";
        for (auto n: _prev) {
          out << sep << "\"" << n << "\"" ;
          sep = ", ";
        }
        out << "]";
        out << "}";
      return out;
    };

    void backward() {
      grad(1.0);

      std::list<Value*> topo;
      std::set<Value*> visited;
      std::queue<Value*> q;
      q.push(this);
      while (!q.empty()) {
        Value* v = q.front();
        q.pop();
        topo.push_back(v);
        for(auto p: v->_prev) {
          auto r = visited.insert(p);
          // is it new?
          if (r.second) {
            q.push(p);
          }
        }
      }
      std::cout << "\nBackward" << std::endl;
      int count = 0;
      for (auto v:topo) {
        std::cout << "-" << count << std::endl ;
        v->repr(std::cout);
        // v->_backward();
        std::cout << std::endl;
        count ++;
      }
    }

  private:

    std::function<void()> _backward{[](){}};
    double _data;
    double _grad{0.0};
    std::string _label;
    std::string _op;
    std::vector<Value*> _prev;
  };

  class Neuron {

    public:
    Neuron(size_t nin)
    {
      _weights.resize(nin);
    }

    Value forward(std::vector<Value> &x)
    {
      _nodes.clear();
      Value *psum = &_bias;
      for(size_t i=0; i < _weights.size(); ++i ) {
        // compute weight * input
        _nodes.push_back(x[i] *_weights[i] );
        Value &xw = _nodes[_nodes.size() -1];
        _nodes.push_back(xw + *psum);
        psum = &_nodes[_nodes.size() -1];
      }
      // psum should now point to the activation value
      _nodes.push_back(psum->tanh());
       _nodes[_nodes.size() -1].backward();
      return  _nodes[_nodes.size() -1];
    }

    // private:
    std::vector<Value> _weights;
    Value _bias;
    // intermediate nodes
    std::vector<Value> _nodes;
  };

std::ostream& operator<<(std::ostream& out, const grad::Value& value) {
    return value.repr(out);
}


} // namespace ccpgrad

void test1() {
  using namespace std;
  using namespace grad;

  Value a{42.0};
  a.label("a");
  a.grad(42.69);
  cout << a << endl;

  Value b{69.0, "nop", {&a}};
  b.label("b");

  Value c(13.0);
  Value d = c + b;
  d.label("c + b");
  cout << d << endl;

  cout << d * a << endl;

}

void test_add() {
  using namespace std;
  using namespace grad;

  Value a{42.0};
  a.label("a");
  a.grad(0);
  Value b{0.69};
  b.label("b");
  b.grad(0);

  Value d = a + b;
  d.label("d");

  d.grad(1.0);
  cout << d << endl;

  d.backward();
  cout << "_backward() of: "  << d << endl;
  cout << a << endl;
  cout << b << endl;

  Value x = d;
  x.label("copy of d");
  cout << x << endl;
}

void test_mul() {
  using namespace std;
  using namespace grad;

  Value a{42.0};
  a.label("a");
  a.grad(0);
  Value b{0.69};
  b.label("b");
  b.grad(0);

  Value d = a * b;
  d.label("d");

  d.grad(1.0);
  cout << d << endl;

  d.backward();
  cout << "_backward() of: "  << d << endl;
  cout << a << endl;
  cout << b << endl;
}

void test_tanh() {
  using namespace std;
  using namespace grad;

  cout << "test_tanh" << endl;
  Value a(0.8814);
  Value t = a.tanh();
  t.grad(1.0);
  t.backward();
  cout << t << endl;
  cout << a << endl;
}

#define BitVal(data,y) ( (data>>y) & 1)      /** Return Data.Y value   **/
#define SetBit(data,y)    data |= (1 << y)    /** Set Data.Y   to 1    **/
#define ClearBit(data,y)  data &= ~(1 << y)   /** Clear Data.Y to 0    **/
#define TogleBit(data,y)     (data ^=BitVal(y))     /** Togle Data.Y  value  **/
#define Togle(data)   (data =~data )         /** Togle Data value     **/

void test_bits() {
  using namespace std;

  const std::uint8_t x = 0b00011101;
  std::cout << "x = " << std::bitset<8>(x) << '\n';
  cout << "x: " << int(x) << endl;
  for (size_t i=0; i < 8; i++){
    cout << ((x >> i) & 1) << endl;
  }

  cout << "1&1:" <<  (1 & 1)
       << ", 0&0: " << (0 & 0)
       << ", 0^0: " << (0^0)
       << ", 1^1: " << (1^1)
       << ", 0^1: " << (0^1)
       << ", ~1: " <<~1
  <<endl;
  cout << "x0: " << BitVal(x,0) << endl;
  cout << "x1: " << BitVal(x,1) << endl;
  cout << "x2: " << BitVal(x,2) << endl;
  cout << "x3: " << BitVal(x,3) << endl;
  cout << "x4: " << BitVal(x,4) << endl;
  cout << "x5: " << BitVal(x,5) << endl;
  cout << "x6: " << BitVal(x,6) << endl;
  cout << "x7: " << BitVal(x,7) << endl;
  cout << "x8: " << BitVal(x,8) << endl;
}

void test_backward() {
  using namespace std;
  using namespace grad;

  Value x1(2.0); x1.label("x1");
  Value x2(0.0); x2.label("x2");
  Value w1(-3.0); w1.label("w1");
  Value w2(1.0); w2.label("w2");
  Value b(6.881373); b.label("b");
  Value x1w1 = x1 * w1; x1w1.label("x1w1");
  Value x2w2 = x2 * w2; x2w2.label("x2w2");
  Value x1w1x2w2 = x1w1 + x2w2; x1w1x2w2.label("x1w1x2w2");
  Value n = x1w1x2w2 + b; n.label("n");
  Value o = n.tanh(); o.label("o");

  o.backward();
}

void test_neuron() {
  using namespace std;
  using namespace grad;

  cout << "test_neuron" << endl;
  Neuron n(2);
  n._bias= Value(6.881373);
  n._weights[0] = Value(-3.0);
  n._weights[1] = Value(1.0);
  std::vector<Value> x;
  x.resize(2);
  x[0] = Value(2.0);
  x[0].label("x0");
  x[1] = Value(0.0);
  x[1].label("x1");

  cout << "inputs" << endl;
  cout << x[0] << endl;
  cout << x[1] << endl;

  cout << "test forward" << endl;
  Value o = n.forward(x);
  o.label("o");
  cout << o << endl;
  cout << "backward" << endl;
  o.backward();
  cout << "output" << endl;
  cout << o << endl;
}

int main() {
  using namespace std;

  cout << "cppgrad v"
    << cppgrad_VERSION_MAJOR << "."
    << cppgrad_VERSION_MINOR
    << endl;

  test1();
  test_add();
  test_mul();
  test_tanh();
  test_bits();
  test_backward();

  test_neuron();
  return 0;
}
