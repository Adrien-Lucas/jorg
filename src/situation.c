#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "character.h"
#include "situation.h"
#include "command.h"

void line_init_nokey(line_t *ret, char *text)
{
  ret->text = text;
}

void line_init(line_t *ret, char *text, line_t *keyw[10])
{
  //Find keywords
  char words[10][20];
  int n = 0;
  for(int i = 0; i < strlen(text); i++)
  {
    if(text[i] == '*')
    {
      char word[20];
      int a = 0;
      i++;
      while (text[i] != '*' && a < 21)
      {
        word[a] = text[i];
        i++;
        a++;
      }
      i++;
      strcpy(words[n], word);
      n++;
    }
  }
  //create keywords
  for(int i = 0; i < n; i++)
  {
    if(strlen(words[i]) > 0)
    {
      ret->keywords[i] = words[i];
    }
  }

  memcpy(ret->next, keyw, sizeof(*keyw));
  ret->text = text;
}

void change_situation(int index, _Bool getcmd)
{
  last_situation = current_situtation;
  current_situtation = &situations[index];
  printf("%s\n", current_situtation->description);

  if(current_situtation->type == MERCHANT)
  {
    show_shop();
  }

  if(getcmd)
    get_cmd();
}

void show_shop()
{
  puts("\n\x1b[35m = SHOP = \x1b[0m");
  printf("\n\x1b[36m   N° |              NAME              | VALUE |             INFOS               \x1b[0m\n");
  //printf("\n\x1b[36m===============================================\x1b[0m\n");
  for(int i = 0; i < current_situtation->market_size; i++)
  {
    char n[2];
    sprintf(n, "%d", i);

    char name[30];
    strcpy(name, items[current_situtation->market[i]].name);

    char value[5];
    sprintf(value, "%d", items[current_situtation->market[i]].value);

    char infos[255];
    if(items[current_situtation->market[i]].type == WEAPON)
    {
      sprintf(infos, " Damages : %d", items[current_situtation->market[i]].power);
    }

    printf("\n - %-3s| %-31s| %-6s|%-4s ", n, name, value, infos);
  }
  printf("\n\nCurrent gold : %d\n\nUse 'buy' or 'sell' to interact, see your inventory with 'status inventory'\n", character->gold_count);
}

//ALL LINES
line_t l3_2 = {"The dark forest is full of spiders"};
line_t l3_1 = {"nxtsit(3)"};

//ALL SITUATIONS
situation_t situations[100] = {
  { //ANARION PLACE - 0
    EXPLORE,
    "You are in the middle of the village of Anarion, the place is full of life and the weather is pretty good\nAt your right is the tavern\nIn front of you siege the castle of the Anarion's lord",
    .explore_names = {"tavern", "castle"},
    .explore_index = {1, 5}
  },
  { //ANARION TAVERN - 1
    ROOM,
    "You are in the tavern, there is a good mood here, the taverner is at your left behind the bar. You can see some sort of mercenaries in the back of the room",
    .talk_names = {"taverner", "mercenaries"},
    .talk_index = {2, 3}
  },
  { //ANARION_TAVERN_TAVERNER - 2
    TALK,
    "The taverner is an old man but he seems pretty strong for his age, he gives you a big smile when you approach",
    .line = {"Hi stranger ! Are you here to *drink* or get *infos* ?", {"drink", "infos"}, {&l3_1, &l3_2}}
  },
  { //ANARION_TAVERN_TAVERNER_SHOP - 3
    MERCHANT,
    "The taverner tells you what you can buy here",
    .market_size = 3,
    .market = {0,1,2}
  },
  { //ANARION_TAVERN_MERCENARIES - 4
    TALK,
    ""
  },
  { //ANARION CASTLE - 5
    ROOM,
    "You get to the castle gate, two guards block your way"
  }
};
