#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "jorg.h"
#include "character.h"
#include "creature.h"
#include "situation.h"
#include "command.h"
#include "class.h"

#ifdef _WIN32
#define  SELECT_ADD 1
#else
#define  SELECT_ADD 0
#endif

void line_init_nokey(line_t *ret, char *text)
{
  ret->text = text;
}

void line_init(line_t *ret, char *text, line_t *keyw[10])
{
  //Find keywords
  char words[10][20];
  int n = 0;
  for(int i = 0; i < strlen(text); i++)
  {
    if(text[i] == '*')
    {
      char word[20];
      int a = 0;
      i++;
      while (text[i] != '*' && a < 21)
      {
        word[a] = text[i];
        i++;
        a++;
      }
      i++;
      strcpy(words[n], word);
      n++;
    }
  }
  //create keywords
  for(int i = 0; i < n; i++)
  {
    if(strlen(words[i]) > 0)
    {
      ret->keywords[i] = words[i];
    }
  }

  memcpy(ret->next, keyw, sizeof(*keyw));
  ret->text = text;
}

void change_situation(int index)
{
  if((current_situtation->type == ROOM || current_situtation->type == EXPLORE) && (situations[index].type == ROOM || situations[index].type == EXPLORE))
  {
    last_place = current_situtation;
    last_pl = curr_sit;
  }

  if(current_situtation->type != MERCHANT && current_situtation->type != TALK)
  {
    last_situation = current_situtation;
    last_sit = curr_sit;
  }

  current_situtation = &situations[index];
  curr_sit = index;
  situation();
}

void change_situation_t(situation_t *sit)
{
  int index = 0;
  if(sit == last_place)
    index = last_pl;
  if(sit == last_situation)
    index = last_sit;

  if((current_situtation->type == ROOM || current_situtation->type == EXPLORE) && (sit->type == ROOM || sit->type == EXPLORE))
  {
    last_place = current_situtation;
    last_pl = curr_sit;
  }
  if(current_situtation->type != MERCHANT && current_situtation->type != TALK)
  {
    last_situation = current_situtation;
    last_sit = curr_sit;
  }

  current_situtation = sit;
  if(index > 0)
    curr_sit = index;
  situation();
}


void show_fight()
{
  printf("\n\x1b[35m               = COMBAT = \x1b[0m\n");
  printf("\n\x1b[31m%s\x1b[0m\n", current_situtation->attack_description);

  getchar();

  srand((unsigned)time(NULL));
  //Get enemies number
  int *nb = current_situtation->enemies_index;
  int enemies_nb = sizeof(nb) / sizeof(int);
  enemies_nb+=SELECT_ADD;
  if(character->has_companion)
        enemies_nb++;

  //Make clone creature of the enemy list
  creature_t instances[enemies_nb];
  for (int i = 0; i < enemies_nb; i++)
  {
    if(character->has_companion && i == enemies_nb-1)
      instances[i] = character->companion;
    else
      instances[i] = creatures[current_situtation->enemies_index[i]];
  }

  //Initiative
  int i, j, a, order[enemies_nb+1];
  unsigned int init[enemies_nb];
  for(int i = 0; i < enemies_nb+1; i++) //max is for player
  {
    if(i == enemies_nb)
      init[enemies_nb] =  roll_dice("Initiative", 1, 20, get_bonus(character->stats.dexterity));
    else
      init[i] = (rand() % 20) + 1 + get_bonus(instances[i].stats.dexterity);

    order[i] = i;

  }
  //Ordrer the list by ascending order
  for (i = 0; i < enemies_nb+1; ++i)
  {
    for (j = i + 1; j < enemies_nb+1; ++j)
    {
      if (init[i] < init[j])
      {
        a =  init[i];
        init[i] = init[j];
        init[j] = a;

        a =  order[i];
        order[i] = order[j];
        order[j] = a;
      }
    }
  }

  getchar();

  _Bool fight_over = false;

  int turn = 0;
  int turn_progress = 0;
  int count;

  char *selection[enemies_nb+1];
  char temp_string[enemies_nb+1][100];
  char *spell_choices[character->spell_count+1];
  char spells_string[character->spell_count+1][200];

  info_t action_infos;

  while(fight_over == false)
  {
    // ================ Player turn ===============

    if(order[turn_progress] == enemies_nb)
    {

      printf("\n\x1b[31m = %s's turn = \x1b[0m", character->name);
      printf("\n\x1b[33m   HP : %d/%d\x1b[0m", character->curr_hp, character->max_hp);
      printf("\n\x1b[32m   MANA : %d/%d\x1b[0m\n", character->curr_mana, character->max_mana);
      char *choices[4] = { "Attack", "Spell", "Use item", "Ran off" };
      int choice = do_choice( "What do you do ?", choices, 4 );

      int target;
      switch(choice)
      {
        //ATTACK
        case 0:
          //List of enemies name + currHp
          count = enemies_nb;
          if(character->has_companion)
            count--;

          for(i = 0; i < count+1; i++)
          {
            if(i < count)
            {
              snprintf(temp_string[i], 100, "%s   %d/%d HP", instances[i].name, instances[i].hp, creatures[current_situtation->enemies_index[i]].hp);
              if(instances[i].hp <= 0)
                snprintf(temp_string[i], 100, "%s   (Dead)", instances[i].name);

              selection[i] = temp_string[i];
            }
            else
              selection[i] = "Back";
          }

          target = do_choice( "Who do you attack ?", selection, count+1 );
          if(target == count)
          {
            turn_progress--;
            break;
          }
          while(instances[target].hp <= 0)
          {
            printf("%s is already dead\n", instances[target].name);
            target = do_choice( "Who do you attack ?", selection, count+1 );
            if(target == count)
            {
              turn_progress--;
              break;
            }
          }
          //Hit
          int hit = roll_dice("Hit", 1, 20, get_bonus(character->stats.strength) + character->bba);
          if(hit >= instances[target].ca)
          {
            //Damage
            printf("You hit %s\n", instances[target].name);
            read_infos(&action_infos, character->eqquiped_weap.note);
            char *action = malloc(sizeof(char*));
            sprintf(action, "Damage %s with %s", instances[target].name, character->eqquiped_weap.name);
            instances[target].hp -= roll_dice(action, action_infos.dice_nb[0], action_infos.dice[0], action_infos.bonus[0] + get_bonus(character->stats.strength));
            if(instances[target].hp <= 0)
            {
              printf("%s died \n", instances[target].name);
              //check if all enemies dead
              count = enemies_nb;
              if(character->has_companion)
                count--;

              _Bool all_dead = true;
              for(i = 0; i < enemies_nb; i++)
              {
                if(instances[i].hp > 0)
                  all_dead = false;
              }
              if(all_dead)
                fight_over = true;
              }
            }
            else
              printf("You missed\n");
          getchar();
          break;
        //SPELL
        case 1:
          //Get spell names list
          for (i = 0; i < character->spell_count+1; i++)
          {
            if(i < character->spell_count)
              sprintf(spells_string[i], "%s  Cost : %d  Effects : %s", character->spell_list[i].name, character->spell_list[i].cost, character->spell_list[i].effects);
            else
              strcpy(spells_string[i], "Back");

            spell_choices[i] = spells_string[i];
          }

          int spell_choice = do_choice("Which spell do you want to throw ?", spell_choices, character->spell_count+1);
          while(character->curr_mana < character->spell_list[choice].cost)
          {
            printf("\x1b[31mYou don't have nough mana to throw this spell\x1b[0m\n");
            printf("\n\n \x1b[32m%d/%d MANA\x1b[0m\n", character->curr_mana, character->max_mana);
            spell_choice = do_choice("Which spell do you want to throw ?", spell_choices, character->spell_count+1);
          }
          character->curr_mana -= character->spell_list[choice].cost;

          if(spell_choice == character->spell_count)
          {
            turn_progress--;
            break;
          }

          //List of enemies name + currHp
          count = enemies_nb+1;

          for(i = 0; i < count; i++)
          {
            if(i < enemies_nb)
            {
              if(instances[i].hp <= 0)
                snprintf(temp_string[i], 100, "%s   (Dead)", instances[i].name);
              else
              {
                if(character->has_companion && i == enemies_nb-1) //companion
                  snprintf(temp_string[i], 100, "%s   %d/%d HP", instances[i].name, instances[i].hp, character->companion.hp);
                else //enemy
                  snprintf(temp_string[i], 100, "%s   %d/%d HP", instances[i].name, instances[i].hp, creatures[current_situtation->enemies_index[i]].hp);
              }
            }
            else //Player
              snprintf(temp_string[i], 100, "%s   %d/%d HP", character->name, character->curr_hp, character->max_hp);

            selection[i] = temp_string[i];
          }

          target = do_choice( "Who do you throw your spell on ?", selection, count );
          if(target < enemies_nb)
          {
            while(instances[target].hp <= 0)
            {
              printf("%s is already dead\n", instances[target].name);
              target = do_choice( "Who do you throw your spell on ?", selection, count );
            }
          }

          if(target < enemies_nb)
          {
            if(character->has_companion && target == enemies_nb-1)
              creature_affected_effects(character->spell_list[spell_choice].effects, character->spell_list[spell_choice].name, &instances[target], character->companion);
            else
              creature_affected_effects(character->spell_list[spell_choice].effects, character->spell_list[spell_choice].name, &instances[target], creatures[current_situtation->enemies_index[i]]);

            if(instances[target].hp <= 0)
            {
              printf("%s died \n", instances[target].name);
              //check if all enemies dead
              count = enemies_nb;
              if(character->has_companion)
                count--;

              _Bool all_dead = true;
              for(i = 0; i < count; i++)
              {
                if(instances[i].hp > 0)
                   all_dead = false;
              }
              if(all_dead)
                fight_over = true;
            }
          }
          else
          {
            player_affected_effects(character->spell_list[spell_choice].effects, character->spell_list[spell_choice].name);
            if(character->curr_hp <= 0)
            {
              getchar();
              printf("%s\n", strcolor("You killed yourself, really ?", 31));
              getchar();
              death();
              return;
            }
          }
          getchar();
          break;
        //USE ITEM
        case 2:
          //Show CONSUMABLE list
          count = 0;
          char *consumables[31];
          int consums_index[30];
          for(i = 0; i < 31; i++)
          {
            if(character->inventory[i].type == CONSUMABLE)
            {
              char *tmpstr = strdup(character->inventory[i].name);
              char itoa[3];
              sprintf(itoa, " - Count : %d", character->inventory[i].count );
              strcat(tmpstr, itoa);
              strcat(tmpstr, " - Effects : ");
              strcat(tmpstr, character->inventory[i].note);

              consumables[count] = tmpstr;
              consums_index[count] = i;
              count++;
            }
          }
          consumables[count] = "Back";

          int choice = do_choice("What object do you want to use ?", consumables, count+1);
          if(choice == count)
          {
            turn_progress--;
            break;
          }

          player_affected_effects(character->inventory[consums_index[choice]].note, character->inventory[consums_index[choice]].name);
          rmv_item(consums_index[choice], 1);
          if(character->curr_hp <= 0)
          {
            getchar();
            printf("%s\n", strcolor("You killed yourself, really ?", 31));
            getchar();
            death();
            return;
          }
          getchar();
          break;
        //RAN OFF
        case 3:
          choices[0] = "I Flee !";
          choices[1] = "I'll fight to my last breath !";
          choice = do_choice("Do you really want to ran off ?", choices, 2);

          if(choice == 0) //Flee
          {
            int flee = roll_dice("Flee", 1, 20, get_bonus(character->stats.dexterity));
            count = enemies_nb;
            if(character->has_companion)
              count--;

            printf("Your enemies try to catch you !\n");
            int average_pursue = 0;
            for(i = 0; i < count; i++)
            {
              int pursue = roll_dice("Pursue", 1, 20, get_bonus(instances[i].stats.dexterity));
              average_pursue += pursue;
            }
            average_pursue /= count;
            printf("\nAverage pursue score : %d\n", average_pursue);
            if(average_pursue > flee)
            {
              printf("%s\n", strcolor("You have been catch in your escape", 31));
              getchar();
              break;
            }
            else
            {
              printf("%s\n", strcolor("You lose your enemies, you did it !", 31));
              getchar();
              change_situation_t(last_situation);
              return;
            }
          }
          else //Warrior mind
          {
            turn_progress--;
            break;
          }
          break;
      }
    }

    // ================ NPCS turn ===============
    else if(instances[order[turn_progress]].hp > 0 && fight_over == false)// npc turn
    {
      int index = order[turn_progress];
      printf("\n\x1b[31m = %s's turn = \x1b[0m", instances[index].name);

      // ================ Enemy turn ===============
      if((character->has_companion && index < enemies_nb-1) || !character->has_companion) // enemy
      {
        printf("\n\x1b[33m   HP : %d/%d\x1b[0m\n", instances[index].hp, creatures[current_situtation->enemies_index[i]].hp);

        int CA;
        char *NAME;

        int rand_target = rand() % 2;
        if(character->has_companion && instances[enemies_nb-1].hp <= 0)
        {
          rand_target = 0;
        }

        if(rand_target == 0 || !character->has_companion)
        {
          rand_target = 0;
          CA = character->ca;
          NAME = character->name;
        }
        else if(rand_target == 1)
        {
          CA = character->companion.ca;
          NAME = character->companion.name;
        }

        //Hit
        int hit = roll_dice("Hit", 1, 20, get_bonus(instances[index].stats.strength) + instances[index].bba);
        if(hit >= CA)
        {
          //Damage
          printf("%s hit %s\n", instances[index].name, NAME);
          read_infos(&action_infos, instances[index].actions);
          int rand_action = rand() % action_infos.size;
          int dmg = roll_dice(action_infos.name[rand_action], action_infos.dice_nb[rand_action], action_infos.dice[rand_action], action_infos.bonus[rand_action] + get_bonus(instances[index].stats.strength));
          printf("%s deals %d damages to %s with %s\n", instances[index].name, dmg, NAME, action_infos.name[rand_action]);

          if(rand_target == 0)
          {
            character->curr_hp -= dmg;
            if(character->curr_hp <= 0)
            {
              getchar();
              printf("\n\x1b[31mYou have been killed by %s\x1b[0m\n", instances[index].name);
              getchar();
              death();
              return;
            }
          }
          else
          {
            instances[enemies_nb-1].hp -= dmg;
            if(instances[enemies_nb-1].hp <= 0)
            {
              printf("\x1b[31m%s has been killed by %s\x1b[0m\n", character->companion.name, instances[index].name);
            }
          }
        }
        else
          printf("%s missed %s\n", instances[index].name, NAME);
      }
      // ================ Companion turn ===============
      else if(character->has_companion) // companion
      {
        printf("\n\x1b[33m   HP : %d/%d\x1b[0m\n", instances[index].hp, character->companion.hp);

        int rand_target = rand() % (enemies_nb-1);
        while (instances[rand_target].hp <= 0)
        {
          rand_target = rand() % (enemies_nb-1);
        }

        //Hit
        int hit = roll_dice("Hit", 1, 20, get_bonus(instances[index].stats.strength) + instances[index].bba);
        if(hit >= instances[rand_target].ca)
        {
          //Damage
          printf("%s hit %s\n", character->companion.name, instances[rand_target].name);
          read_infos(&action_infos, character->companion.actions);
          int rand_action = rand() % action_infos.size;
          int dmg = roll_dice(action_infos.name[rand_action], action_infos.dice_nb[rand_action], action_infos.dice[rand_action], action_infos.bonus[rand_action] + get_bonus(instances[index].stats.strength));
          printf("%s deals %d damages to %s with %s\n", character->companion.name, dmg, instances[rand_target].name, action_infos.name[rand_action]);
          instances[rand_target].hp -= dmg;

          if(instances[rand_target].hp <= 0)
          {
            printf("%s died \n", instances[rand_target].name);
            //check if all enemies dead

            _Bool all_dead = true;
            for(i = 0; i < enemies_nb-1; i++)
            {
              if(instances[i].hp > 0)
                all_dead = false;
            }
            if(all_dead)
              fight_over = true;
          }
        }
        else
          printf("%s missed %s\n", character->companion.name, instances[rand_target].name);
      }
      getchar();
    }

    turn_progress++;
    if(turn_progress >= enemies_nb+1)
    {
      turn++;
      turn_progress = 0;
    }
  }

  current_situtation->type = current_situtation->tobe;
  printf("Fight is over ! Well play\n");
  getchar();
  situation();
}

void show_shop()
{
  puts("\n\x1b[35m = SHOP = \x1b[0m");
  printf("\n\x1b[36m   N° |              NAME              | VALUE |             INFOS               \x1b[0m\n");

  for(int i = 0; i < current_situtation->market_size; i++)
  {
    char n[2];
    sprintf(n, "%d", i);

    char name[30];
    strcpy(name, items[current_situtation->market[i]].name);

    char value[5];
    sprintf(value, "%d", items[current_situtation->market[i]].value);

    printf("\n - %-3s| %-31s| %-6s| %-4s ", n, name, value, items[current_situtation->market[i]].note);
  }
  printf("\n\nCurrent gold : %d\n\nUse 'buy' or 'sell' to interact, see your inventory with 'status inventory'\nQuit the shop by typing 'exit shop'", character->gold_count);
}

void open_container(int index)
{
  printf("%s\n", strcolor("\n                               = CONTAINER = ", 35));

  printf("\n\n  %s : %d", strcolor("Gold", 33), containers[index].gold );
  printf("\n\x1b[36m   N° |              NAME              | VALUE | COUNT |             INFOS               \x1b[0m\n");
  for(int i = 0; i < 30 && containers[index].count[i] != 0; i++)
  {
    printf("\n - %-3d| %-31s| %-6d| %-6d| %-4s ", i, items[containers[index].items[i]].name, items[containers[index].items[i]].value, containers[index].count[i], items[containers[index].items[i]].note);
  }
  printf("\n\nUse 'take gold', 'take 'index' 'count'' or 'take all' to interact with this container\n");
  printf("Quit the container by typing 'exit container'\n");

  while(true)
  {
    printf("\n:");
    char cmd[255];
    fgets(cmd, 255, stdin);
    size_t ln = strlen(cmd) - 1;
    if (cmd[ln] == '\n')
        cmd[ln] = '\0';

    if(strstr(cmd, "take") != NULL)
    {
      if(strstr(cmd, "gold") != NULL)
      {
          character->gold_count += containers[index].gold;
          printf( "%dgp added, you now have %dgp\n", containers[index].gold, character->gold_count );
          containers[index].gold = 0;
          open_container(index);
          return;
      }
      else if(strstr(cmd, "all") != NULL)
      {
        for(int i = 0; i < 30 && containers[index].count[i] != 0; i++)
        {
          add_item(containers[index].items[i], 1);
          containers[index].items[i] = 0;
          containers[index].count[i] = 0;
        }
        character->gold_count += containers[index].gold;
        printf( "%dgp added, you now have %dgp\n", containers[index].gold, character->gold_count );
        containers[index].gold = 0;
        strcpy(cmd, "exit");
      }
      else
      {
        char *only_arg = strdup(cmd);
        strrmv(only_arg, "take ");
        strcat(only_arg, " ");
        char *args[2];
        strsplit(args, only_arg, " ");
        int id = atoi(args[0]);
        int n = atoi(args[1]);
        if(n==0)
          n = 1;

        if(n > containers[index].count[id])
        {
          printf("The count is too high\n");
          getchar();
          open_container(index);
          return;
        }

        add_item(containers[index].items[id], n);
        containers[index].count[id] -= n;
        for(int i = 0; i < 29; i++)
        {
          if(containers[index].count[i] == 0)
          {
            containers[index].items[i] = containers[index].items[i+1];
            containers[index].items[i+1] = 0;
            containers[index].count[i] = containers[index].count[i+1];
            containers[index].count[i+1] = 0;
          }
        }
        open_container(index);
        return;
      }
    }
    if(strstr(cmd, "exit") != NULL)
    {
      situation();
      get_cmd();
      return;
    }
    printf("\nThis command doesn't exist or is not possible when in a container\n");
    printf("Use 'take 'index'' or 'take all' to interact with this container\n");
    printf("Quit the container by typing 'exit container'\n");
  }
}

void color_keywords(char* str, char *kw[], int color)
{
  for(int i = 0; i < 10 && kw[i] != NULL && strlen(kw[i]) > 1 ; i++)
  {
    strcpy(str, repl_str(str, kw[i], strcolor(kw[i], color)));
  }
}

//ALL CONTAINERS
container_t containers[10] =
{
  {
    12,
    { 1, 3, 5, 7 },
    { 1, 10,2, 1 }
  }
};

//ALL LINES
line_t l4_5 = { "pay(200) companion(0) nxtsit(1)" };
line_t l4_4 = { "Perfect ! Give me 200gp and he will protect your for a while ! (action : pay)", {"pay"}, {&l4_5} };
line_t l4_3 = { "Yann is one of our best element ! It is the guy with a leather jacket and dark hair at my left, want to recruit him (200gp)", {"recruit"}, {&l4_4} };
line_t l4_2 = { "It's none of your buisness, all I can tell you is that we have been well paid" };
line_t l4_1 = { "Perfect ! Two of us already have a mission but Yann could join you.", {"mission", "Yann", "join"}, {&l4_2, &l4_3, &l4_4} };

line_t l3_2 = { "The dark forest is full of spiders" };
line_t l3_1 = { "nxtsit(3)" };

//ALL SITUATIONS
situation_t situations[100] = {
  { //ANARION PLACE - 0
    EXPLORE,
    "You are in the middle of the village of Anarion, the place is full of life and the weather is pretty good\nAt your right is the tavern\nIn front of you siege the castle of the Anarion's lord\nBehind you, the gate of the city gives on the anarion's forest",
    .explore_names = { "tavern", "castle", "forest" },
    .explore_index = { 1, 5, 6 }
  },
  { //ANARION TAVERN - 1
    ROOM,
    "You are in the tavern, there is a good mood here, the taverner is at your left behind the bar. You can see some sort of mercenaries in the back of the room",
    .talk_names = { "taverner", "mercenaries" },
    .talk_index = { 2, 4 }
  },
  { //ANARION_TAVERN_TAVERNER - 2
    TALK,
    "The taverner is an old man but he seems pretty strong for his age, he gives you a big smile when you approach",
    .line = { "Hi stranger ! Are you here to drink or get infos ?", { "drink", "infos" }, { &l3_1, &l3_2 }}
  },
  { //ANARION_TAVERN_TAVERNER_SHOP - 3
    MERCHANT,
    "The taverner tells you what you can buy here",
    .market_size = 3,
    .market = { 4,5,6 }
  },
  { //ANARION_TAVERN_MERCENARIES - 4
    TALK,
    "Three mercenaries are talking around a beer, one of them see you approaching and inform his compagnions. The tallest talks to you",
    .line = {"Welcome to our table ! Do you need our services ?", {"services"}, {&l4_1} },
    .recruitable_companion = "Yann"
  },
  { //ANARION CASTLE - 5
    ROOM,
    "You get to the castle gate, two guards block your way",
    .interact_names = {"gate"},
    .interact_descriptions = {"A big steel gate"},
    .interacts = {"nxtsit(7)"}
  },
  { //ANARION FOREST - 6
    FIGHT,
    "You are in the middle of the forest, you have defeated two wolfs here\nSouth is anarion's city\nYou can go in every directions",
    .attack_description = "You were walking in the forest when two wolfs attack you",
    .enemies_index = { 0, 0 },
    .tobe = EXPLORE
  },
  { //ANARION CASTLE - 7
    ROOM,
    "You are in the castle's hall, there is a HUGE BIG GREAT chest waiting for you",
    .interact_names = {"chest"},
    .interact_descriptions = {"a HUGE BIG GREAT chest"},
    .interacts = {"container(0)"}
  }
};
