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
 