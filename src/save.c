#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "character.h"
#include "situation.h"

void save(char *file_name)
{
  FILE *fp;
  char path[255];
  sprintf(path, "./save/%s.save", file_name);
  fp = fopen(path, "w");

  //Save character
  fprintf(fp, "<character>\r\n");
  fprintf(fp, "%s\r\n", character->name);
  fprintf(fp, "%d\r\n", character->level);
  fprintf(fp, "%d\r\n", character->experience);
  fprintf(fp, "%d\r\n", character->stats.strength);
  fprintf(fp, "%d\r\n", character->stats.constitution);
  fprintf(fp, "%d\r\n", character->stats.dexterity);
  fprintf(fp, "%d\r\n", character->stats.intellect);
  fprintf(fp, "%d\r\n", character->stats.wisdow);
  fprintf(fp, "%d\r\n", character->stats.charisma);
  fprintf(fp, "%d\r\n", character->bba);
  fprintf(fp, "%d\r\n", character->ca);
  fprintf(fp, "%d\r\n", character->curr_hp);
  fprintf(fp, "%d\r\n", character->max_hp);
  fprintf(fp, "%d\r\n", character->curr_mana);
  fprintf(fp, "%d\r\n", character->max_mana);
  fprintf(fp, "%d\r\n", character->gold_count);

  fprintf(fp, "<character/inv>\r\n");
  for(int i = 0; i < 30; i++)
  {
    fprintf(fp, "%s:%d\r\n", character->inventory[i].name, character->inventory[i].count);
  }
  fprintf(fp, "<character/spells(%d)>\r\n", character->spell_count);
  for(int i = 0; i < character->spell_count; i++)
  {
    fprintf(fp, "%s\r\n", character->spell_list[i].name);
  }
  if(character->has_companion)
  {
    fprintf(fp, "<character/companion>\r\n");
    fprintf(fp, "%s\r\n", character->companion.name);
  }
  //Save situations
    int nb = sizeof(situations) / sizeof(situation_t);
  fprintf(fp, "<situations>\r\n");

  for(int i = 0; i < nb; i++)
  {
    if(situations[i].tobe == EXPLORE || situations[i].tobe == ROOM)
    {
      fprintf(fp, "<situation(%d)>\r\n", i);
      fprintf(fp, "%d\r\n", situations[i].type);
    }
  }

  nb = (int)( sizeof(containers) / sizeof(containers[0]));
  fprintf(fp, "<containers(%d)>\r\n", nb);
  //Save containers

  for(int i = 0; i < nb; i++)
  {
    if(containers[i].gold > 0)
    {
      int n = 0;
      for(n = 0; containers[i].count[n] != 0; n++);
      fprintf(fp, "<container(%d)> size(%d) g(%d)\r\n", i, n, containers[i].gold);
      for(int a = 0; a < n; a++)
      {
        fprintf(fp, "%d:%d\r\n", containers[i].items[a], containers[i].count[a]);
      }
    }
  }
  fprintf(fp, "<situation/curr>\r\n");
  fprintf(fp, "%d\r\n", curr_sit);
  fprintf(fp, "<situation/last_place>\r\n");
  fprintf(fp, "%d\r\n", last_pl);
  fprintf(fp, "<situation/last_situation>\r\n");
  fprintf(fp, "%d\r\n", last_sit);
  fclose(fp);
  load(file_name);
}

void load(char *file_name)
{
  FILE *fp;
  char buff[255];

  char path[255];
  sprintf(path, "./save/%s.save", file_name);
  fp = fopen(path, "r");

  fscanf(fp, "%s", buff);

  fclose(fp);
}
