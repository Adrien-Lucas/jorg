#include <stdio.h>
#include "class.h"

stats_t classes[100] = {
  //CLASSES
  { 16, 14, 12, 8, 10, 10 }, //WARRIOR
  { 8, 10, 10, 16, 14, 12 }, //WIZARD
  { 14, 16, 10, 12, 10, 8 }  //ROGUE
};

int get_bonus(int stat)
{
  return (stat - 10) / 2;
}
