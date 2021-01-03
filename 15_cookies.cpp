#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include <utility>

namespace {

std::vector<int> vadd(const std::vector<int>& v1, const std::vector<int>& v2) {
  std::vector<int> v3;
  std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(v3), std::plus<>());
  return v3;
}

std::vector<int> vscale(const std::vector<int>& v, int scale) {
  std::vector<int> res;
  std::transform(v.begin(), v.end(), std::back_inserter(res), [scale](int n) { return n * scale; });
  return res;
}

std::pair<int, int> best_cookie(const std::vector<std::vector<int>>& ingredients, unsigned int n, int remaining, const std::vector<int>& traits_so_far) {
  const std::vector<int>& ingredient = ingredients[n];

  if (n == ingredients.size() - 1) {
    std::vector<int> traits = vadd(traits_so_far, vscale(ingredient, remaining));
    const int calories = traits.back();
    traits.pop_back();

    for (const int& t : traits) {
      if (t <= 0) {
        return {0, 0};
      }
    }

    const int score = std::accumulate(traits.begin(), traits.end(), 1, std::multiplies<>());
    return {score, calories == 500 ? score : 0};
  }

  int best = 0;
  int best500 = 0;
  for (int amt = 0; amt <= remaining; amt++) {
    const std::vector<int> traits_with = vadd(traits_so_far, vscale(ingredient, amt));
    const std::pair<int, int> res = best_cookie(ingredients, n + 1, remaining - amt, traits_with);
    best = std::max(res.first, best);
    best500 = std::max(res.second, best500);
  }
  return {best, best500};
}

std::vector<int> ints(const std::string& s) {
  std::vector<int> ints;
  int curnum = 0;
  bool negative = false;
  bool in_num = false;

  for (const char& c : s) {
    if (c == '-') {
      if (in_num) {
        std::cerr << "bad neg " << s << std::endl;
        std::abort();
      }
      in_num = true;
      negative = true;
    } else if ('0' <= c && c <= '9') {
      curnum = curnum * 10 + (c - '0');
      in_num = true;
    } else if (in_num) {
      ints.push_back(curnum * (negative ? -1 : 1));
      curnum = 0;
      negative = false;
      in_num = false;
    }
  }
  if (in_num) {
    ints.push_back(curnum * (negative ? -1 : 1));
  }

  return ints;
}

void solve(std::istream& is) {
  std::vector<std::vector<int>> ingredients;

  std::string s;
  while (getline(is, s)) {
    ingredients.emplace_back(ints(s));
  }

  if (false) {
    for (const std::vector<int>& ingredient : ingredients) {
      for (const int& i : ingredient) {
        std::cout << i << ',';
      }
      std::cout << std::endl;
    }
  }

  std::pair<int, int> best = best_cookie(ingredients, 0, 100, vscale(ingredients[0], 0));
  std::cout << best.first << std::endl;
  std::cout << best.second << std::endl;
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
