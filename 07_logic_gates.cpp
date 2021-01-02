#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>

namespace {

enum class Op {
  And,
  Or,
  RShift,
  LShift,
};

using Operand = std::variant<uint16_t, std::string>;
using Unop = std::tuple<bool, Operand>;
using Binop = std::tuple<Op, Operand, Operand>;
using Gate = std::variant<Unop, Binop>;
using Circuit = std::unordered_map<std::string, Gate>;

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

Operand operand(const std::string& s) {
  uint16_t i = 0;
  for (const char& c : s) {
    if ('0' <= c && c <= '9') {
      i = i * 10 + (c - '0');
    } else {
      return {s};
    }
  }
  return {i};
}

Op op(const std::string& s) {
  if (s == "AND") {
    return Op::And;
  } else if (s == "OR") {
    return Op::Or;
  } else if (s == "RSHIFT") {
    return Op::RShift;
  } else if (s == "LSHIFT") {
    return Op::LShift;
  } else {
    std::cerr << "bad op " << s << std::endl;
    std::abort();
  }
}

uint16_t value(const Circuit& circuit, const std::string& wire, std::unordered_map<std::string, uint16_t>& cache) {
  auto cached = cache.find(wire);
  if (cached != cache.end()) {
    return cached->second;
  }

  auto val_of_operand = [&circuit, &cache](Operand o) {
    return std::visit(overload {
      [](uint16_t v) { return v; },
      [&circuit, &cache](const std::string& s) { return value(circuit, s, cache); },
    }, o);
  };

  Gate g = circuit.at(wire);
  uint16_t val = std::visit(overload {
    [&val_of_operand](const Unop& u) {
      uint16_t v = val_of_operand(std::get<Operand>(u));
      return std::get<bool>(u) ? ~v : v;
    },
    [&val_of_operand](const Binop& b) {
      uint16_t v1 = val_of_operand(std::get<1>(b));
      uint16_t v2 = val_of_operand(std::get<2>(b));
      switch (std::get<Op>(b)) {
      case Op::And:
        return v1 & v2;
      case Op::Or:
        return v1 | v2;
      case Op::LShift:
        return v1 << v2;
      case Op::RShift:
        return v1 >> v2;
      default:
        std::cerr << "impossible" << std::endl;
        std::abort();
      }
    },
  }, g);
  cache.emplace(wire, val);

  return val;
}

void solve(std::istream& is) {
  std::string s;
  Circuit circuit;

  while (getline(is, s, ' ')) {
    bool inverter = false;
    if (s == "NOT") {
      inverter = true;
      getline(is, s, ' ');
    }
    const Operand op1 = operand(s);
    getline(is, s, ' ');
    if (s == "->") {
      getline(is, s);
      circuit.emplace(s, Unop{inverter, op1});
    } else if (inverter) {
      std::cerr << "inverting a binop" << std::endl;
      std::abort();
    } else {
      const Op oper{op(s)};
      getline(is, s, ' ');
      const Operand op2 = operand(s);
      getline(is, s, ' ');
      if (s != "->") {
        std::cerr << "bad gate, needs to be arrow " << s << std::endl;
        std::abort();
      }
      getline(is, s);
      circuit.emplace(s, Binop{oper, op1, op2});
    }
  }

  std::unordered_map<std::string, uint16_t> cache;
  uint16_t a = value(circuit, "a", cache);
  std::cout << a << std::endl;
  circuit["b"] = Unop{false, a};
  cache.clear();
  std::cout << value(circuit, "a", cache) << std::endl;
}

}

int main(int ac, char** av) {
  if (ac > 1) {
    std::ifstream ifile{av[1]};
    solve(ifile);
  } else {
    solve(std::cin);
  }
  return 0;
}
