#include "snesc.h"


unsigned int getjoystatus(unsigned int j)
{
  return snesc_controllers[j];
}

void clearjoy(unsigned int j)
{
  snesc_controllers[j] = 0;
}
