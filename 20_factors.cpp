#include <array>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unordered_map>

namespace {

const std::array<int, 15> PRIMES{49, 43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
const int max_prime = PRIMES[0];

// askalski's tip:
// https://www.reddit.com/r/adventofcode/comments/po1zel/comment/hd1esc2
int sum_exceeds(int goal, size_t prime_i, std::unordered_map<int, int>& cache) {
  if (prime_i >= PRIMES.size()) {
    return goal;
  }

  int cache_key = goal * static_cast<int>(PRIMES.size()) + prime_i;
  auto cached = cache.find(cache_key);
  if (cached != cache.end()) {
    return cached->second;
  }

  int best = sum_exceeds(goal, prime_i + 1, cache);

  int prime = PRIMES[prime_i];
  int prime_power = 1;
  int prime_sum = 1;

  while (prime_sum < goal) {
    prime_power *= prime;
    prime_sum += prime_power;

    // subproblem: ceil(goal/prime_sum) using only primes less than prime
    int subgoal = (goal + prime_sum - 1) / prime_sum;
    best = std::min(best, prime_power * sum_exceeds(subgoal, prime_i + 1, cache));
  }

  cache.emplace(cache_key, best);
  return best;
}

bool good2(int house, int target) {
  int elves = 0;
  for (int div = 1; div <= 50; div++) {
    if (house % div == 0) {
      elves += house / div;
    }
  }
  return 11 * elves >= target;
}

void solve(int target) {
  std::unordered_map<int, int> cache;
  int house1 = sum_exceeds(target / 10, 0, cache);
  std::cout << house1 << std::endl;

  for (int i = good2(house1, target) ? 0 : house1; i < target; i += 2 * 3 * 5 * 7) {
    if (good2(i, target)) {
      std::cout << i << std::endl;
      return;
    }
  }
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
