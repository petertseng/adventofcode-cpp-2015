#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "md5.h"

namespace {

void solve(const char* key) {
  const int keylen = strnlen(key, 8);

  char to_hash[30];
  strncpy(to_hash, key, keylen);
  to_hash[keylen] = '0';
  to_hash[keylen + 1] = '\0';

  int digits = 0;
  int targetlen = 5;

  for (int i = 1; ; i++) {
    // Unfortunately I find it necessary to optimise writing into the buffer,
    // only overwriting digits that are due to change.
    // Cuts runtime in about 2/3 compared to sprintf:
    // digits = sprintf(to_hash + keylen, "%d", i);
    for (int j = keylen + digits - 1; j >= keylen; j--) {
      if (to_hash[j] == '9') {
        // change a 9 to 0 and increase the next digit
        // (e.g. 19 -> 20, etc.)
        to_hash[j] = '0';
      } else {
        to_hash[j]++;
        break;
      }
    }
    if (to_hash[keylen] == '0') {
      // we reached a new power of 10; extend appropriately.
      to_hash[keylen + digits] = '0';
      to_hash[keylen] = '1';
      digits++;
      // null byte not necessary (md5.update takes explicit length),
      // but useful if I ever want to print it
      to_hash[keylen + digits] = '\0';
    }

    MD5 md5{to_hash, static_cast<unsigned int>(keylen + digits)};

    if (md5.has_leading_zeroes(targetlen)) {
      std::cout << i << std::endl;
      if (targetlen == 6) {
        return;
      }
      targetlen++;
    }
  }
}

void solve(std::istream& is) {
  char key[9];
  is.getline(key, 9);
  solve(key);
}

}

int main(int ac, char** av) {
  if (ac > 1) {
    struct stat statbuf;
    if (stat(av[1], &statbuf) == 0) {
      std::ifstream ifile{av[1]};
      solve(ifile);
    } else {
      solve(av[1]);
    }
  } else {
    solve(std::cin);
  }
  return 0;
}
