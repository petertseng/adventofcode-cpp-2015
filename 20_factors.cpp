#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <vector>

namespace {

int house(int target, int present_mult, int limit) {
  const int upper_bound = target / present_mult;
  // Hmm, ISO C++ forbids variable length array;
  // shall I use a vector, or shall I just use a fixed size?
  // nope, can't make an array that big (segfaults)
  //int presents2[4'000'000];
  //std::fill(presents2, presents2 + sizeof(presents2) / sizeof(presents2[0]), 0);
  std::vector<int> presents(upper_bound, 0);

  for (int elf = 1; elf < upper_bound; elf++) {
    presents[elf] += elf * present_mult;
    if (presents[elf] >= target) {
      return elf;
    }
    for (int m = 2; m <= limit && elf * m < upper_bound; m ++) {
      presents[elf * m] += elf * present_mult;
    }
  }

  std::cerr << "bad, " << upper_bound << " too low upper bound for " << target << std::endl;
  std::abort();
}

void solve(int target) {
  std::cout << house(target, 10, target) << std::endl;
  std::cout << house(target, 11, 50) << std::endl;
}

void solve(std::istream& is) {
  std::string s;
  getline(is, s);
  solve(stoi(s));
}

}

int main(int ac, char** av) {
  if (ac > 1) {
    struct stat statbuf;
    if (stat(av[1], &statbuf) == 0) {
      std::ifstream ifile{av[1]};
      solve(ifile);
    } else {
      solve(atoi(av[1]));
    }
  } else {
    solve(std::cin);
  }
  return 0;
}
