//Note can contain the following effects
// - n() No note
// - h(x) heal of x
// - d(x) damage of x
// - p(x) power + x

#ifndef _JORG_ITEM_H
#define _JORG_ITEM_H

typedef enum { MISC, WEAPON, ARMOR, CONSUMABLE } item_type_t;
typedef struct
{
  char name[50];
  int value;
  item_type_t type;
  int power;
  char note[10];
} item_t;

item_t items[3];

#endif /* _JORG_ITEM_H */
