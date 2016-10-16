#ifndef _JORG_JORG_H
#define _JORG_JORG_H

void game_start(void);
void print_welcome_text(void);
void wait_key(void);
void ask(char answer[], int size, const char *question);
int do_choice(const char *question, char *choices[], int size);

#endif /* _JORG_JORG_H */
