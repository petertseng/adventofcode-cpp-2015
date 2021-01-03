#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace {

// Nope, don't use const char* as key,
// it's going to compare addresses unless I provide my own comparator and hash.
const std::unordered_map<std::string, int> expected_traits{
  {"children:", 3},
  {"cats:", 7},
  {"samoyeds:", 2},
  {"pomeranians:", 3},
  {"akitas:", 0},
  {"vizslas:", 0},
  {"goldfish:", 5},
  {"trees:", 3},
  {"cars:", 2},
  {"perfumes:", 1},
};

const std::unordered_map<std::string, std::function<bool(int, int)>> ops{
  {"cats:", std::greater<>()},
  {"trees:", std::greater<>()},
  {"pomeranians:", std::less<>()},
  {"goldfish:", std::less<>()},
};

const int longest_trait = strlen("pomeranians:");

bool match1(const char* trait, int value) {
  return expected_traits.at(trait) == value;
}

bool match2(const char* trait, int value) {
  const auto it = ops.find(trait);
  if (it == ops.end()) {
    return match1(trait, value);
  }
  return it->second(value, expected_traits.at(trait));
}

void solve(std::istream& is) {
  std::string s;
  int sue1 = 0;
  int sue2 = 0;

  while (getline(is, s)) {
    int sue;
    char trait1[longest_trait + 1];
    int val1;
    char trait2[longest_trait + 1];
    int val2;
    char trait3[longest_trait + 1];
    int val3;

    if (sscanf(s.c_str(), "Sue %d: %s %d, %s %d, %s %d", &sue, trait1, &val1, trait2, &val2, trait3, &val3) < 7) {
      std::cerr << "bad aunt " << s << std::endl;
      std::abort();
    }

    if (match1(trait1, val1) && match1(trait2, val2) && match1(trait3, val3)) {
      if (sue1) {
        std::cerr << "already have sue 1 " << sue1 << " and " << sue << std::endl;
        std::abort();
      }
      sue1 = sue;
    }
    if (match2(trait1, val1) && match2(trait2, val2) && match2(trait3, val3)) {
      if (sue2) {
        std::cerr << "already have sue 2 " << sue2 << " and " << sue << std::endl;
        std::abort();
      }
      sue2 = sue;
    }
  }

  if (sue1) {
    std::cout << sue1 << std::endl;
  } else {
    std::cerr << "no sue 1" << std::endl;
  }

  if (sue2) {
    std::cout << sue2 << std::endl;
  } else {
    std::cerr << "no sue 2" << std::endl;
  }

  if (!sue1 || !sue2) {
    std::abort();
  }
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
