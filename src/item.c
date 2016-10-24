#include "item.h"

item_t items[100] = {
  { "empty" },
  { .name = "Iron Sword", .value = 10, WEAPON, "Damage(1d6)", 0 },                   //1
  { .name = "Steel Sword", .value = 20, WEAPON, "Damage(1d8)", 0 },                  //2
  { .name = "Magic steel Sword", .value = 50, WEAPON, "Damage(1d8+2)", 0 },          //3
  { .name = "Beer", .value = 1, CONSUMABLE, "Heal(1d4)", 0 },                      //4
  { .name = "Wine", .value = 2, CONSUMABLE, "Heal(1d4+1)", 0 },                    //5
  { .name = "Huran's elixir", .value = 10, CONSUMABLE, "Heal(1d8+1)", 0 },         //6
  { .name = "Leather armor", .value = 50, ARMOR, "Protect(4)", 0 },               //7
  { .name = "Wool dress", .value = 10, ARMOR, "Protect(1)", 0 },                   //8
  { .name = "Wood stick", .value = 1, WEAPON, "Damage(1d3)", 0 }
};
