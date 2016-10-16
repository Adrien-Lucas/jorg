#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "character.h"
#include "situation.h"
#include "jansson.h"

void get_cmd(void)
{
  printf("\n:");
  char cmd[255];
  fgets(cmd, 255, stdin);
  size_t ln = strlen(cmd) - 1;
  if (cmd[ln] == '\n')
      cmd[ln] = '\0';

  //printf("%s\n",cmd );
  if(strstr(cmd, "say") != NULL)
  {
    say(strdup(cmd));
  }
  else if(strstr(cmd, "status") != NULL)
  {
    status();
  }
  else if(strstr(cmd, "situation") != NULL)
  {
    situation();
  }
  else if(strstr(cmd, "help") != NULL)
  {
    help();
  }

}

void help()
{
  puts("\n\x1b[35m = HELP = \x1b[0mList of all commands :");
  puts("\n   - status - show all infos about the current character");
  puts("\n   - situation - reexplain the current situation");
  puts("\n   - say 'type' - the message should contain keywords (see help talk)");
  puts("\n   - save 'savename' - save the current game");
  puts("\n   - load 'savename' - loads a saved game");
  puts("\n   - exit - exit the game");

  get_cmd();
}

void status()
{
  char className[40];
  get_class_name(className, character->class);
  puts("\n\x1b[35m = STATUS = \x1b[0mAll informations about your character");
  printf("\n   - Name :");
  printf("\n      %s\n", character->name);
  printf("\n   - Class :");
  printf("\n      %s\n", className);
  printf("\n   - Stats :");
  printf("\n      Strength      %d", character->stats.strength);
  printf("\n      Dexterity     %d", character->stats.dexterity);
  printf("\n      Constitution  %d", character->stats.constitution);
  printf("\n      Intellect     %d", character->stats.intellect);
  printf("\n      Wisdow        %d", character->stats.wisdow);
  printf("\n      Charisma      %d\n", character->stats.charisma);
  printf("\n   - Gold :");
  printf("\n      %d\n", character->gold_count);

  get_cmd();
}

void say(char *arg)
{

  if(strstr(arg, "help") != NULL)
  {
    puts("\n\x1b[35m = SAY HELP = \x1b[0mAll informations conversation");
    puts("\n   - say help - show all informations conversation");
    puts("\n   - say hi - start conversation");
    puts("\n   - say goodbye - start conversation");
    puts("\n   - say *phrase with keyword* - answer (do not put '*') \n     eg : *I*'m the programmer of Jorg\n          :say I who are you?\n          I'm Adrien !");
  }
  else if(strstr(arg, "hi") != NULL)
  {
    current_line = &current_situtation->line;
    printf("%s\n", current_line->text);
  }
  else if(current_line != NULL && strstr(arg, "goodbye") != NULL)
  {
    current_line = NULL;
    printf("\nGoodbye %s", character->name);
  }
  else if(current_line != NULL)
  {
    int findkw = 0;
    for (int i = 0; i < 10; i++)
    {
      if(current_line->keywords[i] != NULL && strstr(arg, current_line->keywords[i]) != NULL)
      {
        if(strstr(current_line->next[i]->text, "nxtsit(") != NULL)
        {
          current_situtation = &situations[current_line->next[i]->text[7] - '0'];
          printf("%s\n", current_situtation->description);
        }
        else
        {
          current_line = current_line->next[i];
          printf("%s\n", current_line->text);
        }
        findkw = 1;
        break;
      }
    }
    if(findkw == 0)
      printf("\nI don't understand, can you repeat ?");
  }
  else
    say("help");

  free(arg);
  get_cmd();
}

void situation()
{
  printf("%s\n", current_situtation->description);
  get_cmd();
}

void save()
{

}

void load()
{

}

void exit_game()
{

}
