#ifndef _JORG_SITUATION_H
#define _JORG_SITUATION_H

typedef struct _line_t
{
  char *text;
  char *keywords[10];
  struct _line_t *next[10];
} line_t;


typedef enum { TALK, FIGHT, MERCHANT, ROOM } situation_type_t;
typedef struct
{
  situation_type_t type;
  char *description;

  //TALK
  line_t line;

} situation_t;

situation_t *current_situtation;
situation_t situations[2];

void line_init(line_t *ret, char *text, line_t *keyw[10]);
void line_init_nokey(line_t *ret, char *text);

#endif /* _JORG_SITUATION_H */
