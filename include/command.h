#include "situation.h"

#ifndef _JORG_COMMAND_H
#define _JORG_COMMAND_H

void get_cmd(void);
void help(char *arg);
void status(char *arg);
void equip(char *arg);
void situation();
void talk(char *arg);
void say(char *arg);
void go(char *arg);
void buy(char *arg);
void sell(char *arg);
void save();
void load();
void exit_cmd(char *arg);

line_t *current_line;

#endif /* _JORG_COMMAND_H */
