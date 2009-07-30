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

void writenum(unsigned long long num, unsigned char len, unsigned int *map, unsigned int p, unsigned int offset)
{
    unsigned char figure;
    p += len - 1;
    while (len && num) {
      figure = num % 10;
      if (num || figure) map[p] = figure + offset;
      num /= 10;
      p--;
      len--;
    }
}
