#include "item.h"

item_t items[100] = {
  { "empty" },
  { .name = "Iron Sword", .value = 10, WEAPON, 5, "n()", 0 },              //1
  { .name = "Steel Sword", .value = 20, WEAPON, 10, "n()", 0 },            //2
  { .name = "Magic steel Sword", .value = 50, WEAPON, 10, "p(5)", 0 },     //3
  { .name = "Beer", .value = 1, CONSUMABLE, 5, "h(5)", 0 },                //4
  { .name = "Wine", .value = 2, CONSUMABLE, 5, "h(7)", 0 },                //5
  { .name = "Huran's elixir", .value = 10, CONSUMABLE, 5, "h(15)", 0 },    //6
  { .name = "Leather armor", .value = 50, ARMOR, 5, "n()", 0 },            //7
  { .name = "Wool dress", .value = 10, ARMOR, 1, "n()", 0 }                //8
};
