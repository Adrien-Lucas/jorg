#ifndef _JORG_JORG_H
#define _JORG_JORG_H

typedef struct
{
  int size;
  char *name[10];
  char *type[10];
  int dice_nb[10];
  int dice[10];
  int bonus[10];
} info_t;

void game_start(void);
void print_welcome_text(void);
void wait_key(void);
void ask(char answer[], int size, const char *question);
int do_choice(char *question, char *choices[], int size);
int roll_dice(char *name, int nb, int type, int bonus);

void read_infos(info_t *infos, char *source);
int read_function(char *str, char *fct);
int rand_lim(int limit);
char* strrmv(char *text, char *removeword);
void strsplit(char*words[50], char *string, char *separator);
char* strrmvbfr(char str[255], char word[30]);
char* mystrsep(char** stringp, const char* delim);

#endif /* _JORG_JORG_H */
