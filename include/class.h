#ifndef _JORG_CLASS_H
#define _JORG_CLASS_H

typedef struct
{
	int strength;
	int dexterity;
	int constitution;
	int intellect;
	int wisdow;
	int charisma;
} stats_t;

stats_t classes[100];

int get_bonus(int stat);

#endif /* _JORG_CLASS_H */
