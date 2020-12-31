#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {

void solve(std::istream& is) {
  int floor = 0;
  int basement = -1;
  int i = 0;
  char c;
  while (is.get(c)) {
    i++;
    switch (c) {
    case '(':
      floor += 1;
      break;
    case ')':
      floor -= 1;
      if (floor == -1 && basement == -1) {
        basement = i;
      }
      break;
    case '\n':
      break;
    default:
      std::cerr << "bad char " << c << std::endl;
      std::abort();
    }
  }
  std::cout << floor << std::endl;
  std::cout << basement << std::endl;
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
