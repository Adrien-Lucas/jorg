//Note can contain the following effects
// - n() No note
// - h(x) heal of x
// - d(x) damage of x

#ifndef _JORG_ITEM_H
#define _JORG_ITEM_H

typedef enum { MISC, WEAPON, ARMOR, CONSUMABLE } item_type_t;
typedef struct
{
  char name[30];
  int value;
  item_type_t type;
  char note[50];

  //used in game var
  int count;
} item_t;

item_t items[100];

#endif /* _JORG_ITEM_H */
