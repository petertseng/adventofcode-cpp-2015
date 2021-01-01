#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {

void must_x(std::istream& is) {
  char c = is.get();
  if (c != 'x') {
    std::cerr << "bad char " << c << std::endl;
    std::abort();
  }
}

void solve(std::istream& is) {
  int paper = 0;
  int ribbon = 0;

  int a;
  while (is >> a) {
    must_x(is);
    int b;
    is >> b;
    must_x(is);
    int c;
    is >> c;

    const int min = std::min(std::min(a, b), c);
    const int max = std::max(std::max(a, b), c);
    const int med = a + b + c - min - max;

    paper += 2 * (a * b + b * c + c * a) + min * med;
    ribbon += a * b * c + 2 * (min + med);
  }

  std::cout << paper << std::endl;
  std::cout << ribbon << std::endl;
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
