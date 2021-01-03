#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

std::pair<int, int> minmax(std::vector<std::string>& places, std::unordered_map<std::string, int>& dists) {
  int min = std::numeric_limits<int>::max();
  int max = 0;

  // remember that next_permutation returns if lexicographically greater
  // so either I have to sort it before,
  // or I have to count how many perms I expect to make.
  int num_perms = 1;
  for (unsigned int i = 1; i <= places.size(); i++) {
    num_perms *= i;
  }

  for (int i = 0; i < num_perms; i++) {
    int dist = 0;
    for (unsigned int i = 0; i < places.size() - 1; i++) {
      dist += dists[places[i] + ',' + places[i + 1]];
    }
    if (false) {
      for (const std::string& s : places) {
        std::cout << s << ' ';
      }
      std::cout << " dist " << dist << std::endl;
    }
    min = std::min(dist, min);
    max = std::max(dist, max);
    std::next_permutation(places.begin(), places.end());
  }

  return {min, max};
}

void must_be_word(std::istream& is, const char* word) {
  std::string s;
  getline(is, s, ' ');
  if (s != word) {
    std::cerr << "bad, " << s << " should be " << word << std::endl;
    std::abort();
  }
}

int must_dist(std::istream& is) {
  std::string s;
  getline(is, s);
  return stoi(s);
}

void solve(std::istream& is) {
  std::string first_place;
  std::string place1;
  std::vector<std::string> places;
  std::unordered_map<std::string, int> dists;

  while (getline(is, place1, ' ')) {
    if (places.empty()) {
      first_place = place1;
      places.push_back(place1);
    }

    must_be_word(is, "to");

    std::string place2;
    getline(is, place2, ' ');
    if (place1 == first_place) {
      places.push_back(place2);
    }

    must_be_word(is, "=");

    const int dist = must_dist(is);
    dists.emplace(place1 + ',' + place2, dist);
    dists.emplace(place2 + ',' + place1, dist);
  }

  std::pair<int, int> r = minmax(places, dists);

  std::cout << r.first << std::endl;
  std::cout << r.second << std::endl;
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
