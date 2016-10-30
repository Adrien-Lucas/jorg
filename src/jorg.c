#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <dirent.h>
#include "jorg.h"
#include "character.h"
#include "command.h"
#include "save.h"


#if (__STDC_VERSION__ >= 199901L)
#include <stdint.h>
#endif

void game_start(void)
{
  print_welcome_text();

  char *choices[2] = { "Create a new character", "Load a game" };
  int choice = do_choice(" = START = ", choices, 2);
  if(choice == 0)
  {
    character_create();

    char className[40];
    get_class_name(className,character->class);
    printf("\nWelcome to the world of Jorg %s the %s !", character->name, className);
    puts("\nTo play you have to get commands, you can get any help at any moment by typing 'help'");
    puts("\n\nAnd here start our story ...");

    current_situtation = &situations[0];
    situation();
  }
  else
  {
    _Bool valid = false;
    while(!valid)
    {
      char *file_name = malloc(sizeof(char*));
      ask(file_name, 50, "Save name");

      if(strlen(file_name) > 1)
      {
        if(strstr(file_name, ".save") == NULL )
          strcat(file_name, ".save");

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir ("./save/")) != NULL)
        {
          while ((ent = readdir (dir)) != NULL)
          {
            if(strcmp(file_name, ent->d_name) == 0)
            {
              valid = true;
              load(file_name);
            }
          }
          closedir (dir);
        }
        else
        {
          /* could not open directory */
          perror ("");
        }
      }
      if(!valid)
      {
        printf("Invalid name\n");
      }
    }
  }
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
  printf("\n\n%s\n", strcolor(question, 35));

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
  char *src = strdup(source);
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
    infos->name[size] = "";
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
    char *vars = strdup(bracket_separation[1]);
    strrmv(vars, ")");
    //Is constant ?
    if(strstr(vars, "d") != NULL)
    {
      // - GET DICE Nb
      char *d_separation[2];
      strsplit(d_separation, vars, "d");
      infos->dice_nb[size] = atoi(d_separation[0]);
      // - GET DICE TYPE AND BONUS
      if(strstr(d_separation[1], "+") != NULL)
      {
        char *plus_separation[2];
        strsplit(plus_separation, d_separation[1], "+");
        infos->dice[size] = atoi(plus_separation[0]);
        infos->bonus[size] = atoi(plus_separation[1]);
      }
      else if(strstr(d_separation[1], "-") != NULL)
      {
        char *minus_separation[2];
        strsplit(minus_separation, d_separation[1], "-");
        infos->dice[size] = atoi(minus_separation[0]);
        infos->bonus[size] = atoi(minus_separation[1]);
      }
      else //No bonus
      {
        infos->dice[size] = atoi(d_separation[1]);
        infos->bonus[size] = 0;
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
  char* fct_name = strdup(fct);
  strcpy(fct_name, fct);
  strcat(fct_name, "(");
  char* reader = strdup(strrmvbfr(str, fct_name));
  reader = strrmv(reader, fct_name);

  char *spliting[10];
  strsplit(spliting, reader, ")");
  return atoi(spliting[0]);
}

char *read_function_str(char *str, char *fct)
{
  char* fct_name = strdup(fct);
  strcpy(fct_name, fct);
  strcat(fct_name, "(");
  char* reader = strdup(strrmvbfr(str, fct_name));
  reader = strrmv(reader, fct_name);

  char *spliting[10];
  strsplit(spliting, reader, ")");
  return spliting[0];
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

char* strrmv(char *text, char *removeword)
{
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
        return strdup(newstr);
      }
    }
  }
  return "$error using strrmvbfr";
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

char *strcolor(const char *str, const int color)
{
  #ifdef _WIN32
    switch (color)
    {
      case 0:
        break;
      case 31:
        break;
      case 32:
        break;
      case 33:
        break;
      case 34:
        break;
      case 35:
        break;
      case 36:
        break;
      case 37:
        break;
    }
  #else
    char* tmp = malloc(strlen(str) + 15);
    sprintf(tmp, "\x1b[%dm%s\x1b[0m", color, str);
    return tmp;
  #endif
}

// You must free the result if result is non-NULL.
char *repl_str(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig && !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
