#include "item.h"
#include "character.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



item_t items[100] = {
  { "empty" },
  { .name = "Fists", .value = 0, WEAPON, "Damage(1d2)", 0 }, //WEAPON
  { .name = "Broken Sword", .value = 3, WEAPON, "Damage(1d4)", 0 },
  { .name = "Iron Sword", .value = 10, WEAPON, "Damage(1d6)", 0 },
  { .name = "Iron Dagger", .value = 2, WEAPON, "Damage(1d4) Damage(1d4)", 0 },
  { .name = "Wooden Stick", .value = 1, WEAPON, "Damage(1d3)", 0 },
  { .name = "Wooden Mace", .value = 5, WEAPON, "Damage(1d4+1)", 0 },
  { .name = "Heavy Iron Mace", .value = 20, WEAPON, "Damage(1d8+1)", 0 },
  { .name = "Morningstar", .value = 15, WEAPON, "Damage(1d8)", 0 },
  { .name = "Steel sword", .value = 25, WEAPON, "Damage(1d6+2)", 0 },
  { .name = "Scimitar", .value = 75, WEAPON, "Damage(2d4+2)", 0 },
  { .name = "Enchanted Iron Sword", .value = 100, WEAPON, "Damage(1d6+4)", 0 },
  { .name = "Enchanted Iron Dagger", .value = 75, WEAPON, "Damage(1d4+2) Damage(1d4+2)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "", .value = 0, WEAPON, "Damage(xdx+x)", 0 },
  { .name = "Wool Dress", .value = 5, ARMOR, "Protect(1)", 0 },   //ARMOR
  { .name = "Leather Armor", .value = 10, ARMOR, "Protect(2)", 0 },
  { .name = "Rusted Iron Armor", .value = 30, ARMOR, "Protect(3)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "", .value = 0, ARMOR, "Protect(xdx+x)", 0 },
  { .name = "Food Ration", .value = 2, CONSUMABLE, "Heal(1d2)", 0 }, //CONSUMABLE
  { .name = "Water bottle", .value = 1, CONSUMABLE, "Heal(1d2)", 0 },
  { .name = "Medicinal Plant", .value = 30, CONSUMABLE, "Heal(2d4)", 0 },
  { .name = "Minor Heal Potion", .value = 50, CONSUMABLE, "Heal(1d8+2)", 0 },
  { .name = "Average Heal Potion", .value = 150, CONSUMABLE, "Heal(2d8+4)", 0 },
  { .name = "Major Heal Potion", .value = 300, CONSUMABLE, "Heal(3d8+6)", 0 },
  { .name = "Minor Mana Potion", .value = 50, CONSUMABLE, "Mana(1d8+2)", 0 },
  { .name = "Average Mana Potion", .value = 150, CONSUMABLE, "Mana(2d8+4)", 0 },
  { .name = "Major Mana Potion", .value = 300, CONSUMABLE, "Mana(3d8+6)", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "", .value = 0, CONSUMABLE, "", 0 },
  { .name = "A letter for brother Lionnel", .value = 1, BOOK, "", 0, .book_id = 0 }, //BOOK
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "", .value = 0, BOOK, "", 0 },
  { .name = "Short Rope", .value = 1, MISC, "A rope of 15ft, useful for plenty of things", 0 }, //MISC
  { .name = "Cloth", .value = 1, MISC, "Some cloth rags", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 },
  { .name = "", .value = 0, MISC, "", 0 }
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

  free(action_infos);
}

void player_affected_effects(char *effects, char *name)
{
  info_t *action_infos = malloc(sizeof(info_t));
  read_infos(action_infos, effects);
  int total_dmg = 0;
  int total_heal = 0;
  int total_mana = 0;
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
      total_heal += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
    }
    else if(strcmp(action_infos->type[i], "Mana") == 0)
    {
      sprintf(action, "Give mana with %s", name);
      total_mana += roll_dice(action, action_infos->dice_nb[i], action_infos->dice[i], action_infos->bonus[i]);
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
    if(total_mana > 0)
    {
      if(character->curr_mana > character->max_mana)
      {
        diff = character->max_mana - character->curr_mana;
        character->curr_mana = character->max_mana;
      }
      int mana = total_mana - abs(diff);
      if(mana < 0)
        mana = 0;
      printf("%s gives %s %d mana\n", name, character->name, mana);
    }
  }

  free(action_infos);
}
