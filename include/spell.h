//Note can contain the following effects
// - n() No note
// - h(x) heal of x
// - d(x) damage of x
// - p(x) power + x

#ifndef _JORG_SPELL_H
#define _JORG_SPELL_H

typedef enum { DAMAGE, HEAL, BUFF } spell_type_t;
typedef struct
{
  char name[50];
  spell_type_t type;
  int cost;
  int power;
  char note[10];
} spell_t;


spell_t spells[2];

#endif /* _JORG_SPELL_H */
