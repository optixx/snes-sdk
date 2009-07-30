void writestring(unsigned char *st, unsigned int *map, unsigned int p, unsigned int offset)
{
  unsigned int sp = p; /* start position */
  unsigned int c;
  while ((c = *st)) {
    if (c == '\n') {
      sp += 0x20;
      p = sp;
    }
    else {
      map[p] = c + offset;
      p++;
    }
    st++;
  }
}

void writebcd(long long bcd, unsigned int *map, unsigned int p, unsigned int offset)
{
    unsigned char shift = 8;
    unsigned char write_zeros = 0;
    unsigned char figure;
    while (shift--) {
      figure = (bcd & 0xf0000000ULL) >> 28;
      if (figure) write_zeros = 1;
      if (write_zeros) map[p] = figure + offset;
      bcd <<= 4;
      p++;
    }
}
 