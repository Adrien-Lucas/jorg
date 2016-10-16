#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "situation.h"

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

//ALL LINES
line_t l0_3 = {"nxtsit(1)"};
line_t l0_2 = {"Ne posez pas de questions et tout ira bien pour vous"};
line_t l0_1 = {"Notre bon roi Maxime V"};

//ALL SITUATIONS
situation_t situations[2] = {
  {
    TALK,
    "Vous travaillez lorsque deux gardes viennent à votre rencontre",
    {"Au nom du *roi* *suivez* nous ! (other : *ok*)", {"roi", "suivez", "ok"}, {&l0_1, &l0_2, &l0_3}}
  },
  {
    TALK,
    "Les gardes vous mennent en geole, vous voyez un viel homme qui pourris dans votre cellule",
    {"Tu veux quoi ? (other : *qui etes vous*)", {"qui etes vous"}, {&l0_1, &l0_2}}
  }
};
