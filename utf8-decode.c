#define FNT_IMPLEMENTATION
#include "fnt.h"

#include <stdint.h>
#include <stdio.h>

int main(int argc, char ** argv) {
  for (int i = 1; i < argc; i++) {
    for (const char * c = argv[i]; *c;) {
      printf("%04x\n", fnt_decode_utf8(&c));
    }
  }
}
