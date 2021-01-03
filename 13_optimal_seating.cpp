#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

int happiest(std::vector<std::string>& people, std::unordered_map<std::string, int>& deltas) {
  int max = 0;

  // fix the person at the start,
  // only permuting all the others.
  // (thus, < instead of <=)
  int num_perms = 1;
  for (unsigned int i = 1; i < people.size(); i++) {
    num_perms *= i;
  }

  for (int i = 0; i < num_perms; i++) {
    int dist = 0;
    for (unsigned int i = 0; i < people.size() - 1; i++) {
      dist += deltas[people[i] + ',' + people[i + 1]];
    }
    dist += deltas[people[0] + ',' + people[people.size() - 1]];
    if (false) {
      for (const std::string& s : people) {
        std::cout << s << ' ';
      }
      std::cout << " dist " << dist << std::endl;
    }
    max = std::max(dist, max);
    std::next_permutation(people.begin() + 1, people.end());
  }

  return max;
}

void must_be_word(std::istream& is, const char* word) {
  std::string s;
  getline(is, s, ' ');
  if (s != word) {
    std::cerr << "bad, " << s << " should be " << word << std::endl;
    std::abort();
  }
}

int gain_or_lose(std::istream& is) {
  std::string s;
  getline(is, s, ' ');
  if (s == "gain") {
    return 1;
  } else if (s == "lose") {
    return -1;
  }
  std::cerr << "bad, " << s << " not gain or lose " << std::endl;
  std::abort();
}

int must_int(std::istream& is) {
  std::string s;
  getline(is, s, ' ');
  return stoi(s);
}

void solve(std::istream& is) {
  std::string first_person;
  std::string person1;
  std::vector<std::string> people;
  std::unordered_map<std::string, int> deltas;

  while (getline(is, person1, ' ')) {
    if (people.empty()) {
      first_person = person1;
      people.push_back(person1);
    }

    must_be_word(is, "would");

    const int polarity = gain_or_lose(is);
    const int happiness = must_int(is);

    must_be_word(is, "happiness");
    must_be_word(is, "units");
    must_be_word(is, "by");
    must_be_word(is, "sitting");
    must_be_word(is, "next");
    must_be_word(is, "to");

    std::string person2;
    getline(is, person2);
    person2.pop_back();
    if (person1 == first_person) {
      people.push_back(person2);
    }

    deltas[person1 + ',' + person2] += polarity * happiness;
    deltas[person2 + ',' + person1] += polarity * happiness;
  }

  std::cout << happiest(people, deltas) << std::endl;
  people.emplace_back("me");
  std::cout << happiest(people, deltas) << std::endl;
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
