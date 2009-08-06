/* Copyright (C) 2003 Free Software Foundation.

   Check that the RTL expansion of floating point exponentiation by
   a constant integer doesn't break anything and produces the expected
   results.

   Written by Roger Sayle, 20th June 2003.  */

/* { dg-do run } */
/* { dg-options "-O2 -ffast-math" } */

extern float pow(float,float);
extern void abort(void);

float foo (float x)
{
  return pow (x, 6);
}

float bar (float x)
{
  return pow (x, -4);
}

int main()
{
  if (foo (2.0) != 64.0)
    abort ();

  if (bar (2.0) != 0.0625)
    abort ();

  return 0;
}

