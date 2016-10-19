#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "jorg.h"
#include "character.h"
#include "situation.h"

void get_cmd(void)
{
  printf("\n:");
  char cmd[255];
  fgets(cmd, 255, stdin);
  size_t ln = strlen(cmd) - 1;
  if (cmd[ln] == '\n')
      cmd[ln] = '\0';

  if(strstr(cmd, "help") != NULL)
  {
    help(cmd);
  }
  else if(strstr(cmd, "talk") != NULL)
  {
    talk(cmd);
  }
  else if(strstr(cmd, "say") != NULL)
  {
    say(cmd);
  }
  else if(strstr(cmd, "go") != NULL)
  {
    go(cmd);
  }
  else if(strstr(cmd, "status") != NULL)
  {
    status(cmd);
  }
  else if(strstr(cmd, "situation") != NULL)
  {
    situation();
  }
  else if(strstr(cmd, "buy") != NULL)
  {
    buy(cmd);
  }
  else if(strstr(cmd, "sell") != NULL)
  {
    sell(cmd);
  }
  else if(strstr(cmd, "exit") != NULL)
  {
    exit_game();
  }
  else
  {
    puts("Unknow command, need some help ? type 'help'");
    get_cmd();
  }
}

void help(char *arg)
{
  if(strstr(arg, "status") != NULL)
  {
    puts("\n\x1b[35m = STATUS HELP = \x1b[0mAll informations about your character status");
    puts("\n   - status - show global informations about your character");
    puts("\n   - status inventory - show your current inventory");
  }
  else if(strstr(arg, "situation") != NULL)
  {
    puts("\n\x1b[35m = SITUATION HELP = \x1b[0mAll informations about the situation command");
    puts("\n   - situation - reexplain the current situation");
  }
  else if(strstr(arg, "talk") != NULL)
  {
    puts("\n\x1b[35m = TALK HELP = \x1b[0mAll informations about the talking");
    puts("\n   - talk 'name' - engage conversation with a character");
    puts("\n   (see 'help say')");
  }
  else if(strstr(arg, "say") != NULL)
  {
    puts("\n\x1b[35m = SAY HELP = \x1b[0mAll informations about conversation");
    puts("\n   - say goodbye - start conversation");
    puts("\n   - say *phrase with keyword* - answer (do not put '*') \n     eg : *I*'m the programmer of Jorg\n          :say I who are you?\n          I'm Adrien !");
    puts("\n   (see 'help talk')");
  }
  else if(strstr(arg, "go") != NULL)
  {
    puts("\n\x1b[35m = GO HELP = \x1b[0mAll informations about travels");
    puts("\n   - go 'place' - go to the indicate place");
    puts("\n   - go back - get back to the last place");
  }
  else if(strstr(arg, "save") != NULL)
  {
    puts("\n\x1b[35m = SAVE HELP = \x1b[0mAll informations about travels");
    puts("\n   Save files are located in *gamedir*/saves");
    puts("\n   - save 'savename' - save the current game");
    puts("\n   (see 'help load')");
  }
  else if(strstr(arg, "load") != NULL)
  {
    puts("\n\x1b[35m = LOAD HELP = \x1b[0mAll informations about travels");
    puts("\n   Save files are located in *gamedir*/saves");
    puts("\n   - load 'savename' - loads a saved game");
    puts("\n   (see 'help save')");
  }
  else if(strstr(arg, "exit") != NULL)
  {
    puts("\n\x1b[35m = EXIT HELP = \x1b[0mAll informations about quiting the game");
    puts("\n   - exit - ask you a confirmation and exit the game");
    puts("\n   (see 'help save' and 'help load')");
  }
  else
  {
    puts("\n\x1b[35m = HELP = \x1b[0mList of all commands :");
    puts("\n   - help 'command name' - show all infos about a command, e.g : 'help say'");
    puts("\n   - status - show all infos about the current character");
    puts("\n   - situation - reexplain the current situation");
    puts("\n   - talk 'name' - engage conversation with a character");
    puts("\n   - say 'keyword' - the message should contain keyword");
    puts("\n   - go 'place' - the message should contain place name");
    puts("\n   - save 'savename' - save the current game");
    puts("\n   - load 'savename' - loads a saved game");
    puts("\n   - exit - exit the game");
  }

  get_cmd();
}

void status(char *arg)
{
  if(strstr(arg, "inventory") != NULL)
  {
    puts("\n\x1b[35m = INVENTORY = \x1b[0mYour current inventory");
    printf("\n\x1b[36m   N° |              NAME              | VALUE | COUNT |             INFOS               \x1b[0m\n");
    for(int i = 0; i < 30; i++)
    {
      char *name = character->inventory[i].name;
      if(strcmp(name, "empty") != 0)
      {
        char value[5];
        sprintf(value, "%d", character->inventory[i].value);

        /*char count[5];
        sprintf(value, "%d", character->inventory[i].count);*/

        char infos[255];
        if(character->inventory[i].type == WEAPON)
        {
          sprintf(infos, " Damages : %d", character->inventory[i].power);
        }

        printf("\n - %-3d| %-31s| %-6d| %-6d| %-4s", i, name, character->inventory[i].value, character->inventory[i].count, infos);
      }
    }
  }
  else
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
  }

  get_cmd();
}

void talk(char *arg)
{
  for (int i = 0; i < 10; i++)
  {
    if(current_situtation->talk_names[i] != NULL && strstr(arg, current_situtation->talk_names[i]) != NULL)
    {
      change_situation(current_situtation->talk_index[i], false);
      say("hi");
      return;
    }
  }
  printf("\nThere is no one with this name, try 'talk myself' if you feel alone");

  get_cmd();
}

void say(char *arg)
{
  if(current_situtation->type == TALK)
  {
    if(strstr(arg, "hi") != NULL)
    {
      current_line = &current_situtation->line;
      printf("%s\n", current_line->text);
    }
    else if(current_line != NULL && strstr(arg, "goodbye") != NULL)
    {
      current_line = NULL;
      printf("\nGoodbye\n");
      go("back");
      return;
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
            change_situation(current_line->next[i]->text[7] - '0', true);
            return;
          }
          else
          {
            printf("%s\n", current_line->next[i]->text);
            if(current_line->next[i]->keywords[0] != NULL)
              current_line = current_line->next[i];
          }
          findkw = 1;
          break;
        }
      }
      if(findkw == 0)
        printf("\nI don't understand, can you repeat ?");
    }
    else
    {
        say("help");
        return;
    }
  }
  else
    printf("\nYou are not talking with someone, maybe try 'talk 'name''");

  get_cmd();
}

void go(char *arg)
{
  if(strstr(arg, "back") != NULL)
  {
    current_situtation = last_situation;
    printf("%s\n", current_situtation->description);
  }
  else
  {
    int findkw = 0;
    for (int i = 0; i < 10; i++)
    {
      if(current_situtation->explore_names[i] != NULL && strstr(arg, current_situtation->explore_names[i]) != NULL)
      {
        change_situation(current_situtation->explore_index[i], false);
        findkw = 1;
        break;
      }
    }
    if(findkw == 0)
      printf("\nThere is no such place, are you lost ?");
  }

  get_cmd();
}

void situation()
{
  printf("%s\n", current_situtation->description);
  if(current_situtation->type == MERCHANT)
  {
    show_shop();
  }

  get_cmd();
}

void buy(char *arg)
{
  if(current_situtation->type == MERCHANT)
  {
    char *only_arg = malloc(sizeof(char *));
    strcpy(only_arg, arg);
    strrmv(only_arg, "buy ");

    char *args[2];
    strsplit(args, only_arg, " ");
    int id = atoi(args[0]);
    int n = atoi(args[1]);
    if(n==0)
      n = 1;

    if(character->gold_count >= items[current_situtation->market[id]].value * n)
    {
      char question[255];
      sprintf(question, "Are you sure you want to buy x%d %s for %d ?", n, items[current_situtation->market[id]].name, items[current_situtation->market[id]].value * n);
      char yes[255];
      sprintf(yes ,"Yes I'm sure to buy a %s", items[current_situtation->market[id]].name);
      char *choices[2] = {yes,"Never mind"};
      int confirmation = do_choice(question, choices, 2);
      if(confirmation == 0)
      {
          character->gold_count -= items[current_situtation->market[id]].value * n;
          add_item(current_situtation->market[id], n);
          printf("\n%s Added to your inventory ! (see 'status inventory')\n\n", items[current_situtation->market[id]].name);
      }
    }
    else
      printf("You don't have enough money to buy x%d %s\n", n, items[current_situtation->market[id]].name);

    situation();
    return;
  }
  else
  {
    printf("Your not in a shop");
  }

  get_cmd();
}

void sell(char *arg)
{

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

void strsplit(char*words[50], char *string, char *separator)
{
  char* token;

  if (string != NULL)
  {
    int i = 0;

    while ((token = strsep(&string, separator)) != NULL)
    {
      words[i] = token;
      i++;
    }
  }
}

char* strrmv(char *text, char *removeword){
    char *p=text;
    int rlen;
    rlen = strlen(removeword);
    while(NULL!=(p=strstr(p, removeword))){
        memmove(p, p+rlen, strlen(p+rlen)+1);
    }
    return text;
}
