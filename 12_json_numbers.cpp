#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace {

enum class Nestable {
  Array,
  Object,
};

void must_pop(std::vector<Nestable>& nestables, Nestable nestable) {
  if (nestables.back() != nestable) {
    std::cerr << "malformed: bad close" << std::endl;
    std::abort();
  }
  nestables.pop_back();
}

void solve(std::istream& is) {
  std::vector<Nestable> nestables;
  std::vector<std::pair<int, bool>> objs{{0, false}};

  int total = 0;

  bool neg = false;
  int cur_num = 0;

  int quote_start = 0;
  int quote_red = 0;
  bool in_string = false;

  auto parse_num = [&total, &cur_num, &neg, &objs]() {
    int val = cur_num * (neg ? -1 : 1);
    total += val;
    std::get<int>(objs.back()) += val;
    cur_num = 0;
    neg = false;
  };

  char c;
  int i = 0;
  while (is.get(c)) {
    i++;
    if (in_string) {
      switch (c) {
      case 'r':
        if (quote_start == i - 1) {
          quote_red++;
        }
        break;
      case 'e':
        if (quote_start == i - 2) {
          quote_red++;
        }
        break;
      case 'd':
        if (quote_start == i - 3) {
          quote_red++;
        }
        break;
      case '"':
        if (quote_red == 3 && quote_start == i - 4 && nestables.back() == Nestable::Object) {
          std::get<bool>(objs.back()) = true;
        }
        in_string = false;
        break;
      }
      continue;
    }
    // not in_string
    switch (c) {
    case '{':
      nestables.push_back(Nestable::Object);
      objs.emplace_back(0, false);
      break;
    case '}':
      parse_num();
      must_pop(nestables, Nestable::Object);
      {
        int objval = std::get<bool>(objs.back()) ? 0 : std::get<int>(objs.back());
        objs.pop_back();
        std::get<int>(objs.back()) += objval;
      }
      break;
    case '[':
      nestables.push_back(Nestable::Array);
      break;
    case ']':
      parse_num();
      must_pop(nestables, Nestable::Array);
      break;
    case '"':
      in_string = true;
      quote_start = i;
      quote_red = 0;
      break;
    case ',':
      parse_num();
      break;
    case '-':
      if (neg || cur_num != 0) {
        std::cerr << "malformed: bad neg" << std::endl;
        std::abort();
      }
      neg = true;
      break;
    case ':':
    case '\n':
      break;
    default:
      if ('0' <= c && c <= '9') {
        cur_num = cur_num * 10 + (c - '0');
      } else {
        std::cerr << "malformed: bad char" << c << std::endl;
        std::abort();
      }
      break;
    }
  }

  std::cout << total << std::endl;

  if (objs.size() > 1) {
    std::cerr << "malformed: unclosed obj" << std::endl;
    std::abort();
  }
  if (!nestables.empty()) {
    std::cerr << "malformed: unclosed arr" << std::endl;
    std::abort();
  }
  if (objs.empty()) {
    std::cerr << "impossible: always have one obj" << std::endl;
    std::abort();
  }

  std::cout << (std::get<bool>(objs.back()) ? 0 : std::get<int>(objs.back())) << std::endl;
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
