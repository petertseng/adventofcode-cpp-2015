#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

void life_step(std::vector<bool>& dest, const std::vector<bool>& src, int height, int width) {
  for (int y = 0; y < height; y++) {
    const int miny = std::max(y - 1, 0);
    const int maxy = std::min(y + 1, height - 1);
    for (int x = 0; x < width; x++) {
      const int minx = std::max(x - 1, 0);
      const int maxx = std::min(x + 1, width - 1);
      int neighs = 0;
      for (int ny = miny; ny <= maxy; ny++) {
        for (int nx = minx; nx <= maxx; nx++) {
          neighs += src[ny * width + nx];
        }
      }
      const int pos = y * width + x;
      dest[pos] = neighs == 3 || (neighs == 4 && src[pos]);
    }
  }
}

void corners_on(std::vector<bool>& v, int height, int width) {
  v[0] = true;
  v[width - 1] = true;
  v[(height - 1) * width] = true;
  v[height * width - 1] = true;
}

void solve(std::istream& is) {
  int width = 0;
  int height = 0;
  std::vector<bool> init_lights;

  std::string s;
  while (getline(is, s)) {
    height++;
    if (!width) {
      width = static_cast<int>(s.size());
      init_lights.reserve(width * width);
    }
    if (static_cast<int>(s.size()) != width) {
      std::cerr << "bad width " << s.size() << " != " << width << std::endl;
      std::abort();
    }

    for (const char& c : s) {
      switch(c) {
      case '#':
        init_lights.push_back(true);
        break;
      case '.':
        init_lights.push_back(false);
        break;
      default:
        std::cerr << "bad " << c << " in " << s << std::endl;
        std::abort();
      }
    }
  }

  std::vector<bool> lights1a{init_lights};
  std::vector<bool> lights1b(height * width);

  for (int i = 0; i < 50; i++) {
    life_step(lights1b, lights1a, height, width);
    life_step(lights1a, lights1b, height, width);
  }

  std::cout << std::accumulate(lights1a.begin(), lights1a.end(), 0) << std::endl;

  std::vector<bool> lights2a{init_lights};
  std::vector<bool> lights2b(height * width);

  for (int i = 0; i < 50; i++) {
    corners_on(lights2a, height, width);
    life_step(lights2b, lights2a, height, width);
    corners_on(lights2b, height, width);
    life_step(lights2a, lights2b, height, width);
  }
  corners_on(lights2a, height, width);

  std::cout << std::accumulate(lights2a.begin(), lights2a.end(), 0) << std::endl;
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
