#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>
#include <vector>

namespace {

enum class Spell {
  MagicMissile,
  Drain,
  Shield,
  Poison,
  Recharge,
};

enum class Combatant {
  Player,
  Boss,
};

int cost(Spell spell) {
  switch (spell) {
  case Spell::MagicMissile:
    return 53;
  case Spell::Drain:
    return 73;
  case Spell::Shield:
    return 113;
  case Spell::Poison:
    return 173;
  case Spell::Recharge:
    return 229;
  default:
    std::cerr << "bad spell" << std::endl;
    std::abort();
  }
}

const size_t time_bits = 3;
const size_t boss_hp_bits = 6;
const size_t my_hp_bits = 7;
const size_t mp_bits = 64 - time_bits * 3 - boss_hp_bits - my_hp_bits;
// A little too big for 32 bits,
// since player can recharge a lot of MP.
using game_hash_t = uint64_t;

struct Game {
  Game(int bhp, int batk, bool hard) : boss_hp(bhp), boss_dmg(batk), hard(hard) {
    start_turn_effects(Combatant::Player);
  };

  // are hashed:
  int my_hp = 50;
  int mp = 500;
  int boss_hp;
  int shield = 0;
  int poison = 0;
  int recharge = 0;

  // are not hashed:
  const int boss_dmg;
  const bool hard;

  game_hash_t hash() const;
  bool over() const;
  void start_turn_effects(Combatant c);
  void boss_attack();
  std::pair<Game, bool> cast(Spell s) const;
};

game_hash_t Game::hash() const {
  if (static_cast<game_hash_t>(mp) >= (1ull << mp_bits)) {
    std::cerr << "mp too high " << mp << std::endl;
    std::abort();
  }
  if (my_hp >= (1 << my_hp_bits)) {
    std::cerr << "my hp too high " << my_hp << std::endl;
    std::abort();
  }
  return recharge
    | (poison << time_bits)
    | (shield << (time_bits * 2))
    | (boss_hp << (time_bits * 3))
    | (my_hp << (time_bits * 3 + boss_hp_bits))
    | (static_cast<game_hash_t>(mp) << (time_bits * 3 + boss_hp_bits + my_hp_bits))
    ;
}

bool Game::over() const {
  if (my_hp <= 0 && boss_hp <= 0) {
    std::cerr << "both died" << std::endl;
    std::abort();
  }
  return my_hp <= 0 || boss_hp <= 0;
}

void Game::start_turn_effects(Combatant c) {
  if (c == Combatant::Player) {
    if (hard) {
      my_hp--;
    }
    if (my_hp <= 0) {
      return;
    }
  }

  if (shield) {
    shield--;
  }
  if (poison) {
    boss_hp -= 3;
    poison--;
  }
  if (recharge) {
    mp += 101;
    recharge--;
  }
}

void Game::boss_attack() {
  int dealt = std::max(boss_dmg - (shield ? 7 : 0), 1);
  my_hp -= dealt;
}

// Maybe this should return a variant instead
std::pair<Game, bool> Game::cast(Spell s) const {
  if (mp < cost(s)) {
    return {*this, false};
  }
  Game g2 = *this;

  g2.mp -= cost(s);
  switch (s) {
  case Spell::MagicMissile:
    g2.boss_hp -= 4;
    break;
  case Spell::Drain:
    g2.boss_hp -= 2;
    g2.my_hp += 2;
    break;
  case Spell::Shield:
    if (shield) {
      return {g2, false};
    }
    g2.shield = 6;
    break;
  case Spell::Poison:
    if (poison) {
      return {g2, false};
    }
    g2.poison = 6;
    break;
  case Spell::Recharge:
    if (recharge) {
      return {g2, false};
    }
    g2.recharge = 5;
    break;
  default:
    std::cerr << "bad spell" << std::endl;
    std::abort();
  }
  if (g2.over()) {
    return {g2, true};
  }

  g2.start_turn_effects(Combatant::Boss);
  if (g2.over()) {
    return {g2, true};
  }

  g2.boss_attack();
  if (g2.over()) {
    return {g2, true};
  }

  g2.start_turn_effects(Combatant::Player);
  return {g2, true};
}

struct Search {
  std::unordered_set<uint32_t> seen{};
  int best_so_far = std::numeric_limits<int>::max();
  int least_mp(const Game& g, int cost_so_far);
};

int Search::least_mp(const Game& game, int cost_so_far = 0) {
  if (cost_so_far > best_so_far) {
    return cost_so_far;
  }

  int min_win = std::numeric_limits<int>::max();
  for (const Spell& s : {Spell::Poison, Spell::Shield, Spell::Recharge, Spell::MagicMissile, Spell::Drain}) {
    const std::pair<Game, bool> game_valid = game.cast(s);
    if (!std::get<bool>(game_valid)) {
      continue;
    }

    const Game new_game = std::get<Game>(game_valid);
    const int new_cost = cost_so_far + cost(s);

    if (new_game.over()) {
      if (new_game.my_hp <= 0) {
        continue;
      }
      if (new_game.boss_hp <= 0) {
        min_win = std::min(new_cost, min_win);
        best_so_far = std::min(new_cost, best_so_far);
        continue;
      }
    }

    if (seen.find(new_game.hash()) != seen.end()) {
      continue;
    }
    seen.insert(new_game.hash());

    min_win = std::min(least_mp(new_game, new_cost), min_win);
  }

  return min_win;
}

void solve(int bhp, int batk) {
  if (bhp >= (1 << boss_hp_bits)) {
    std::cerr << "boss hp too high " << bhp << std::endl;
    std::abort();
  }

  const Game g{bhp, batk, false};
  Search search;
  std::cout << search.least_mp(g) << std::endl;

  const Game g2{bhp, batk, true};
  Search search2;
  std::cout << search2.least_mp(g2) << std::endl;
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
  if (i.size() < 2) {
    std::cerr << "bad, not enough ints in " << s << std::endl;
    std::abort();
  }
  solve(i[0], i[1]);
}

}

int main(int ac, char** av) {
  if (ac > 2) {
    solve(atoi(av[1]), atoi(av[2]));
  } else if (ac > 1) {
    std::ifstream ifile{av[1]};
    solve(ifile);
  } else {
    solve(std::cin);
  }
  return 0;
}
