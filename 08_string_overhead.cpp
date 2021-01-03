#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {

void must_be_hex(char c) {
  if (c < 'a' && c > 'f' && c < '0' && c > '9') {
    std::cerr << "not hex " << c << std::endl;
    std::abort();
  }
}

void solve(std::istream& is) {
  char c;
  int lit_minus_mem = 0;
  int encode_minus_orig = 0;
  bool in_string = false;
  while (is.get(c)) {
    if (!in_string) {
      if (c != '"') {
        std::cerr << "string not start with double quote: " << c << std::endl;
        std::abort();
      }
      in_string = true;
      lit_minus_mem++;
      encode_minus_orig += 2;
      continue;
    }

    switch (c) {
    case '\\':
      encode_minus_orig++;
      is.get(c);
      switch (c) {
      case '\\':
      case '"':
        lit_minus_mem++;
        encode_minus_orig++;
        break;
      case 'x':
        is.get(c);
        must_be_hex(c);
        is.get(c);
        must_be_hex(c);
        lit_minus_mem += 3;
        break;
      default:
        std::cerr << "bad escape" << std::endl;
        std::abort();
      }
      break;
    case '"':
      is.get(c);
      if (c != '\n') {
        std::cerr << "string closed too early: " << c << std::endl;
        std::abort();
      }
      in_string = false;
      lit_minus_mem++;
      encode_minus_orig += 2;
      break;
    default:
      // ok
      break;
    }
  }
  if (in_string) {
    std::cerr << "unclosed string" << std::endl;
    std::abort();
  }
  std::cout << lit_minus_mem << std::endl;
  std::cout << encode_minus_orig << std::endl;
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
