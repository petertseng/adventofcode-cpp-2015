#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

namespace {

int min_packages(const std::vector<int>& packages, int target) {
  int so_far = 0;
  int i = 0;
  for (const int& package : packages) {
    i++;
    so_far += package;
    if (so_far >= target) {
      return i;
    }
  }
  return packages.size();
}

int64_t qe(const std::vector<int>& packages, int groups) {
  const int sum = std::accumulate(packages.begin(), packages.end(), 0);
  if (sum % groups != 0) {
    std::cerr << "bad, " << sum << " not divisible by " << groups << std::endl;
    std::abort();
  }
  const int per_group = sum / groups;
  const int min_size = min_packages(packages, per_group);

  int64_t min_qe = std::numeric_limits<int64_t>::max();

  // I don't really want to use size_t
  const int num_packages = static_cast<int>(packages.size());
  std::vector<bool> select(packages.size());
  for (int size = min_size; size < num_packages; size++) {
    bool found_any = false;
    std::fill(select.begin(), select.begin() + size, true);
    do {
      int sum = 0;
      int64_t qe = 1;
      for (int i = 0; i < num_packages; i++) {
        if (select[i]) {
          sum += packages[i];
          qe *= packages[i];
        }
      }
      if (sum == per_group) {
        // TODO: partition the rest of the packages too
        found_any = true;
        min_qe = std::min(qe, min_qe);
      }
    } while (std::prev_permutation(select.begin(), select.end()));
    if (found_any) {
      return min_qe;
    }
  }
  std::cerr << "can't" << std::endl;
  std::abort();
}

void solve(std::istream& is) {
  std::vector<int> packages;
  int pkg;
  while (is >> pkg) {
    packages.emplace_back(pkg);
  }
  std::sort(packages.rbegin(), packages.rend());
  std::cout << qe(packages, 3) << std::endl;
  std::cout << qe(packages, 4) << std::endl;
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
