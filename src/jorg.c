#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jorg.h"
#include "character.h"
#include "command.h"

void game_start(void)
{
  print_welcome_text();
  character_create();

  char className[40];
  get_class_name(className,character->class);
  printf("\nWelcome to the world of Jorg %s the %s !", character->name, className);
  puts("\nTo play you have to get commands, you can get any help at any moment by typing 'help'");
  puts("\n\nAnd here start our story ...");

  current_situtation = &situations[0];
  situation();
}

void print_welcome_text(void)
{
	puts("\x1b[35mWelcome to Jorg !\x1b[0m");
	puts("\n\x1b[31mJorg is a text based RPG\x1b[0m");
	puts("\n\x1b[31mPlay your character and explore epic dungeons\x1b[0m");
	puts("\n\x1b[31mBut take care to not awake the dark forces ...\x1b[0m");
}

void wait_key(void)
{
  puts("\nPress Enter to continue ...");
  getchar();
}

void ask(char answer[], int size, const char *question)
{
  printf("\n\n\x1b[35m%s\x1b[0m\n", question);
  fgets(answer, size, stdin);
  size_t ln = strlen(answer) - 1;
  if (answer[ln] == '\n')
      answer[ln] = '\0';
}

int do_choice(char *question, char *choices[], int size)
{
  printf("\n\n\x1b[35m%s\x1b[0m\n", question);

  for(int i = 0; i < size; i++)
  {
    printf("\n%d . %s", i+1, choices[i]);
  }
  printf("\n:");
  char schoice[255];
  fgets(schoice, 255, stdin);
  if(schoice[0] == '\0' || schoice[0] == '\n')
  {
    printf("\n$You didn't enter anything");
    return do_choice(question, choices, size);
  }

  int choice = atoi(schoice);

  if(choice > size || choice < 0)
  {
    printf("\n$Invalid number : %d", choice);
    return do_choice(question, choices, size);
  }

  return choice - 1;
}

void read_infos(info_t *infos, char *source)
{
  char *notes[10];
  //fill with empty string (to count after assignation)
  for(int i = 0; i < 10; i++)
  {
    notes[i] = "empty";
  }
  char *src = malloc(sizeof(char*));
  strcpy(src, source);
  //printf("src : %s\n", src);
  strsplit(notes, src, " ");
  //GET SIZE
  int size;
  for(size = 0; strstr(notes[size], "empty") == NULL; size++)
  {
    //Work on each note
    char *underscore_separation[2];
    strsplit(underscore_separation, notes[size], "_");
    int effect_part = 0;
    //GET NAME
    if(strstr(underscore_separation[0], "(") == NULL)
    {
        infos->name[size] = underscore_separation[0];
        effect_part = 1;
    }
    //GET TYPE
    char *bracket_separation[2];
    strsplit(bracket_separation, underscore_separation[effect_part], "(");
    infos->type[size] = bracket_separation[0];
    //GET VARS
    char *vars = malloc(sizeof(char*));
    strcpy(vars, bracket_separation[1]);
    strrmv(vars, ")");
    //Is constant ?
    if(strstr(vars, "d") != NULL)
    {
      // - GET DICE Nb
      char *d_separation[2];
      strsplit(d_separation, vars, "d");
      infos->dice_nb[size] = atoi(d_separation[0]);
      // - GET DICE TYPE AND BONUS
      if(strstr(vars, "+") != NULL)
      {
        char *plus_separation[2];
        strsplit(plus_separation, d_separation[1], "+");
        infos->dice[size] = atoi(plus_separation[0]);
        infos->bonus[size] = atoi(plus_separation[1]);
      }
      else if(strstr(vars, "-") != NULL)
      {
        char *minus_separation[2];
        strsplit(minus_separation, d_separation[1], "+");
        infos->dice[size] = atoi(minus_separation[0]);
        infos->bonus[size] = atoi(minus_separation[1]);
      }
      else //No bonus
      {
        infos->dice[size] = atoi(d_separation[1]);
      }
    }
    else
      infos->bonus[size] = atoi(vars);

    //printf("Name : %s, Type : %s, DiceNb : %d, Dice : %d, Bonus : %d\n", infos->name[size], infos->type[size], infos->dice_nb[size], infos->dice[size], infos->bonus[size]);
  }
  infos->size = size;
  //printf("Size : %d\n", infos->size);
}

//Read str like : nxtsit(750) and return 750.
int read_function(char *str, char *fct)
{
  char* reader = malloc(sizeof(char*));
  char* fct_name = malloc(sizeof(char*));
  strcpy(fct_name, fct);
  strcat(fct_name, "(");
  strcpy(reader, strrmvbfr(str, fct_name));
  reader = strrmv(reader, fct_name);

  char *spliting[10];
  strsplit(spliting, reader, ")");
  return atoi(spliting[0]);
}

int roll_dice(char *name, int nb, int type, int bonus)
{
  printf("\n%s : Roll %dd%d (", name, nb, type);
  int result = 0;
  for(int i = 0; i < nb; i++)
  {
    if(i>0)
      printf("+");

    int randnb = rand() % (type) + 1;
    printf("%d", randnb);
    result += randnb;
  }
  printf(") ");
  if(bonus > 0)
    printf("+ %d ", bonus);
  else if(bonus < 0)
    printf("%d ", bonus);

  result += bonus;
  printf("= %d\n", result);
  return result;
}

int rand_lim(int limit)
{
/* return a random number between 0 and limit inclusive.
 */
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}
void strsplit(char*words[50], char *string, char *separator)
{
  char* token;

  if (string != NULL)
  {
    int i = 0;

    while ((token = mystrsep(&string, separator)) != NULL)
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

char* strrmvbfr(char str[255], char word[30])
{
  int a = 0;
  int b = 0;
  for(int i = 0; i < strlen(str); i++)
  {
    if(str[i] == word[0])
    {
      for(a = 0; a < strlen(word) && str[i+a] == word[a]; a++);
      if(a == strlen(word))
      {
        char newstr[(int)strlen(str) - (i-1)];
        for(b = 0; b < (int)strlen(str); b++)
        {
          newstr[b] = str[i+b];
        }
        char *ret = malloc(sizeof(char*));
        strcpy(ret, newstr);
        return ret;
      }
    }
  }
  return "error using strrmvbfr";
}

char* mystrsep(char** stringp, const char* delim)
{
  char* start = *stringp;
  char* p;

  p = (start != NULL) ? strpbrk(start, delim) : NULL;

  if (p == NULL)
  {
    *stringp = NULL;
  }
  else
  {
    *p = '\0';
    *stringp = p + 1;
  }

  return start;
}
