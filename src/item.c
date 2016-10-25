#include "item.h"
#include "character.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




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
  { .name = "Wood stick", .value = 1, WEAPON, "Damage(1d3)", 0 },
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  { .name = "Heal potion", .value = 150, CONSUMABLE, "Heal(1d8+1)", 0 }
};

void creature_affected_effects(char *effects, char *name, creature_t *instance, creature_t reference)
{
  info_t *action_infos = malloc(sizeof(info_t));
  read_infos(action_infos, effects);
  int total_dmg = -1; //-1 to know if was call
  int total_heal = -1;
  int i = 0;
  for(i = 0; i < action_infos->size; i++)
  {
    char *action = malloc(sizeof(char*));
    // ======= TYPE : DAMAGE =======
    if(strcmp(action_infos->type[i], "Damage") == 0)
    {
      //Hit
      int hit = roll_dice("Contact hit", 1, 20, get_bonus(character->stats.intellect));

      if(total_dmg == -1) //Say it has been used
        total_dmg = 0;

      if(hit >= 10 + get_bonus(instance->stats.dexterity))
      {
        //Damage
        sprintf(action, "Damage with %s", name);
        total_dmg += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
      }
    }
    // ======== TYPE : HEAL ========
    else if(strcmp(action_infos->type[i], "Heal") == 0)
    {
      sprintf(action, "Heal with %s", name);

      if(total_heal == -1) //Say it has been used
        total_heal = 0;

      total_heal += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
    }

    if(total_dmg != -1)
    {
      if(total_dmg > 0)
      {
          printf("%s deals %d damages to %s\n", character->name, total_dmg, instance->name);
          instance->hp -= total_dmg;
      }
      else
        printf("You missed\n");
    }
    if(total_heal > 0)
    {
      int diff = 0;
      instance->hp += total_heal;
       //Hp can't go above maxHp
      if(instance->hp > reference.hp)
      {
        diff = reference.hp - instance->hp;
        instance->hp = reference.hp;
      }

      int healed = total_heal - abs(diff);
      if(healed < 0)
        healed = 0;
      printf("%s heals %s of %d\n", character->name, instance->name, healed);
    }
  }
}

void player_affected_effects(char *effects, char *name)
{
  info_t *action_infos = malloc(sizeof(info_t));
  read_infos(action_infos, effects);
  int total_dmg = 0;
  int total_heal = 0;
  int i = 0;
  for(i = 0; i < action_infos->size; i++)
  {
    char *action = malloc(sizeof(char*));
    // ======= TYPE : DAMAGE =======
    if(strcmp(action_infos->type[i], "Damage") == 0)
    {
      //Damage
      sprintf(action, "Damage caused by %s", name);
      total_dmg += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
    }
    // ======== TYPE : HEAL ========
    else if(strcmp(action_infos->type[i], "Heal") == 0)
    {
      sprintf(action, "Heal with %s", name);

      if(total_heal == -1) //Say it has been used
        total_heal = 0;

      total_heal += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
    }
    int diff = 0;
    character->curr_hp -= total_dmg - total_heal;
    if(total_dmg - total_heal > 0)
      printf("%s deals %d damages to %s\n", name, total_dmg - total_heal, character->name);
    else
    {
      if(character->curr_hp > character->max_hp)
      {
        diff = character->max_hp - character->curr_hp;
        character->curr_hp = character->max_hp;
      }
      int healed = total_heal - abs(diff);
      if(healed < 0)
        healed = 0;
      printf("%s heals %s of %d\n", name, character->name, healed);
    }
  }
}
