#include "situation.h"

#ifndef _JORG_COMMAND_H
#define _JORG_COMMAND_H

void get_cmd(void);
void help();
void status();
void situation();
void say(char *arg);
void save();
void load();
void exit_game();

line_t *current_line;

#endif /* _JORG_COMMAND_H */
