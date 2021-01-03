#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

class Reindeer {
public:
  Reindeer(int vel, int run_time, int rest_time) : vel(vel), run_time(run_time), rest_time(rest_time) {}

  int tick() {
    if (running) {
      dist += vel;
      if (++time == run_time) {
        running = false;
        time = 0;
      }
    } else if (++time == rest_time) {
      running = true;
      time = 0;
    }
    return dist;
  }

  // yeah don't care enough to make getters for these.
  int dist = 0;
  int points = 0;
private:
  const int vel;
  const int run_time;
  const int rest_time;
  bool running = true;
  int time = 0;
};

void must_be_word(std::istream& is, const char* word, char delim = ' ') {
  std::string s;
  getline(is, s, delim);
  if (s != word) {
    std::cerr << "bad, " << s << " should be " << word << std::endl;
    std::abort();
  }
}

int must_int(std::istream& is) {
  std::string s;
  getline(is, s, ' ');
  return stoi(s);
}

void solve(std::istream& is) {
  std::vector<Reindeer> reindeer;

  std::string name;
  while (getline(is, name, ' ')) {
    must_be_word(is, "can");
    must_be_word(is, "fly");
    const int vel = must_int(is);
    must_be_word(is, "km/s");
    must_be_word(is, "for");
    const int run_time = must_int(is);
    must_be_word(is, "seconds,");
    must_be_word(is, "but");
    must_be_word(is, "then");
    must_be_word(is, "must");
    must_be_word(is, "rest");
    must_be_word(is, "for");
    const int rest_time = must_int(is);
    must_be_word(is, "seconds.", '\n');
    reindeer.emplace_back(vel, run_time, rest_time);
  }

  int max_dist = 0;

  for (int t = 0; t < 2503; t++) {
    for (Reindeer& r : reindeer) {
      max_dist = std::max(r.tick(), max_dist);
    }
    for (Reindeer& r : reindeer) {
      if (r.dist == max_dist) {
        r.points++;
      }
    }
  }

  std::cout << max_dist << std::endl;

  int max_points = 0;
  for (const Reindeer& r : reindeer) {
    max_points = std::max(r.points, max_points);
  }

  std::cout << max_points << std::endl;
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
