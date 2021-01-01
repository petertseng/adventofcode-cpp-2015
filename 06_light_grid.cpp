#include <bitset>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#ifdef REGEX
#include <string>
#include <regex>
#endif

namespace {

const int side_len = 1000;

// These values correspond to their string lengths,
// so that the regex solution just uses the string length.
const int turn = 4;
const int turn_on = 7;
const int turn_off = 8;
const int toggle = 6;
const int longest_line = strlen("turn off xxx,yyy through xxx,yyy") + 1;

void solve(std::istream& is) {
  std::bitset<side_len * side_len> on_off{};
  int brightness[side_len * side_len]{};

#ifdef REGEX
  static const std::regex command{"^(turn on|turn off|toggle) (\\d+),(\\d+) through (\\d+),(\\d+)$"};
  std::string s;
  std::smatch sm;
  while (getline(is, s)) {
    if (!std::regex_match(s, sm, command)) {
      std::cerr << "bad command " << s << std::endl;
      std::abort();
    }
    int ymin = stoi(sm[2]);
    int xmin = stoi(sm[3]);
    int ymax = stoi(sm[4]);
    int xmax = stoi(sm[5]);
    int op = sm.length(1);
#else
  char line[longest_line];
  while (is.getline(line, longest_line)) {
    char* start = line + (line[turn] == ' ' ? turn : 0);
    char opz[toggle + 1];
    int ymin;
    int xmin;
    int ymax;
    int xmax;
    if (sscanf(start, "%s %d,%d through %d,%d", opz, &ymin, &xmin, &ymax, &xmax) < 5) {
      std::cerr << "bad command " << line << std::endl;
      std::abort();
    }
    int op;
    if (strcmp(opz, "on") == 0) {
      op = turn_on;
    } else if (strcmp(opz, "off") == 0) {
      op = turn_off;
    } else if (strcmp(opz, "toggle") == 0) {
      op = toggle;
    } else {
      std::cerr << "bad command " << line << std::endl;
      std::abort();
    }
#endif

    for (int y = ymin; y <= ymax; y++) {
      for (int x = xmin; x <= xmax; x++) {
        const int pos = y * side_len + x;
        switch (op) {
        case turn_on:
          on_off.set(pos);
          brightness[pos]++;
          break;
        case turn_off:
          on_off.reset(pos);
          if (brightness[pos] > 0) {
            brightness[pos]--;
          }
          break;
        case toggle:
          on_off.flip(pos);
          brightness[pos] += 2;
          break;
        default:
          // Never happens.
          break;
        }
      }
    }
  }
  std::cout << on_off.count() << std::endl;
  std::cout << std::accumulate(brightness, brightness + side_len * side_len, 0) << std::endl;
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
