#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace {

const int pwlen = 8;

constexpr bool confusing(char c) {
  return c == 'i' || c == 'o' || c == 'l';
}

void nextpw(char* pw) {
  for (int i = pwlen - 1; i >= 0; i--) {
    if (pw[i] == 'z') {
      pw[i] = 'a';
    } else {
      if (confusing(++pw[i])) {
        pw[i]++;
      }
      return;
    }
  }
}

constexpr bool valid(const char* pw) {
  uint32_t seen_pair = 0;
  int pairs = 0;
  bool straight = false;

  char prev2 = '\0';
  char prev1 = '\0';
  for (int i = 0; i < pwlen; i++) {
    const char c = *pw++;
    if (confusing(c)) {
      return false;
    }
    if (c == prev1) {
      uint32_t bit = 1 << (c - 'a');
      if (!(seen_pair & bit)) {
        pairs++;
        seen_pair |= bit;
      }
    }
    if (prev2 && prev2 + 1 == prev1 && prev1 + 1 == c) {
      straight = true;
    }
    prev2 = prev1;
    prev1 = c;
  }

  return straight && pairs >= 2;
}
static_assert(!valid("hijklmmn"), "hijklmmn valid");
static_assert(!valid("abbceffg"), "abbceffg valid");
static_assert(!valid("abbcegjk"), "abbcegjk valid");
static_assert(valid("abcdffaa"), "abcdffaa not valid");
static_assert(valid("ghjaabcc"), "ghjaabcc not valid");

void solve(char* pw) {

  do {
    nextpw(pw);
  } while (!valid(pw));

  std::cout << pw << std::endl;

  do {
    nextpw(pw);
  } while (!valid(pw));

  std::cout << pw << std::endl;
}

void solve(std::istream& is) {
  char pw[pwlen + 1];
  is.getline(pw, pwlen + 1);
  solve(pw);
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
