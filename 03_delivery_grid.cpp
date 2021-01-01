#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>

namespace {

using Pos2 = std::pair<int, int>;

// Pairs aren't hashable by default; must provide hasher.
struct coord_pair_hash {
  std::size_t operator() (const Pos2& pair) const {
    return std::hash<int>{}((pair.first << 30) | pair.second);
  }
};

void solve(std::istream& is) {
  Pos2 santa_solo{0, 0};
  std::unordered_set<Pos2, coord_pair_hash> houses_solo{{0, 0}};

  Pos2 santa_pair{0, 0};
  Pos2 robosanta{0, 0};
  std::unordered_set<Pos2, coord_pair_hash> houses_pair{{0, 0}};

  int i = 0;
  char c;
  while (is.get(c)) {
    i++;
    Pos2& santa_move = i % 2 ? santa_pair : robosanta;
    switch (c) {
    case '^':
      santa_solo.first -= 1;
      santa_move.first -= 1;
      break;
    case 'v':
      santa_solo.first += 1;
      santa_move.first += 1;
      break;
    case '<':
      santa_solo.second -= 1;
      santa_move.second -= 1;
      break;
    case '>':
      santa_solo.second += 1;
      santa_move.second += 1;
      break;
    case '\n':
      break;
    default:
      std::cerr << "bad char " << c << std::endl;
      std::abort();
    }

    houses_solo.insert(santa_solo);
    houses_pair.insert(santa_move);
  }

  std::cout << houses_solo.size() << std::endl;
  std::cout << houses_pair.size() << std::endl;
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
