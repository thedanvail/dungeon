#include <_ctype.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <math.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <inttypes.h>
#include <sstream>
#include <iostream>
#include <cassert>

using namespace std;

#define assertmsg(exp, msg) assert(((void)msg, exp))
// For text "colo(u)r"
vector<string> descriptors = { 
    "a gigantic",
    "a huge",
    "an intimidatingly large",
    "a gargantuan",
    "a monstrous",
    "an exquisite",
    "a menacing"
};

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

class ICombat {
    public:
        int hp;
        int strength;
        int evasion;
        int armor;

        void takeDamage(int strength) {
            int damage = (10 * strength - armor) - (10 * strength - armor) * evasion/100.0f;
            if (damage < 0) {
                damage = 0; // No matter how bad the opponent, their weapons probably shouldn't heal us.
            }
            printf("%d damage was dealt!\n", damage);
            this->hp -= damage;
        };
        void attack(ICombat& defender) { 
            assertmsg(!this->isDead(), "shouldn't be attacking if dead already\n");
            defender.takeDamage(this->strength);
        };
        bool isDead() { return this->hp <= 0; };

};

class Player : public ICombat {
    public:
        Player() { hp = 99999; strength = 30; armor = 99; evasion = 45; };
};

enum MonsterType {
    Troll,
    Goblin,
};

static unordered_map<string, MonsterType> monsterTypeLookup {
    {"GOBLIN", MonsterType::Goblin},
    {"TROLL", MonsterType::Troll},
};

// The naming here is to keep convention with functions such as stoi
MonsterType stomt(string type) {
    transform(type.begin(), type.end(), type.begin(), ::toupper);
    return monsterTypeLookup[type];
};

class Encounter {
    public:
        virtual string getName() { return "Encounter"; }
        virtual bool isMonster() = 0;
};

class Treasure : public Encounter {
    public:
        float treasureValue;
        Treasure() {};
        bool isMonster() { return false; };
        string getName() {
            return "Treasure";
        }
};

class Monster : public Encounter, public ICombat {
    public:
        MonsterType monsterType;
        bool isMonster() { return true; };
};

class MonsterGoblin : public Monster {
    public:
        MonsterGoblin() { hp=150; strength=5; evasion=6; armor=2; monsterType=Goblin; };
        string getName() { return "Goblin"; };
};

class MonsterTroll : public Monster {
    public:
        MonsterTroll() { hp=230; strength=14; evasion=1; armor=7; monsterType=Troll; };
        string getName() { return "Troll"; };
};

struct EncounterMgr {
    // factory methods
    Encounter* create_monster(std::string type) { 
        MonsterType monsterType = stomt(type);
        switch (monsterType) {
            case Troll:
                printf("Creating a troll.\n");
                return new MonsterTroll();
            case Goblin:
                printf("Creating a goblin.\n");
                return new MonsterGoblin();
        }
    }
    Encounter* create_treasure() {
        // create
        Treasure* treasure = new Treasure();

        // configure   
        treasure->treasureValue = (std::rand() % 30);
        return treasure;
    }
};


/**
  @brief When the adventurer enters a new room, there will be an encounter
  featuring monsters and treasures. In one "round", everyone attacks each other
  at the same time. The adventurer choses to attack a monster and doesn't directly
  interact with the treasures. The rounds continue until the battle is won or lost
 **/
bool battle(Player& player, vector<Encounter*>& encounters)
{
    printf("You're face-to-face with %s %s!\n", descriptors[rand() % descriptors.size()].c_str(), encounters[0]->getName().c_str());

    while(encounters.size() > 0) {
        // adventurer
        if (player.isDead()) {
            // game_over
            return false;
        }
        // hero attacks 1st alive monster
        /* Going off the comment above, the player was only supposed to attack
         * a SINGLE monster, but was attacking them each. Given how over powered the player
         * is with the settings given in the code (that I shouldn't worry about - that's the 
         * designer's job), the player was slaughtering EVERYTHING before it knew what happened.
         * As a result, the change has been made so the player fights ONE monster each round.
         * */
        Encounter* e = encounters[0];
        if (e->isMonster()) {
            printf("Player attacks! ");
            Monster* m = dynamic_cast<Monster*>(e);
            player.attack(*m);
        }

        for (int i=0; i < encounters.size(); i++)
        {
            // Monster
            if (encounters[i]->isMonster()) {
                Monster* m = (Monster*)encounters[i];
                if (m->isDead()) {
                    encounters.erase(encounters.begin()+i);
                } else {
                    printf("The monster attacks! ");
                    m->attack(player);
                    if (player.isDead()) {
                        return false;
                    }
                }
            } else { // Treasure
                Treasure* t = (Treasure*)encounters[i];
                printf("Your treasure increases your health by %f points\n", t->treasureValue);
                player.hp += t->treasureValue;
                t->treasureValue -= t->treasureValue/2;
                if(t->treasureValue <1) {
                    encounters.erase(encounters.begin()+i);
                }
            }
        }
    }
    return true;
}


/**
  main
 **/
int main() {
    std::string s, line;
    int levels = -1;

    printf("Generating Dungeon...\nHow many levels should be generated?\n");   
    cin >> line;

    try {
        levels = stoi(line);
    } catch (exception _e) { // Either invalid argument (alphabetical) or stoi will be out of range
        printf("What you ask is impossible. You get 10 levels.\n");
        levels = 10;
    }

    while (levels < 1) {
        printf("Sorry, I can't generate less than one level. Gimme another number.\n");
        cin >> line;
        try {
            levels = stoi(line);
        } catch (exception _e) {
            printf("What you ask is impossible. You get 10 levels.\n");
            levels = 10;
        }
    }

    levels+=1; // Lookup table is 0-n indexed inclusive. temp increase num lvls.
    std::vector< vector<int> > encounter_gen_table(levels, std::vector<int>(levels));
    for (int r = 0; r<levels; r+=1) {
        for (int c = 0; c<levels; c+=1) {
            if (c <= r)
                encounter_gen_table[r][c] = horde_factor(r, c);
        }
    }

    levels-=1;
    printf("Enter the dungeon? ");
    cin >> s;
    transform(s.begin(), s.end(), s.begin(), ::toupper);

    if(s.compare("N") == 0 || s.compare("NO") == 0) {
        printf("That wasn't really a question. You're in anyway!\n");
    } else if (s.compare("Y") == 0 || s.compare("YES") == 0) {
        printf("You push the Fear and Hunger from your mind and enter the dungeon.\n");
    } else {
        printf("A passerby, who just so happens to only understand the words 'yes' and 'no', sees you speaking gibberish and pushes you into the dungeon out of fright.\n");
    }
    
    // Construct our important pieces
    Player player = Player();
    int i = 0, j=0;
    EncounterMgr mgr;
    std::vector<Encounter*> encounters;

    while (1) {
        assert(encounters.size() == 0);

        printf("Player health: %d\n", player.hp);
        printf("Do you go left or right?\n");
        cin >> s;
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s.compare("LEFT") != 0 && s.compare("RIGHT") != 0) {
            printf("%s is not a valid direction. Try again. You can do this.\n", s.c_str());
            continue;
        }
        i++;
        j+= (!s.compare("LEFT")) ? 0 : 1;

        printf("You go %s\n", s.c_str());
        printf("----------------------------------------\n");
        printf("|                 LEVEL %d              |\n", i);
        printf("----------------------------------------\n");

        // A troll in the dungeon!
        for(int monsters=0; monsters<j * encounter_gen_table[i][j]; monsters++)  
            encounters.push_back(mgr.create_monster(s.compare("LEFT") ? "Goblin" : "Troll"));
        // Bout to generate a LOT of treasure.
        for(int treasures=0; treasures<(i-j) * encounter_gen_table[i][j]; treasures++)   
            encounters.push_back(mgr.create_treasure());

        if (!battle(player, encounters)) {
            printf("You're dead. Sorry 'bout that. Real shame.\n");
            break;
        }
        /* Confirm that all monsters are dead after the battle. */
        for (Encounter* monster : encounters) 
            if (monster->getName().find("Treasure") != 0) {
                assert(((Monster*)monster)->hp<=0.0);
            }

        if (i >= levels) {
            printf("You won!\n");
            break;
        }
    }

    return 0;
}

