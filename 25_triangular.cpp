#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

int iters(int row, int col) {
  const int n = (row + col - 1);
  return (n * (n + 1)) / 2 - row;
}

int64_t modpow(int64_t b, int64_t pow, int64_t m) {
  int64_t r = 1;
  while (pow > 0) {
    if (pow & 1) {
      r = (r * b) % m;
    }
    b = (b * b) % m;
    pow >>= 1;
  }
  return r;
}

void solve(int row, int col) {
  const int64_t m = 33554393;
  std::cout << (20151125 * modpow(252533, iters(row, col), m) % m) << std::endl;
}

std::vector<int> ints(const std::string& s) {
  std::vector<int> ints;
  int curnum = 0;
  bool in_num = false;

  for (const char& c : s) {
    if ('0' <= c && c <= '9') {
      curnum = curnum * 10 + (c - '0');
      in_num = true;
    } else if (in_num) {
      ints.push_back(curnum);
      curnum = 0;
      in_num = false;
    }
  }
  if (in_num) {
    ints.push_back(curnum);
  }

  return ints;
}

void solve(std::istream& is) {
  std::string s;
  getline(is, s);
  std::vector<int> i = ints(s);
  if (i.size() < 2) {
    std::cerr << "bad, not enough ints in " << s << std::endl;
    std::abort();
  }
  solve(i[0], i[1]);
}

}

int main(int ac, char** av) {
  if (ac > 2) {
    solve(atoi(av[1]), atoi(av[2]));
  } else if (ac > 1) {
    std::ifstream ifile{av[1]};
    solve(ifile);
  } else {
    solve(std::cin);
  }
  return 0;
}
