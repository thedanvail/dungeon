#include <cstdio>
#include <cstring>
#include <math.h>
#include <string>
#include <vector>
#include <inttypes.h>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace std;



// Adventurer stats.
float health	= 99999;
float strength	= 30;  // comb. w enemy armor for AP
float armor		= 99; ///< Counters enemy attack power directly
float evasion	= 45; // % damage decrease in the end.

unsigned int f(unsigned int n){
	if (n <= 1)
		return 1;
	else
		return n * f(n-1);
}
// binomial coefficient
int horde_factor(int n, int k) {
	return f(n) / (f(k) * f(n-k));
}


class ICombat
{
public:
	float hp;
	float strength;
	float evasion;
	float armor;
};

class Encounter {
public:
	string getName() { return "Enconter"; };
};

class Treasure : public Encounter {
public:
	float value;
};

class Monster : public Encounter, public ICombat {
public:
	void attack() { if(!this->hp) assert("shouldn't be attacking if dead already"); health -= (10 * this->strength - armor) - (10 * this->strength - armor) * evasion/100.0f; };
private:
	//Sprite animation; // used in the graphical version
	//vector<Sound> reactions; // used in the graphical version
};

class MonsterGoblin : public Monster {
public:
	MonsterGoblin() { hp=150; strength=5; evasion=6; armor=2;};
	string getName() { return "Monster Goblin"; }
};

class MonsterTroll : public Monster {
public:
	MonsterTroll() { hp=230; strength=14; evasion=1; armor=7;};
	string getName() { return "Monster Troll"; }
};

struct EncounterMgr {
	// factory methods
	Encounter* create_monster(std::string type) { if (type == "Goblin") return new MonsterGoblin(); else return new MonsterTroll(); }
	Encounter* create_treasure() {
		// create
		Treasure* treasure = new Treasure();

		// configure   
		treasure->value = (std::rand() % 30);
		printf("%g\n", treasure->value);
		return treasure;
	}
};


/**
@brief When the adventurer enters a new room, there will be an encounter
featuring monsters and treasures. In one "round", everyone attacks each other
at the same time. The adventurer choses to attack a monster and doesn't directly
interact with the treasures. The rounds continue until the battle is won or lost
**/
bool battle(vector<Encounter*> enconters)
{
	printf("You're facing a huge %s\n", enconters[0]->getName());

	// adventurer
	if (health == 0.0f)
	{
		// game_over
		return false;
	}
	else
		// hero attacks 1st alive monster
		for (Encounter* e : enconters)
			if (strncmp(e->getName().c_str(),"Monster",7)==0)
				((Monster*)e)->attack();

	for (int i=0; i < enconters.size(); ++i)
	{
		// Monster
		if (!strncmp(enconters[i]->getName().c_str(),"Monster",7))
		{
			Monster* m = (Monster*)enconters[i];
			if (m->hp > 0) m->attack();
			else enconters.erase(enconters.begin()+i);
		}
		// Treasure
		if (!strncmp(enconters[i]->getName().c_str(),"Treasure",8))
		{
			Treasure* t = (Treasure*)enconters[i];
			health += t->value;
			t->value -= t->value/2;
			if(t->value <1)
				enconters.erase(enconters.begin()+i);
		}
	}

	return true;
}


/**
main
**/
int main()
{
	std::string s;
	printf("Generating Dungeon...\n");   
	int l;  
	cin >> l;
	l+=1; // Lookup table is 0-n indexed inclusive. temp increase num lvls.
	std::vector< vector<int> > encounter_gen_table(l, std::vector<int>(l));
	for (int r = 0; r<l; r+=1) {
        for (int c = 0; c<l; c+=1) {
            if (c <= r)
		    encounter_gen_table[r][c] = horde_factor(r, c);
		}
	}
	//for (int r = 0; r<l; r+=1) {
	//	for (int c = 0; c<l; c+=1)
	//		cout << encounter_gen_table[r][c] << ", ";
	//	cout << " (lvl " << r <<")" << endl; 
	//}
	l-=1;

	printf("Enter the dungeon? ");
	cin >> s;
	if(s.compare("n") != 0 || s.compare("no") != 0)
		printf("That wasn't really a question. You're in anyway!\n");

	int i = 0, j=0;
	while (1)
	{
		printf("Left or Right? ");
		cin >> s;
		i++;
		j+= (!s.compare("left")) ? 0 : 1;

		// A troll in the dungeon!
		EncounterMgr mgr;
		std::vector<Encounter*> v;
		for(int monsters=0; monsters<j * encounter_gen_table[i][j]; monsters++)  
			v.push_back(mgr.create_monster(s.compare("left") ? "Goblin" : "Troll"));
		for(int treasures=0; treasures<(i-j) * encounter_gen_table[i][j]; treasures++)   
			v.push_back(mgr.create_treasure());

		if (!battle(v));
			break;

		/* Confirm that all monsters are dead after the battle. */
		for (auto monster : v) 
			if (monster->getName().starts_with("Treasure"))
				assert(((Monster*)monster)->hp==0.0);

		if (i >= l)
			break;
	}

	printf("You won!");
	return 0;
}

