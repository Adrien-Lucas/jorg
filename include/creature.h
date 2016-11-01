#include "class.h"

#ifndef _JORG_CREATURE_H
#define _JORG_CREATURE_H
typedef struct
{
  char *name;
  char *description;

  stats_t stats;
  int hp;
  int ca;
  int bba;

  char *actions;

  int xp;
} creature_t;

creature_t creatures[100];
creature_t companions[1];

#endif /* _JORG_CREATURE_H */
