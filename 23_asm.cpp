#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace {

enum class Op {
  Half,
  Triple,
  Increment,
  Jump,
  JumpIfEven,
  JumpIfOne,
};

// What I actually want is to be able to associate data with each enum element...
using Inst = std::tuple<Op, char, int>;

unsigned int run(const std::vector<Inst>& prog, unsigned int a) {
  unsigned int reg[2]{a, 0};
  const int size = static_cast<int>(prog.size());

  for (int pc = 0; 0 <= pc && pc < size; pc++) {
    //std::cout << pc << " " << reg[0] << " " << reg[1] << std::endl;

    const Inst& inst = prog[pc];
    switch (std::get<Op>(inst)) {
    case Op::Half:
      reg[std::get<char>(inst) - 'a'] /= 2;
      break;
    case Op::Triple:
      reg[std::get<char>(inst) - 'a'] *= 3;
      break;
    case Op::Increment:
      reg[std::get<char>(inst) - 'a']++;
      break;
    case Op::Jump:
      pc += std::get<int>(inst) - 1;
      break;
    case Op::JumpIfEven:
      if ((reg[std::get<char>(inst) - 'a'] % 2) == 0) {
        pc += std::get<int>(inst) - 1;
      }
      break;
    case Op::JumpIfOne:
      if (reg[std::get<char>(inst) - 'a'] == 1) {
        pc += std::get<int>(inst) - 1;
      }
      break;
    default:
      std::cerr << "bad op" << std::endl;
      std::abort();
      break;
    }
  }

  return reg[1];
}

char reg(std::istream& is, char delim = '\n', char discard = '\0') {
  char buf[3];
  is.getline(buf, 3, delim);
  if (buf[1] != discard && buf[1] != '\0') {
    std::cerr << "bad reg " << buf << std::endl;
    std::abort();
  }
  if (buf[0] != 'a' && buf[0] != 'b') {
    std::cerr << "bad reg " << buf << std::endl;
    std::abort();
  }
  return buf[0];
}

char offset(std::istream& is) {
  std::string s;
  getline(is, s);
  return stoi(s);
}

void solve(std::istream& is) {
  std::string s;
  std::vector<Inst> prog;

  while (getline(is, s, ' ')) {
    if (s == "hlf") {
      prog.emplace_back(Op::Half, reg(is), 0);
    } else if (s == "tpl") {
      prog.emplace_back(Op::Triple, reg(is), 0);
    } else if (s == "inc") {
      prog.emplace_back(Op::Increment, reg(is), 0);
    } else if (s == "jmp") {
      prog.emplace_back(Op::Jump, '\0', offset(is));
    } else if (s == "jie") {
      // evaluation order; don't put reg() and offset() in same line.
      char r = reg(is, ' ', ',');
      prog.emplace_back(Op::JumpIfEven, r, offset(is));
    } else if (s == "jio") {
      // evaluation order; don't put reg() and offset() in same line.
      char r = reg(is, ' ', ',');
      prog.emplace_back(Op::JumpIfOne, r, offset(is));
    }
  }

  std::cout << run(prog, 0) << std::endl;
  std::cout << run(prog, 1) << std::endl;
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
