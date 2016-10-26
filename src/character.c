#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
  character->level = 0;
  character->experience = 0;
  character->spell_count = 0;

  switch (character->class)
  {
    case 0:
      add_item(1,1);
      add_item(7,1);
      character->max_hp = 10 + get_bonus(character->stats.constitution);
      character->max_mana = 5;
      character->bba = 1;
      //WARRIOR BASE STUFF
      break;
    case 1:
      add_item(9,1);
      add_item(8,1);
      character->max_hp = 6 + get_bonus(character->stats.constitution);
      character->max_mana = 25;
      character->bba = 0;
      add_spell(0);
      add_spell(1);
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

  character->curr_hp = character->max_hp;
  character->curr_mana = character->max_mana;
  info_t *armor = malloc(sizeof(info_t));
  read_infos(armor, character->eqquiped_armor.note);
  character->ca = 10 + armor->bonus[0] + get_bonus(character->stats.dexterity);
  free(armor);
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
