#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "character.h"
#include "situation.h"
#include "command.h"

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
    if(strcmp(character->eqquiped_armor.name, character->inventory[i].name) == 0 || strcmp(character->eqquiped_weap.name, character->inventory[i].name) == 0 )
      fprintf(fp, "%s*:%d\r\n", character->inventory[i].name, character->inventory[i].count);
    else
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
  for(int i = 0; i < nb; i++)
  {
    fprintf(fp, "<situation(%d)>\r\n", i);
    fprintf(fp, "%d\r\n", situations[i].type);
    fprintf(fp, "%d\r\n", situations[i].last_situation);
  }

  //Save containers
  nb = (int)( sizeof(containers) / sizeof(containers[0]));
  for(int i = 0; i < nb && sizeof(containers[i]) > 0; i++)
  {
    int n = 0;
    for(n = 0; containers[i].count[n] != 0; n++);
    fprintf(fp, "<container(%d)> size(%d) g(%d)\r\n", i, n, containers[i].gold);
    for(int a = 0; a < n; a++)
    {
      fprintf(fp, "%d:%d\r\n", containers[i].items[a], containers[i].count[a]);
    }
  }
  fprintf(fp, "<situation/curr>\r\n");
  fprintf(fp, "%d\r\n", curr_sit);
  fclose(fp);

  printf("%s (Use 'load *save name* to load a saved game')\n", strcolor("Game successfully saved !", 35));
}

void load(char *file_name)
{
  printf("\nLoading %s\n", file_name);
  if(character == NULL)
    character = malloc(sizeof(character_t));

  FILE *fp;
  char path[255];
  sprintf(path, "./save/%s", file_name);
  fp = fopen(path, "r");

  char buffer[255];
  printf("Progress 0%c\n", '%');
  while (fgets(buffer, sizeof buffer, fp) != NULL)
  {
    if(strstr(buffer, "<character>") != NULL)
    {
      char *charname = strdup(fgets(buffer, sizeof buffer, fp));
      strrmv(charname, "\r\n");
      strcpy(character->name, charname);
      character->level = atoi(fgets(buffer, sizeof buffer, fp));
      character->experience = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.strength = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.dexterity = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.constitution = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.intellect = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.wisdow = atoi(fgets(buffer, sizeof buffer, fp));
      character->stats.charisma = atoi(fgets(buffer, sizeof buffer, fp));
      character->bba = atoi(fgets(buffer, sizeof buffer, fp));
      character->ca = atoi(fgets(buffer, sizeof buffer, fp));
      character->curr_hp = atoi(fgets(buffer, sizeof buffer, fp));
      character->max_hp = atoi(fgets(buffer, sizeof buffer, fp));
      character->curr_mana = atoi(fgets(buffer, sizeof buffer, fp));
      character->max_mana = atoi(fgets(buffer, sizeof buffer, fp));
      character->gold_count = atoi(fgets(buffer, sizeof buffer, fp));
      printf("Progress 25%c\n", '%');
    }

    if(strstr(buffer, "<character/inv>") != NULL)
    {
      for(int i = 0; i < 30; i++)
      {
        char *sep[2];
        strsplit(sep, fgets(buffer, sizeof buffer, fp), ":");

        _Bool equipped = false;
        if(strstr(sep[0], "*") != NULL)
        {
          equipped = true;
          strrmv(sep[0], "*");
        }
        for(int a = 0; a < (int)( sizeof(items) / sizeof(items[0])); a++)
        {
          if(strcmp(items[a].name, sep[0]) == 0 )
          {
            character->inventory[i] = items[a];
            if(equipped)
            {
              if(character->inventory[i].type == ARMOR)
                character->eqquiped_armor = character->inventory[i];
              else
                character->eqquiped_weap = character->inventory[i];
            }
          }
        }
        character->inventory[i].count = atoi(sep[1]);
      }
      printf("Progress 50%c\n", '%');
    }
    if(strstr(buffer, "<character/spells(") != NULL)
    {
      const int nb = read_function(buffer, "spells");
      character->spell_count = nb;
      for(int i = 0; i < nb; i++)
      {
        char *name = strdup(fgets(buffer, sizeof buffer, fp));
        strrmv(name, "\r\n");
        for(int a = 0; a < (int)( sizeof(spells) / sizeof(spells[0])); a++)
        {
          if(strcmp(spells[a].name, name) == 0 )
            character->spell_list[i] = spells[a];
        }
      }
      printf("Progress 75%c\n", '%');
    }
    if(strstr(buffer, "<character/companion>") != NULL)
    {
      character->has_companion = true;
      char *comp = strdup(fgets(buffer, sizeof buffer, fp));
      strrmv(comp, "\r\n");
      for(int a = 0; a < (int)( sizeof(companions) / sizeof(companions[0])); a++)
      {
        if(strcmp(companions[a].name, comp) == 0 )
          character->companion = companions[a];
      }
    }
    if(strstr(buffer, "<situation(") != NULL)
    {
      int index = read_function(buffer, "situation");
      situations[index].type = atoi(fgets(buffer, sizeof buffer, fp));
      situations[index].last_situation = atoi(fgets(buffer, sizeof buffer, fp));
    }
    if(strstr(buffer, "<container(") != NULL)
    {
      int index = read_function(buffer, "container");
      int size = read_function(buffer, "size");
      int g = read_function(buffer, "g");
      containers[index].gold = g;
      for(int i = 0; i < 30; i++)
      {
        if(i < size)
        {
          char *sep[2];
          strsplit(sep, fgets(buffer, sizeof buffer, fp), ":");
          containers[index].items[i] = atoi(sep[0]);
          containers[index].count[i] = atoi(sep[1]);
        }
        else
        {
          containers[index].items[i] = 0;
          containers[index].count[i] = 0;
        }
      }
    }
    if(strstr(buffer, "<situation/curr>") != NULL)
    {
      curr_sit = atoi(fgets(buffer, sizeof buffer, fp));
      current_situtation = &situations[curr_sit];
    }
  }
  printf("Progress 100%c\n", '%');
  fclose(fp);
  printf("%s\n", strcolor("Save successfully loaded !", 35));
  situation();
}
