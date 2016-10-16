#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "character.h"
#include "jorg.h"

character_t *character_create()
{
  character_t *ptr = malloc(sizeof(character_t));
  char *class_choice[4] = { "Warrior", "Wizard", "Rogue", "Priest" };
  char *wealths[4] = { "A noble family", "A merchant family", "A peasant family", "What family ?" };

  printf("\nYou first have to create your character !");
  ask(ptr->name, 40, "What is your name ?");
	ptr->class = do_choice("What are you ?", class_choice, 4);
  ptr->level = 1;

  int wealth = do_choice("Where did you come from ?", wealths, 4);
  ptr->gold_count = (4-wealth) * (4-wealth) * 10;
  ptr->stats = classes[ptr->class];

  switch (ptr->class)
  {
    case 0:
      //WARRIOR BASE STUFF
      break;
    case 1:
      //WIZARD BASE STUFF
      break;
    case 2:
      //ROGUE BASE STUFF
      break;
    case 3:
      //PRIEST BASE STUFF
      break;
  }

  return ptr;
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
