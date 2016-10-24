//Note can contain the following effects
// - n() No note
// - h(x) heal of x
// - d(x) damage of x
// - p(x) power + x

#ifndef _JORG_SPELL_H
#define _JORG_SPELL_H

typedef struct
{
  char name[50];
  int cost;
  char *effects;
  int shop_value;
} spell_t;


spell_t spells[2];

#endif /* _JORG_SPELL_H */
