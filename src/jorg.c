#include <stdio.h>
#include <string.h>
#include "jorg.h"
#include "character.h"
#include "command.h"

void game_start(void)
{
  print_welcome_text();
  character = character_create();

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

int do_choice(const char *question, char *choices[], int size)
{
  printf("\n\n\x1b[35m%s\x1b[0m\n", question);

  for(int i = 0; i < size; i++)
  {
    printf("\n%d . %s", i+1, choices[i]);
  }
  printf("\n:");
  int choice = getchar() - '0';
  getchar();//ate enter
  if(choice > size)
  {
    printf("\n$Invalid number : %d", choice);
    return do_choice(question, choices, size);
  }

  return choice - 1;
}
