#include "class.h"

#ifndef _JORG_CREATURE_H
#define _JORG_CREATURE_H

typedef struct
{
  char *name;
  char *description;

  stats_t stats;
  int level;
  int hp;
  int ca;
  int bba;

  int action_number;
  char *actions;
} creature_t;

creature_t creatures[100];
creature_t companions[10];

#endif /* _JORG_CREATURE_H */
