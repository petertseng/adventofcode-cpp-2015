#include <fstream>
#include <iostream>
#include <string>
#ifdef REGEX
#include <regex>
#else
#include <unordered_map>
#endif

namespace {

#ifdef REGEX
bool nice1(const std::string& pw) {
  static const std::regex badpair{"ab|cd|pq|xy"};
  static const std::regex vowels{"(?:[aeiou].*){3,}"};
  static const std::regex repeat{"(.)\\1"};
  return !std::regex_search(pw, badpair) && std::regex_search(pw, vowels) && std::regex_search(pw, repeat);
}

bool nice2(const std::string& pw) {
  static const std::regex aba{"(.).\\1"};
  static const std::regex repeat_pair{"(..).*\\1"};
  return std::regex_search(pw, aba) && std::regex_search(pw, repeat_pair);
}
#else
bool nice1(const std::string& pw) {
  int vowels = 0;
  bool pair = false;
  char prev = '\0';

  for (const char& c : pw) {
    if (c == prev) {
      pair = true;
    }
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
      vowels++;
    }
    if ((prev == 'a' || prev == 'c' || prev == 'p' || prev == 'x') && prev + 1 == c) {
      return false;
    }
    prev = c;
  }
  return pair && vowels >= 3;
}

bool nice2(const std::string& pw) {
  std::unordered_map<short, int> pair_pos;
  bool aba = false;
  bool repeat_pair = false;
  char prev2 = '\0';
  char prev1 = '\0';
  int i = -1;

  for (const char& c : pw) {
    i++;

    if (c == prev2) {
      if (repeat_pair) {
        return true;
      }
      aba = true;
    }

    short pair = (static_cast<short>(prev1) << 8) | c;
    auto prev_pair_pos = pair_pos.find(pair);
    if (prev_pair_pos == pair_pos.end()) {
      pair_pos.emplace(pair, i);
    } else if (prev_pair_pos->second < i - 1) {
      if (aba) {
        return true;
      }
      repeat_pair = true;
    }

    prev2 = prev1;
    prev1 = c;
  }
  return false;
}
#endif

void solve(std::istream& is) {
  std::string s;
  int num_nice1 = 0;
  int num_nice2 = 0;
  while (getline(is, s)) {
    if (nice1(s)) {
      num_nice1++;
    }
    if (nice2(s)) {
      num_nice2++;
    }
  }
  std::cout << num_nice1 << std::endl;
  std::cout << num_nice2 << std::endl;
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
