//Note can contain the following effects
// - n() No note
// - h(x) heal of x
// - d(x) damage of x
#include "creature.h"
#include "jorg.h"

#ifndef _JORG_ITEM_H
#define _JORG_ITEM_H

typedef enum { MISC, WEAPON, ARMOR, CONSUMABLE, BOOK } item_type_t;
typedef struct
{
  char name[30];
  int value;
  item_type_t type;
  char note[50];

  int book_id;

  //used in game var
  int count;
} item_t;

item_t items[100];

void creature_affected_effects(char *effects, char *name, creature_t *instance, creature_t reference);
void player_affected_effects(char *effects, char *name);

#endif /* _JORG_ITEM_H */
