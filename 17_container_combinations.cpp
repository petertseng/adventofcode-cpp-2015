#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace {

using intmap = std::unordered_map<int, int>;

intmap ways(const std::vector<int>& containers, int weight, unsigned int min_index, int num_used) {
  if (weight == 0) {
    return {{num_used, 1}};
  }
  if (min_index >= containers.size()) {
    return {};
  }
  if (weight < 0) {
    return {};
  }

  const intmap ways_without{ways(containers, weight, min_index + 1, num_used)};
  intmap ways_with{ways(containers, weight - containers[min_index], min_index + 1, num_used + 1)};

  for (const auto& it : ways_without) {
    ways_with[it.first] += it.second;
  }

  return ways_with;
}

void solve(std::istream& is) {
  std::vector<int> containers;
  int cont;
  while (is >> cont) {
    containers.emplace_back(cont);
  }

  intmap ways150 = ways(containers, 150, 0, 0);
  int total_ways = 0;
  int min_containers = containers.size();
  int ways_with_min = 0;

  for (const auto& it : ways150) {
    total_ways += it.second;
    if (it.first < min_containers) {
      min_containers = it.first;
      ways_with_min = it.second;
    }
  }

  std::cout << total_ways << std::endl;
  std::cout << ways_with_min << std::endl;
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
