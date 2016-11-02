#include <stdbool.h>
#include "item.h"

#ifndef _JORG_SITUATION_H
#define _JORG_SITUATION_H

typedef struct _line_t
{
  char *text;
  char *keywords[10];
  struct _line_t *next[10];
} line_t;

typedef enum { TALK, FIGHT, MERCHANT, ROOM, EXPLORE } situation_type_t;
typedef struct
{
  situation_type_t type;
  char *description;
  int last_situation;

  //TALK
  char *talk_names[10];
  int talk_index[10];
  line_t line;
  char *recruitable_companion;

  //FIGHT
  char *attack_description;
  int enemies_index[10];
  situation_type_t tobe;

  //MERCHANT
  int market_size;
  int market[100];

  //ROOM
  char *interact_names[10];
  char *interact_descriptions[10];
  char *interacts[10];

  //EXPLORE
  char *explore_names[10];
  int explore_index[10];

} situation_t;

typedef struct
{
  int gold;
  int items[30];
  int count[30];
} container_t;

container_t containers[1];

situation_t *current_situtation;
situation_t situations[10];

int curr_sit;

void line_init(line_t *ret, char *text, line_t *keyw[10]);
void line_init_nokey(line_t *ret, char *text);
void change_situation(int index);
void change_situation_quiet(int index);
void show_shop();
void open_container(int index);
void show_fight();
void color_keywords(char* str, char *kw[10], int color);

#endif /* _JORG_SITUATION_H */
