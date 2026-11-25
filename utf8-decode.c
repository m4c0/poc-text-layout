#include <stdint.h>
#include <stdio.h>

int main(int argc, char ** argv) {
  for (int i = 1; i < argc; i++) {
    for (char * c = argv[i]; *c; c++) {
      uint8_t n0 = *c;
      if ((n0 & 0x80) == 0) {
        printf("%02x = %c\n", n0, n0);
      } else if ((n0 & 0xE0) == 0xC0) {
        uint8_t n1 = c[1];
        if ((n1 & 0xC0) == 0x80) {
          int ch = (n0 & ~0xE0) << 6 | (n1 & ~0xC0);
          printf("%04x (%02x %02x)\n", ch, n0, n1);
          c++;
        }
      } else if ((n0 & 0xF0) == 0xE0) {
        uint8_t n1 = c[1];
        if ((n1 & 0xC0) == 0x80) {
          uint8_t n2 = c[2];
          if ((n2 & 0xC0) == 0x80) {
            int ch = (n0 & ~0xF0) << 12 | (n1 & ~0xC0) << 6 | (n2 & ~0xC0);
            printf("%04x (%02x %02x %02x)\n", ch, n0, n1, n2);
            c += 2;
          }
        }
      } else if ((n0 & 0xF8) == 0xF0) {
        uint8_t n1 = c[1];
        if ((n1 & 0xC0) == 0x80) {
          uint8_t n2 = c[2];
          if ((n2 & 0xC0) == 0x80) {
            uint8_t n3 = c[3];
            if ((n3 & 0xC0) == 0x80) {
              int ch = (n0 & ~0xF0) << 18 | (n1 & ~0xC0) << 12 | (n2 & ~0xC0) << 6 | (n3 & ~0xC0);;
              printf("%04x (%02x %02x %02x %02x)\n", ch, n0, n1, n2, n3);
              c += 3;
            }
          }
        }
      } else {
        printf("%04x is invalid or unsupported\n", n0);
      }
    }
  }
}
