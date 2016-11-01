#include <stdio.h>
#include "creature.h"
#include "class.h"

creature_t creatures[100] =
{
  {
    .name = "Wolf",
    .description = "This powerful canine watches its prey with piercing yellow eyes, darting its tongue across sharp white teeth",
    .stats = { 13, 15, 15, 2, 12, 6 },
    .hp = 13, .ca = 14, .bba = 1,
    .actions = "bite_d(1d6+1)",
    .xp = 400
  }
};

creature_t companions[1] =
{
  {
    .name = "Yann",
    .description = "A brutal warrior",
    .stats = { 16, 10, 14, 8, 8, 10 },
    .hp = 12, .ca = 15, .bba = 1,
    .actions = "Strike_d(1d6)",
    .xp = 0
  }
};
