#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include <tuple>

namespace {

using Equip = std::tuple<int, int, int>;

const std::vector<Equip> weapons{
  {8, 4, 0},
  {10, 5, 0},
  {25, 6, 0},
  {40, 7, 0},
  {74, 8, 0},
};

const std::vector<Equip> armours{
  {0, 0, 0},
  {13, 0, 1},
  {31, 0, 2},
  {53, 0, 3},
  {75, 0, 4},
  {102, 0, 5},
};

const std::vector<Equip> rings{
  {0, 0, 0},
  {25, 1, 0},
  {50, 2, 0},
  {100, 3, 0},
  {20, 0, 1},
  {40, 0, 2},
  {80, 0, 3},
};

bool win(int patk, int pdef, int bhp, int batk, int bdef) {
  const int pdmg = std::max(patk - bdef, 1);
  const int bdmg = std::max(batk - pdef, 1);
  const int pturns = (bhp + pdmg - 1) / pdmg;
  const int bturns = (100 + bdmg - 1) / bdmg;
  return pturns <= bturns;
}

template<std::size_t I> int gets(Equip e1, Equip e2, Equip e3, Equip e4) {
  return std::get<I>(e1) + std::get<I>(e2) + std::get<I>(e3) + std::get<I>(e4);
}

void solve(int bhp, int batk, int bdef) {
  int min_win = std::numeric_limits<int>::max();
  int max_lose = 0;
  for (const Equip& weapon : weapons) {
    for (const Equip& armour : armours) {
      for (const Equip& ring1 : rings) {
        for (const Equip& ring2 : rings) {
          if (ring1 == ring2 && std::get<0>(ring1) != 0) {
            continue;
          }
          const int cost = gets<0>(weapon, armour, ring1, ring2);
          const int atk = gets<1>(weapon, armour, ring1, ring2);
          const int def = gets<2>(weapon, armour, ring1, ring2);
          if (win(atk, def, bhp, batk, bdef)) {
            min_win = std::min(cost, min_win);
          } else {
            max_lose = std::max(cost, max_lose);
          }
        }
      }
    }
  }
  std::cout << min_win << std::endl;
  std::cout << max_lose << std::endl;
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
  if (i.size() < 3) {
    std::cerr << "bad, not enough ints in " << s << std::endl;
    std::abort();
  }
  solve(i[0], i[1], i[2]);
}

}

int main(int ac, char** av) {
  if (ac > 3) {
    solve(atoi(av[1]), atoi(av[2]), atoi(av[3]));
  } else if (ac > 1) {
    std::ifstream ifile{av[1]};
    solve(ifile);
  } else {
    solve(std::cin);
  }
  return 0;
}
