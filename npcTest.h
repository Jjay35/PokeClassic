#include <stdio.h>
#include <stdlib.h>
// #include "pokemon.h"
// #include "items.h"
#include "pokemonData.h"
#include <vector>
class npc{
    public:
    int coordX;
    int coordY;
    int prevX;
    int prevY;
    char npcType;
    int numberofPokemon;
    // vector<pokemonData> trainerPokemon;
    pokemonData trainerPokemon[6];
};

class npcPokemons{
    public:
    vector<vector<pokemonData>> pokemonList;

};

class groupNPC{
    public:
    npc* arrNPCs; 
    int size;
};

class player{
    public:
    int posX;
    int posY;
    int numberOfPokemon;
    vector<pokemonData> playerPokemons;
    vector<pokemonData> computerPokemons;
    int potions;
    int revives;
    int pokeballs;
    int pokeBucks;
};