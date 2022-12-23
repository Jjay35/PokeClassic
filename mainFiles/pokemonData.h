#include <iostream>
#include <cstdlib>
#include <cstring>
#include "database.h"

using namespace std;
class pokemonData{
public:
    
    int id;
    int level;
    string name;
    string move1;
    string move2;
    int priority1;
    int priority2;
    int power1;
    int power2;
    int aliveStatus;
    int mainHp;
    int hp;
    int attack;
    int defense;
    int speed;
    int speAtk;
    int speDef;

    pokemonData(){}
    
    ~pokemonData(){}
};
