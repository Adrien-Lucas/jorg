#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "character.h"
#include "item.h"
#include "jorg.h"

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
  int gold = (4-wealth) * (4-wealth) * 10;
  character->gold_count = gold;
  character->stats = classes[character->class];
  //Fill inventory with empty items
  for(int i = 0; i < 30; i++)
  {
    character->inventory[i] = items[0];
  }
  switch (character->class)
  {
    case 0:
      add_item(1,1);
      add_item(7,1);
      //WARRIOR BASE STUFF
      break;
    case 1:
      add_item(2,1);
      //WIZARD BASE STUFF
      break;
    case 2:
      //ROGUE BASE STUFF
      break;
    case 3:
      //PRIEST BASE STUFF
      break;
  }
}

void add_item(int id, int n)
{
  _Bool has_found = false;
  //Find exisisting object to increment the count
  for(int i = 0; i < 30; i++)
  {
    char *name = character->inventory[i].name;
    if(strcmp(name, items[id].name) == 0)
    {
      character->inventory[i].count += n;
      has_found = true;
      break;
    }
  }
  //else find the first empty cell
  if(has_found == false)
    for(int i = 0; i < 30; i++)
    {
      char *name = character->inventory[i].name;
      if(strcmp(name, "empty") == 0)
      {
        character->inventory[i] = items[id];
        character->inventory[i].count += n;
        has_found = true;
        break;
      }
    }

  if(has_found == false)
    printf("\nYour inventory is full");
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
