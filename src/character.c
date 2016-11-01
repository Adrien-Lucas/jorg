#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "character.h"
#include "creature.h"
#include "command.h"
#include "item.h"
#include "jorg.h"
#include "save.h"

void character_create()
{
  character = malloc(sizeof(character_t));
  char *class_choice[4] = { "Warrior", "Wizard", "Rogue", "Priest" };
  char *wealths[4] = { "A noble family", "A merchant family", "A peasant family", "What family ?" };

  printf("\nYou first have to create your character !");
  ask(character->name, 40, "What is your name ?");
	character->class = do_choice("What are you ?", class_choice, 4);
  character->level = 1;

  int wealth = do_choice("Where did you come from ?", wealths, 4);
  printf("\n");
  int gold = (4-wealth) * (4-wealth) * 20;
  character->gold_count = gold;
  character->stats = classes[character->class];
  //Fill inventory with empty items
  for(int i = 0; i < 31; i++)
  {
    character->inventory[i] = items[0];
  }

  character->has_companion = false;
  character->level = 1;
  character->experience = 0;
  character->spell_count = 0;

  switch (character->class)
  {
    case 0:
      add_item(1,1);
      add_item(7,1);
      character->max_mana = 5;
      character->life_dice = 10;
      character->bba = 1;
      //WARRIOR BASE STUFF
      break;
    case 1:
      add_item(9,1);
      add_item(8,1);
      add_spell(0);
      add_spell(1);
      character->max_mana = 25;
      character->life_dice = 6;
      character->bba = 0;
      //WIZARD BASE STUFF
      break;
    case 2:
      //ROGUE BASE STUFF
      break;
    case 3:
      //PRIEST BASE STUFF
      break;
  }

  character->eqquiped_weap = character->inventory[0];
  character->eqquiped_armor = character->inventory[1];

  character->max_hp = character->life_dice + get_bonus(character->stats.constitution);
  character->curr_hp = character->max_hp;
  character->curr_mana = character->max_mana;
  info_t *armor = malloc(sizeof(info_t));
  read_infos(armor, character->eqquiped_armor.note);
  character->ca = 10 + armor->bonus[0] + get_bonus(character->stats.dexterity);
  free(armor);
  check_progression();
}

void add_item(int id, int n)
{
  _Bool has_found = false;
  //Find exisisting object to increment the count
  for(int i = 0; i < 31; i++)
  {
    char *name = character->inventory[i].name;
    if(strcmp(name, items[id].name) == 0)
    {
      character->inventory[i].count += n;
      has_found = true;
      printf("%d %s Added to your inventory ! (see 'status inventory')\n", n, name);
      return;
    }
  }
  //else find the first empty cell
  if(has_found == false)
    for(int i = 0; i < 31; i++)
    {
      char *name = character->inventory[i].name;
      if(strcmp(name, "empty") == 0)
      {
        character->inventory[i] = items[id];
        character->inventory[i].count += n;
        has_found = true;
        printf("%d %s Added to your inventory ! (see 'status inventory')\n", n, character->inventory[i].name);
        return;
      }
    }

  if(has_found == false)
    printf("\nYour inventory is full");
}

void rmv_item(int index, int n)
{
  char *name = strdup(character->inventory[index].name);
  character->inventory[index].count -= n;
  if(character->inventory[index].count <= 0)
  {
    character->inventory[index] = items[0];
    reorganize_inventory();
  }
  printf("\n%d %s Removed from your inventory (see 'status inventory')\n\n", n, name);
}

void add_spell(int id)
{
  //See if already have spell
  for(int i = 0; i < character->spell_count; i++)
  {
    if(character->spell_list[i].name == spells[id].name)
    {
      printf("You already have the spell '%s'\n", spells[id].name);
      return;
    }
  }
  character->spell_list[character->spell_count] = spells[id];
  character->spell_count++;
  printf("The spell '%s' has been added to your spell list !\n", spells[id].name);
}

void reorganize_inventory()
{
  for(int i = 0; i < 29; i++)
  {
    char *name = strdup(character->inventory[i].name);
    if(strcmp(name, "empty") == 0)
    {
      character->inventory[i] = character->inventory[i+1];
      character->inventory[i+1] = items[0];
    }
  }
}

int needed_xp(int level)
{
  //Calculate the next level xp requirement (Medium prgression of D&D)
  int a = round(level / 2);
  int b = (level - 1) / 2;
  int m = 2000; //XP for level 2
  int n = 5000 - m; //5000 is XP of level 3
  int c = m*(pow(2, a) - 1) + n*(pow(2, b) - 1); //aproximative value of XP needed for the current level
  //Now for some rounding (for levels 11+)
  int d = sqrt(level - 1);
  double e = (pow(10, d)) / 0.2f;
  int f = round(c / e);

  int ret = f * e + 1;
  return ret;
}

void check_progression()
{
  if(character->experience >= needed_xp(character->level+1))
  {
    printf("%s\n", strcolor("You have won a level ! Type 'levelup' to upgrade your character", 35));
  }
}

void levelup()
{
  character->level++;

  printf("\n%s\n", strcolor("          = LEVEL UP = ", 35));
  printf("\n  %s is upgrading to level %d\n", character->name, character->level);
  printf("\n  HP +%d\n", (int)(character->life_dice/2 + get_bonus(character->stats.constitution)));
  printf("  MANA +%d\n", (int)(character->max_mana/4));
  printf("  BBA +%d\n", (character->life_dice < 8 && (int)(character->life_dice/2) ==  character->life_dice/2) || character->life_dice >= 8 ? 1 : 0);

  if((int)(character->level / 4) == (double)character->level / 4)
  {
    char *choices[6] = {"Strenght", "Dexterity", "Constitution", "Intellect", "Wisdow", "Charisma"};
    int choice = do_choice("You won a new skill point, what attribute do you want to updgrade ?", choices, 6);

    switch (choice) {
      case 0:
        character->stats.strength ++;
        break;
      case 1:
        character->stats.dexterity ++;
        break;
      case 2:
        character->stats.constitution ++;
        break;
      case 3:
        character->stats.intellect ++;
        break;
      case 4:
        character->stats.wisdow ++;
        break;
      case 5:
        character->stats.charisma ++;
        break;
    }
  }

  character->curr_hp += (int)(character->life_dice/2 + get_bonus(character->stats.constitution));
  character->max_hp += (int)(character->life_dice/2 + get_bonus(character->stats.constitution));
  character->curr_mana += (int)(character->max_mana/4);
  character->max_mana += (int)(character->max_mana/4);
  character->bba += (character->life_dice < 8 && (int)(character->life_dice/2) ==  character->life_dice/2) || character->life_dice >= 8 ? 1 : 0;

  printf("\n  Next level in %d XP\n", needed_xp(character->level+1) - character->experience);
  getchar();
  if(character->experience >= needed_xp(character->level+1))
  {
    levelup();
    return;
  }

  get_cmd();
}

void death()
{
  char *choices[2] = {"Load a game", "Exit jorg"};
  int choice = do_choice("You are dead, what are your last words ?", choices, 2);

  if(choice == 0)
  {

  }
  else
  {
    exit(0);
  }
}

void character_free(character_t *ptr)
{
  free(ptr);
}

void get_class_name(char answer[], class_t class)
{
  switch (class)
  {
    case 0: strcpy(answer, "Warrior"); break;
    case 1: strcpy(answer, "Wizard"); break;
    case 2: strcpy(answer, "Rogue"); break;
    case 3: strcpy(answer, "Priest"); break;
  }
}
