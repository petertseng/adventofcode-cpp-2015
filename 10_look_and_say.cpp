#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace {

std::string look_and_say(const std::string& s) {
  int count = 0;
  char prev = '\0';
  std::string next;

  for (const char& c : s) {
    if (c != prev) {
      if (count) {
        next.append(1, '0' + count);
        next.append(1, prev);
      }
      count = 1;
      prev = c;
    } else {
      count++;
    }
  }
  next.append(1, '0' + count);
  next.append(1, prev);

  return next;
}

void solve(const char* seed) {
  std::string cur{seed};

  for (int i = 0; i < 40; i++) {
    cur = look_and_say(cur);
  }

  std::cout << cur.size() << std::endl;

  for (int i = 0; i < 10; i++) {
    cur = look_and_say(cur);
  }

  std::cout << cur.size() << std::endl;
}

void solve(std::istream& is) {
  char seed[11];
  is.getline(seed, 11);
  solve(seed);
}

}

int main(int ac, char** av) {
  if (ac > 1) {
    struct stat statbuf;
    if (stat(av[1], &statbuf) == 0) {
      std::ifstream ifile{av[1]};
      solve(ifile);
    } else {
      solve(av[1]);
    }
  } else {
    solve(std::cin);
  }
  return 0;
}
