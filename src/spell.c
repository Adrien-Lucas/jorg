#include "spell.h"

spell_t spells[2] = {
  { .name = "Hurt", .cost = 5, .effects = "Damage(1d6)", .shop_value = 150},
  { .name = "Heal", .cost = 5, .effects = "Heal(1d6)", .shop_value = 150}
};
