#include <stdio.h>
#include "creature.h"
#include "class.h"

creature_t creatures[100] =
{
  {
    .name = "Wolf",
    .description = "A dangerous predator",
    .stats = { 14, 12, 12, 3, 5, 4 },
    .level = 1, .hp = 7, .ca = 14, .bba = 0,
    .action_number = 2, .actions = "claw_d(1d4);bite_d(1d6)"
  }
};

creature_t companions[10] =
{
  {
    .name = "Yann",
    .description = "A brutal warrior",
    .stats = { 16, 10, 14, 8, 8, 10 },
    .level = 1, .hp = 12, .ca = 15, .bba = 1,
    .action_number = 1, .actions = "Strike_d(1d6)"
  }
};
