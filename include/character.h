#include <stdbool.h>
#include "item.h"
#include "class.h"
#include "spell.h"
#include "creature.h"

#ifndef _JORG_CHARACTER_H
#define _JORG_CHARACTER_H

typedef enum { WARRIOR, WIZARD, ROGUE, PRIEST } class_t;
typedef struct
{
  char name[40];
  class_t class;
  unsigned int level;
  unsigned int experience;
  stats_t stats;

  int bba;
  int ca;
  int max_hp;
  int curr_hp;

  item_t inventory[30];
  item_t eqquiped_weap;
  item_t eqquiped_armor;
  int gold_count;

  spell_t spell_list[sizeof(spells)];
  int spell_count;
  int max_mana;
  int curr_mana;

  _Bool has_companion;
  creature_t companion;

} character_t;

typedef struct
{
  char name[40];
  class_t class;
} character_options_t;

void character_create();
void character_free(character_t *ptr);
void get_class_name(char answer[], class_t class);
void add_item(int id, int n);
void rmv_item(int index, int n);
void add_spell(int id);
void death();
void reorganize_inventory();

character_t *character;

#endif /* _JORG_CHARACTER_H */
