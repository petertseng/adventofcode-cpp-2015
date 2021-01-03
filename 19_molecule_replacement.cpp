#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

void add_replacements(const std::string& s, const std::string& from, const std::vector<std::string>& tos, std::unordered_set<std::string>& r) {
  for (size_t pos = s.find(from); pos != std::string::npos; pos = s.find(from, pos + 1)) {
    for (const std::string& to : tos) {
      std::string replaced{s};
      replaced.replace(pos, from.size(), to);
      r.insert(replaced);
    }
  }
}

int count(const std::string& haystack, const std::string& needle) {
  int n = 0;
  for (size_t pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1)) {
    n++;
  }
  return n;
}

int count_upper(const std::string& s) {
  int n = 0;
  for (const char& c : s) {
    if ('A' <= c && c <= 'Z') {
      n++;
    }
  }
  return n;
}

void must_be_word(std::istream& is, const char* word) {
  std::string s;
  getline(is, s, ' ');
  if (s != word) {
    std::cerr << "bad, " << s << " should be " << word << std::endl;
    std::abort();
  }
}

void solve(std::istream& is) {
  std::unordered_map<std::string, std::vector<std::string>> replacements;

  std::string l;
  std::string molecule;
  while (getline(is, l, ' ')) {
    if (l.size() > 2) {
      molecule = l;
      break;
    }

    must_be_word(is, "=>");

    std::string r;
    getline(is, r);

    replacements[l].push_back(r);
  }

  std::unordered_set<std::string> after_replace;
  for (const auto& rep : replacements) {
    add_replacements(molecule, rep.first, rep.second, after_replace);
  }
  std::cout << after_replace.size() << std::endl;

  int e_increase = 0;
  for (const std::string& s : replacements["e"]) {
    e_increase = std::max(count_upper(s) - 1, e_increase);
  }
  const int elements = count_upper(molecule);
  const int rn = count(molecule, "Rn");
  const int y = count(molecule, "Y");
  const int ar = count(molecule, "Ar");
  if (rn != ar) {
    std::cerr << "bad, " << rn << " Rn != " << ar << " Ar" << std::endl;
    std::abort();
  }
  std::cout << elements - 2 * (rn + y) - e_increase << std::endl;
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
