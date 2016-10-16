#include "item.h"
#include "class.h"
#include "spell.h"

#ifndef _JORG_CHARACTER_H
#define _JORG_CHARACTER_H

typedef enum { WARRIOR, WIZARD, ROGUE, PRIEST } class_t;
typedef struct
{
  char name[40];
  class_t class;
  unsigned int level;
  stats_t stats;
  item_t inventory[30];
  int gold_count;
} character_t;

typedef struct
{
  char name[40];
  class_t class;
} character_options_t;

character_t *character_create();
void character_free(character_t *ptr);
void get_class_name(char answer[], class_t class);
character_t *character;

#endif /* _JORG_CHARACTER_H */
