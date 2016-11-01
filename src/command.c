#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "command.h"
#include "save.h"
#include "jorg.h"
#include "character.h"
#include "situation.h"

void get_cmd(void)
{
  printf("\n:");
  char cmd[255];
  fgets(cmd, 255, stdin);
  size_t ln = strlen(cmd) - 1;
  if (cmd[ln] == '\n')
      cmd[ln] = '\0';

  if(strstr(cmd, "help") != NULL)
  {
    help(cmd);
  }
  else if(strstr(cmd, "talk") != NULL)
  {
    talk(cmd);
  }
  else if(strstr(cmd, "say") != NULL)
  {
    say(cmd);
  }
  else if(strstr(cmd, "see") != NULL)
  {
    see(cmd);
  }
  else if(strstr(cmd, "interact") != NULL)
  {
    interact(cmd);
  }
  else if(strstr(cmd, "go") != NULL)
  {
    go(cmd);
  }
  else if(strstr(cmd, "status") != NULL)
  {
    status(cmd);
  }
  else if(strstr(cmd, "equip") != NULL)
  {
    equip(cmd);
  }
  else if(strstr(cmd, "use") != NULL)
  {
    use(cmd);
  }
  else if(strstr(cmd, "situation") != NULL)
  {
    situation();
  }
  else if(strstr(cmd, "levelup") != NULL)
  {
    if(character->experience >= needed_xp(character->level+1))
      levelup();
    else
      printf("You don't have enough XP to get a new level\n");
  }
  else if(strstr(cmd, "save") != NULL)
  {
    char *file_name = strdup(cmd);
    strrmv(file_name, "save ");
    save(file_name);
    get_cmd();
  }
  else if(strstr(cmd, "load") != NULL)
  {
    char *file_name = strdup(cmd);
    strrmv(file_name, "load ");
    strcat(file_name, ".save");
    load(file_name);
    get_cmd();
  }
  else if(strstr(cmd, "buy") != NULL)
  {
    buy(cmd);
  }
  else if(strstr(cmd, "sell") != NULL)
  {
    sell(cmd);
  }
  else if(strstr(cmd, "exit") != NULL)
  {
    exit_cmd(cmd);
  }
  else
  {
    puts("Unknow command, need some help ? type 'help'");
    get_cmd();
  }
}

void help(char *arg)
{
  if(strstr(arg, "status") != NULL)
  {
    puts("\n\x1b[35m = STATUS HELP = \x1b[0mAll informations about your character status");
    puts("\n   - status - show global informations about your character");
    puts("\n   - status inventory - show your current inventory");
  }
  else if(strstr(arg, "situation") != NULL)
  {
    puts("\n\x1b[35m = SITUATION HELP = \x1b[0mAll informations about the situation command");
    puts("\n   - situation - reexplain the current situation");
  }
  else if(strstr(arg, "talk") != NULL)
  {
    puts("\n\x1b[35m = TALK HELP = \x1b[0mAll informations about the talking");
    puts("\n   - talk 'name' - engage conversation with a character");
    puts("\n   (see 'help say')");
  }
  else if(strstr(arg, "say") != NULL)
  {
    puts("\n\x1b[35m = SAY HELP = \x1b[0mAll informations about conversation");
    puts("\n   - say goodbye - start conversation");
    puts("\n   - say *phrase with keyword* - answer (do not put '*') \n     eg : *I*'m the programmer of Jorg\n          :say I who are you?\n          I'm Adrien !");
    puts("\n   (see 'help talk')");
  }
  else if(strstr(arg, "go") != NULL)
  {
    puts("\n\x1b[35m = GO HELP = \x1b[0mAll informations about travels");
    puts("\n   - go 'place' - go to the indicate place");
    puts("\n   - go back - get back to the last place");
  }
  else if(strstr(arg, "save") != NULL)
  {
    puts("\n\x1b[35m = SAVE HELP = \x1b[0mAll informations about travels");
    puts("\n   Save files are located in *gamedir*/saves");
    puts("\n   - save 'savename' - save the current game");
    puts("\n   (see 'help load')");
  }
  else if(strstr(arg, "load") != NULL)
  {
    puts("\n\x1b[35m = LOAD HELP = \x1b[0mAll informations about travels");
    puts("\n   Save files are located in *gamedir*/saves");
    puts("\n   - load 'savename' - loads a saved game");
    puts("\n   (see 'help save')");
  }
  else if(strstr(arg, "exit") != NULL)
  {
    puts("\n\x1b[35m = EXIT HELP = \x1b[0mAll informations about quiting the game");
    puts("\n   - exit - ask you a confirmation and exit the game");
    puts("\n   - exit shop - exit the actual shop");
    puts("\n   (see 'help save' and 'help load')");
  }
  else
  {
    puts("\n\x1b[35m = HELP = \x1b[0mList of all commands :");
    puts("\n   - help 'command name' - show all infos about a command, e.g : 'help say'");
    puts("\n   - status (inventory) - show all infos about the current character, 'status inventory' shows your inventory");
    puts("\n   - equip 'index' - equip the selected item in wepon or armor slot, 'index' is the item index's in your inventory");
    puts("\n   - situation - reexplain the current situation");
    puts("\n   - talk 'name' - engage conversation with a character");
    puts("\n   - say 'keyword' - the message should contain keyword");
    puts("\n   - buy 'index' 'quantity' - use this to buy items in shops, 'index' is the item's index in the shop table");
    puts("\n   - sell 'index' 'quantity' - use this to sell items in shops, 'index' is the item's index in your inventory");
    puts("\n   - go 'place' - the message should contain place name");
    puts("\n   - save 'savename' - save the current game");
    puts("\n   - load 'savename' - loads a saved game");
    puts("\n   - exit - exit the game");
  }

  get_cmd();
}

void status(char *arg)
{
  if(strstr(arg, "inventory") != NULL)
  {
    puts("\n\x1b[35m = INVENTORY = \x1b[0mYour current inventory");
    printf("\n\x1b[36m   N° |              NAME              | VALUE | COUNT |             INFOS               \x1b[0m\n");

    for(int i = 0; i < 30; i++)
    {
      if(strcmp(character->inventory[i].name, "empty") != 0)
      {
        _Bool equipped = false;
        if(strcmp(character->inventory[i].name, "Equipped") != 0 && (strstr(character->eqquiped_weap.name, character->inventory[i].name) != NULL || strstr(character->eqquiped_armor.name, character->inventory[i].name) != NULL))
          equipped = true;

        if(equipped)
          printf("\n - %-3d| (Equipped) %-20s| %-6d| %-6d| %-4s", i, character->inventory[i].name, character->inventory[i].value, character->inventory[i].count, character->inventory[i].note);
        else
          printf("\n - %-3d| %-31s| %-6d| %-6d| %-4s", i, character->inventory[i].name, character->inventory[i].value, character->inventory[i].count, character->inventory[i].note);
      }
    }
  }
  else if(strstr(arg, "spell") != NULL)
  {
    puts("\n\x1b[35m = SPELL LIST = \x1b[0mYour current spells");
    printf("\n\x1b[36m   N° |              NAME              | COST | VALUE |             EFFECTS               \x1b[0m\n");
    for(int i = 0; i < character->spell_count; i++)
    {
      printf("\n - %-3d| %-31s| %-5d| %-6d| %-4s", i, character->spell_list[i].name, character->spell_list[i].cost, character->spell_list[i].shop_value, character->spell_list[i].effects);
    }
  }
  else if(strstr(arg, "companion") != NULL)
  {
    if(character->has_companion)
    {
      puts("\n\x1b[35m = COMPANION STATUS = \x1b[0mYour companion's status");
      printf("\n   - \x1b[33mName :\x1b[0m");
      printf("\n      %s\n", character->companion.name);
      printf("\n   - \x1b[33mStats :\x1b[0m");
      printf("\n      Strength      %-2d %s%d)", character->companion.stats.strength, get_bonus(character->companion.stats.strength)>0?"(+":"(", get_bonus(character->companion.stats.strength));
      printf("\n      Dexterity     %-2d %s%d)", character->companion.stats.dexterity, get_bonus(character->companion.stats.dexterity)>0?"(+":"(", get_bonus(character->companion.stats.dexterity));
      printf("\n      Constitution  %-2d %s%d)", character->companion.stats.constitution, get_bonus(character->companion.stats.constitution)>0?"(+":"(", get_bonus(character->companion.stats.constitution));
      printf("\n      Intellect     %-2d %s%d)", character->companion.stats.intellect, get_bonus(character->companion.stats.intellect)>0?"(+":"(", get_bonus(character->companion.stats.intellect));
      printf("\n      Wisdow        %-2d %s%d)", character->companion.stats.wisdow, get_bonus(character->companion.stats.wisdow)>0?"(+":"(", get_bonus(character->companion.stats.wisdow));
      printf("\n      Charisma      %-2d %s%d)\n", character->companion.stats.charisma, get_bonus(character->companion.stats.charisma)>0?"(+":"(", get_bonus(character->companion.stats.charisma));
      printf("\n   - \x1b[33mArmor class :\x1b[0m");
      printf("\n      %d\n", character->companion.ca);
      printf("\n   - \x1b[33mAttacks :\x1b[0m");

      info_t action;
      read_infos(&action, character->companion.actions);
      for(int i = 0; i < action.size; i++)
      {
        char bonus[7];
        if(action.bonus[i] > 0)
          sprintf(bonus, "+%d", action.bonus[i]);
        else if(action.bonus[i] < 0)
          sprintf(bonus, "%d", action.bonus[i]);
        else
          strcpy(bonus, "");

        printf("\n      %s (%dd%d%s)\n", action.name[i], action.dice_nb[i], action.dice[i], bonus);
      }
    }
    else
      printf("You don't have any companion\n");
  }
  else
  {
    char className[40];
    get_class_name(className, character->class);
    puts("\n\x1b[35m = STATUS = \x1b[0mAll informations about your character");
    printf("\n   \x1b[31m%d/%d HP\n   \x1b[32m%d/%d MANA\x1b[0m\n", character->curr_hp, character->max_hp, character->curr_mana, character->max_mana);
    printf("\n   - \x1b[33mName :\x1b[0m");
    printf("\n      %s\n", character->name);
    printf("\n   - \x1b[33mClass :\x1b[0m");
    printf("\n      %s\n", className);
    printf("\n   - \x1b[33mProgression :\x1b[0m");
    printf("\n      Level %d", character->level);
    printf("\n      %d / %d XP\n", character->experience, needed_xp(character->level+1));
    printf("\n   - \x1b[33mStats :\x1b[0m");
    printf("\n      Strength      %-2d %s%d)", character->stats.strength, get_bonus(character->stats.strength)>0?"(+":"(", get_bonus(character->stats.strength));
    printf("\n      Dexterity     %-2d %s%d)", character->stats.dexterity, get_bonus(character->stats.dexterity)>0?"(+":"(", get_bonus(character->stats.dexterity));
    printf("\n      Constitution  %-2d %s%d)", character->stats.constitution, get_bonus(character->stats.constitution)>0?"(+":"(", get_bonus(character->stats.constitution));
    printf("\n      Intellect     %-2d %s%d)", character->stats.intellect, get_bonus(character->stats.intellect)>0?"(+":"(", get_bonus(character->stats.intellect));
    printf("\n      Wisdow        %-2d %s%d)", character->stats.wisdow, get_bonus(character->stats.wisdow)>0?"(+":"(", get_bonus(character->stats.wisdow));
    printf("\n      Charisma      %-2d %s%d)\n", character->stats.charisma, get_bonus(character->stats.charisma)>0?"(+":"(", get_bonus(character->stats.charisma));
    printf("\n   - \x1b[33mArmor class :\x1b[0m");
    printf("\n      %d\n", character->ca);
    printf("\n   - \x1b[33mGold :\x1b[0m");
    printf("\n      %d\n", character->gold_count);
    if(character->has_companion)
    {
      printf("\n   - \x1b[33mCompanion :\x1b[0m   (see 'status companion')");
      printf("\n      %s\n", character->companion.name);
    }
    printf("\nUse 'status inventory' to see your inventory and 'status spell' to see your spell list\n");
  }

  get_cmd();
}

void equip(char *arg)
{
  char *only_arg = strdup(arg);
  strrmv(only_arg, "equip ");

  int id = atoi(only_arg);
  char *name = character->inventory[id].name;
  if(id >= 0 && strcmp(name, "empty") != 0 && (character->inventory[id].type == ARMOR || character->inventory[id].type == WEAPON))
  {
    if(character->inventory[id].type == WEAPON)
      character->eqquiped_weap = character->inventory[id];
    else if(character->inventory[id].type == ARMOR)
      character->eqquiped_armor = character->inventory[id];

    printf("%s Equipped successfully !\n", name);
  }
  else
    printf("You cannot equip this item (see 'help equip')\n");

  get_cmd();
}

void use(char *arg)
{
  char *only_arg = strdup(arg);
  strrmv(only_arg, "use ");

  int id = atoi(only_arg);
  char *name = character->inventory[id].name;
  if(character->inventory[id].type == CONSUMABLE)
  {
    player_affected_effects(character->inventory[id].note, character->inventory[id].name);
    rmv_item(id, 1);
    if(character->curr_hp <= 0)
    {
      getchar();
      printf("%s\n", strcolor("You killed yourself, really ?", 31));
      getchar();
      death();
      return;
    }
  }
  else
  {
    printf("%s is not a consumable\n", name);
  }
  get_cmd();
}

void talk(char *arg)
{
  for (int i = 0; i < 10; i++)
  {
    if(current_situtation->talk_names[i] != NULL && strstr(arg, current_situtation->talk_names[i]) != NULL)
    {
      change_situation(current_situtation->talk_index[i]);
      return;
    }
  }
  printf("\nThere is no one with this name, try 'talk myself' if you feel alone");

  get_cmd();
}

void say(char *arg)
{
  if(current_situtation->type == TALK)
  {
    if(strstr(arg, "hi") != NULL)
    {
      if(character->has_companion && strcmp(character->companion.name, current_situtation->recruitable_companion) == 0)
      {
        printf("You already have %s\n", character->companion.name);
        say("goodbye");
      }
      else
      {
        current_line = &current_situtation->line;
        char *colored = strdup(current_line->text);
        color_keywords(colored, current_line->keywords, 33);
        printf("%s\n", colored);
      }
    }
    else if(current_line != NULL && strstr(arg, "goodbye") != NULL)
    {
      current_line = NULL;
      printf("\nGoodbye\n");
      change_situation_t(last_situation);
      return;
    }
    else if(current_line != NULL)
    {
      int findkw = 0;
      for (int i = 0; i < 10; i++)
      {
        if(current_line->keywords[i] != NULL && strstr(arg, current_line->keywords[i]) != NULL)
        {
          _Bool is_command = false;
          int price = 0;
          if(strstr(current_line->next[i]->text, "pay(") != NULL)
          {
            price = read_function(current_line->next[i]->text, "pay");
            if(price <= character->gold_count)
              character->gold_count -= price;
            else
            {
              printf("You don't have enough money, come back when you'll have %dgp\n", price);
              change_situation_t( last_situation );
              return;
            }
            is_command = true;
          }
          if(strstr(current_line->next[i]->text, "companion(") != NULL)
          {
            int comp = read_function(current_line->next[i]->text, "companion");
            if(character->has_companion)
            {
              char *question = malloc(sizeof(char*));
              sprintf(question, "Are you sure to replace %s with %s ?", character->companion.name, companions[comp].name);
              char *choices[2] = {"Yes", "No"};
              int choice = do_choice(question, choices, 2);
              if(choice == 0)
              {
                character->companion = companions[comp];
                printf("%s is now your companion (see 'status companion')\n", character->companion.name);
              }
              else
                character->gold_count += price;
            }
            else
            {
              character->companion = companions[comp];
              character->has_companion = true;
              printf("%s is now your companion (see 'status companion')\n", character->companion.name);
            }
            is_command = true;
          }
          if(strstr(current_line->next[i]->text, "additem(") != NULL)
          {
            int item = read_function(current_line->next[i]->text, "additem");
            add_item(item, 1);
            is_command = true;
          }
          if(strstr(current_line->next[i]->text, "addspell(") != NULL)
          {
            int spell = read_function(current_line->next[i]->text, "addspell");
            add_spell(spell);
            is_command = true;
          }
          if(strstr(current_line->next[i]->text, "print(") != NULL) //don't work
          {
            printf("%s\n", read_function_str(current_line->next[i]->text, "print"));
            is_command = true;
          }
          if(strstr(current_line->next[i]->text, "nxtsit(") != NULL)
          {
            int sit = read_function(current_line->next[i]->text, "nxtsit");
            change_situation( sit );
            return;
          }
          if(!is_command)
          {
            char *colored = strdup(current_line->next[i]->text);
            color_keywords(colored, current_line->next[i]->keywords, 33);
            printf("%s\n", colored);
            if(current_line->next[i]->keywords[0] != NULL)
              current_line = current_line->next[i];
          }
          findkw = 1;
          break;
        }
      }
      if(findkw == 0)
        printf("\nI don't understand, can you repeat ?");
    }
    else
    {
        say("help");
        return;
    }
  }
  else
    printf("\nYou are not talking with someone, maybe try 'talk 'name''");

  get_cmd();
}

void see(char *arg)
{
  _Bool find = false;
  for(int i = 0; i < 10 && current_situtation->interact_names[i] != NULL; i++)
  {
    if(strstr(arg, current_situtation->interact_names[i]) != NULL)
    {
      printf("%s\n", current_situtation->interact_descriptions[i]);
      find = true;
    }
  }
  if(!find)
  {
    printf("There is no interactables object with this name (see 'help see')\n");
  }

  get_cmd();
}

void interact(char *arg)
{
  _Bool find = false;
  for(int i = 0; i < 10 && current_situtation->interact_names[i] != NULL; i++)
  {
    if(strstr(arg, current_situtation->interact_names[i]) != NULL)
    {
      if(strstr(current_situtation->interacts[i], "additem(") != NULL)
      {
        int item = read_function(current_situtation->interacts[i], "additem");
        add_item(item, 1);
      }
      if(strstr(current_situtation->interacts[i], "addspell(") != NULL)
      {
        int spell = read_function(current_situtation->interacts[i], "addspell");
        add_spell(spell);
      }
      if(strstr(current_situtation->interacts[i], "container(") != NULL)
      {
        int container = read_function(current_situtation->interacts[i], "container");
        open_container(container);
      }
      if(strstr(current_situtation->interacts[i], "nxtsit(") != NULL)
      {
        int sit = read_function(current_situtation->interacts[i], "nxtsit");
        change_situation( sit );
        return;
      }
      find = true;
    }
  }
  if(!find)
  {
    printf("There is no interactables object with this name (see 'help see')\n");
  }

  get_cmd();
}

void go(char *arg)
{
  if(strstr(arg, "back") != NULL)
  {
    /*current_situtation = last_place;
    printf("%s\n", current_situtation->description);*/
    change_situation_t(last_place);
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      if(current_situtation->explore_names[i] != NULL && strstr(arg, current_situtation->explore_names[i]) != NULL)
      {
        change_situation( current_situtation->explore_index[i] );
        return;
      }
    }
    printf("\nThere is no such place, are you lost ?");
  }

  get_cmd();
}

void situation()
{
  if(current_situtation->type != FIGHT)
  {
    char *str = strdup(current_situtation->description);
    color_keywords(str, current_situtation->explore_names, 31);
    color_keywords(str, current_situtation->talk_names, 32);
    color_keywords(str, current_situtation->interact_names, 35);
    printf("%s\n", str);

    if(current_situtation->type == MERCHANT)
      show_shop();

    if(current_situtation->type == TALK)
      say("hi");
  }
  else
  {
    show_fight();
    return;
  }

  get_cmd();
}

void buy(char *arg)
{
  if(current_situtation->type == MERCHANT)
  {
    char *only_arg = strdup(arg);
    strrmv(only_arg, "buy ");

    char *args[2];
    strsplit(args, only_arg, " ");
    int id = atoi(args[0]);
    int n = 0;
    if(strlen(args[1])>1)
      n = atoi(args[1]);
    if(n==0)
      n = 1;
    if(id < current_situtation->market_size)
    {
      if(character->gold_count >= items[current_situtation->market[id]].value * n)
      {
        char question[255];
        sprintf(question, "Are you sure you want to buy %d %s for %dgp ?", n, items[current_situtation->market[id]].name, items[current_situtation->market[id]].value * n);
        char yes[255];
        sprintf(yes ,"Yes I'm sure to buy %d %s", n, items[current_situtation->market[id]].name);
        char *choices[2] = {yes,"Never mind"};
        int confirmation = do_choice(question, choices, 2);
        if(confirmation == 0)
        {
            character->gold_count -= items[current_situtation->market[id]].value * n;
            add_item(current_situtation->market[id], n);
            situation();
            return;
        }
      }
      else
        printf("You don't have enough money to buy %d %s\n", n, items[current_situtation->market[id]].name);
    }
    else
        printf("I don't sell this object, indicate the number of the desired object on the shop list please\n");

    return;
  }
  else
  {
    printf("Your not in a shop\n");
  }

  get_cmd();
}

void sell(char *arg)
{
  if(current_situtation->type == MERCHANT)
  {
    char *only_arg = strdup(arg);
    strrmv(only_arg, "sell ");

    char *args[2];
    strsplit(args, only_arg, " ");
    int id = atoi(args[0]);
    int n = atoi(args[1]);
    if(n==0)
      n = 1;

    char *name = character->inventory[id].name;
    if(strcmp(name, "empty") != 0)
    {
      if(n <= character->inventory[id].count)
      {
        char question[255];
        sprintf(question, "I'll give you %dgp for your %d %s, did you accept ?", (character->inventory[id].value * n)/2, n, character->inventory[id].name);
        char yes[255];
        sprintf(yes ,"Yes I want to sell %d %s", n, character->inventory[id].name);
        char *choices[2] = {yes,"Never mind"};
        int confirmation = do_choice(question, choices, 2);
        if(confirmation == 0)
        {
            character->gold_count += (character->inventory[id].value * n) / 2;
            rmv_item(id, n);
        }
      }
      else
      {
        printf("You don't have %d %s\n", n, character->inventory[id].name);
      }
    }
    else
    {
      printf("There is no object at line %d in your inventory\n", id);
    }
  }
  else
  {
    printf("Your not in a shop\n");
  }

  get_cmd();
}

void exit_cmd(char *arg)
{
  if(strstr(arg, "shop") != NULL)
  {
    if(current_situtation->type == MERCHANT)
    {
      change_situation_t(last_situation);
      return;
    }
    else
    {
      printf("Your not in a shop\n");
    }
  }
  else
  {
    char *choices[2] = {"Exit Jorg", "Never mind"};
    int choice = do_choice("Are you sure you want to exit Jorg ? All unsaved progress would be lost", choices, 2);
    if(choice == 0)
    {
      character_free(character);
      exit(0);
      //Exit the game
    }
  }
  get_cmd();
}
