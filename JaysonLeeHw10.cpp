#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "latestMap.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <cstring>
#include <vector>
using namespace std;
#define INFINITY 9999

int trainerCount = 5;
int totalDistance = 2;
int npcPokemon = 0;
int playerPokemon = 0;
/*
Functions for pokemon vectors
*/
pokemonData spawnPokemon(map_m * map, pokeDatabase * pokeData);
void printPlayerPokemonInfo(player *player);
void printNPCPokemon(map_m *map, groupNPC *groupNPC);
void pokemonPrompt(player *player, pokeDatabase* pokeData);
void battleBegin(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map);
int critChance();
int checkPokemonMoveTypePlayer(pokeDatabase * pokeData,player *player);
int checkPokemonMoveTypeNPC(pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map);
int checkFirstMoveAccuracy(pokeDatabase * pokeData, player *player);
int checkFirstMoveAccuracyNPC(pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map);
int checkSecondMoveAccuracy(pokeDatabase * pokeData, player *player);
int checkSecondMoveAccuracyNPC(pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map);
void fightOption(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map);
int switchPokemon(player *player);
int bagWindow(player *player);
void optionSelecter();
void wildPokemonBattle(pokeDatabase *pokeData, player *player, pokemonData wildPokemon);
int wildPokemonBagWindow(player *player, pokemonData wildPokemon);
int runFromPokemon(player *player, pokemonData wildPokemon, int escapeAttempt);
void spawnPlayerItems(player *player);
void pokemonInterface(player* player);
int revivePokemon(player* player, char input);
int potionOnPokemon(player *player, char input);
int simulateNPCPokemonAttack(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map);
int checkAlivePokemon(player *player);
int openBagWindow(player *player);
int fightWildPokemon (pokeDatabase *pokeData , player * player, pokemonData wildPokemon);
int pokemonCatchRate();
// void directionMovementNPC( map_m *map, int direction, char t[ROWS][COLUMNS], )
// if(tempNode.direction == 1){
//           npcMap[tempNode.currX+1][tempNode.currY] = tempNode.symbol;
//           npcMap[tempNode.currX][tempNode.currY] = ' ';
//           tempNode.dist += map->rivalNodes[tempNode.currX+1][tempNode.currY].cost;
//           tempNode.currX++;
//           tempNode.direction = 1;
//         }
void npcMovement(MinHeap *heap, map_m *map, char npcMap[ROWS][COLUMNS], int count);
void updateNPCmap(int posX, int posY, int newPosX, int newPosY, char npcMap[ROWS][COLUMNS]);
int checkPlayerSurrounding(map_m *map, int posX, int posY);
void fillHikerValue(map_m *map, player *player);
void fillRivalNodes(map_m *map, player *player);

typedef struct World{
  map_m *maps[401][401];
  int mapXCoord;
  int mapYCoord;
}world_w;

/*
World methods
*/
void checkSurrounding(world_w *world, int exitCoordinates[8]);
void closeBorder(world_w *world, char table[21][80], int exitCoordinates[8]);
int checkOutofBounds(int mapCoordinate);
void exitUp(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData);
void exitDown(world_w *world, char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData);
void exitLeft(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData);
void exitRight(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData);
void flycommand(world_w *world, int x, int y, char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData, int distance);


void swapFunc(player* player, int firstIndex, int secondIndex){
  pokemonData temp = player->playerPokemons[firstIndex-1];
  player->playerPokemons[firstIndex-1] = player->playerPokemons[secondIndex-1];
  player->playerPokemons[secondIndex-1] = temp;
}

void swapPokemonInTeam(player* player){
  char inputKey;
  int firstIndex;
  int secondIndex;

  while(inputKey != 'p'){
    clear();
    printw("Current pokemon team\n");
    for(int i = 0; i < player->playerPokemons.size(); i++){
      printw("Index: %d, Pokemon name: %s, Level: %d, Hp: %d\n", i+1, player->playerPokemons[i].name.c_str(), player->playerPokemons[i].level, player->playerPokemons[i].hp);
    }
    printw("Enter 1 if you want to swap pokemons in your team. Enter p again to leave the window.");
    

    inputKey = getch();
    if(inputKey == '1'){
      printw("\nEnter index of the pokemon you want to swap positions with. Enter two numbers.");
      scanw("%d %d", &firstIndex, &secondIndex);
      if(player->playerPokemons.size() < 2){
        printw("You got no other pokemon to swap with.");
        getch();
        break;
      }
      if((firstIndex < 1 || secondIndex < 1 )&&  (firstIndex != secondIndex) ){
        printw("Try again");
        getch();
      }
      else if(firstIndex <= player->playerPokemons.size() && secondIndex <= player->playerPokemons.size()){
        swapFunc(player, firstIndex, secondIndex);
        printw("Successfully swapped!");
        getch();
      }
      else{
        printw("Transfer fail..");
        getch();
      }
    }
  }
  
  

 
}

void swapPokemonPC(player *player, int pokemonInParty, int pokemonInComp){
  pokemonData temp = player->playerPokemons[pokemonInParty-1];
  player->playerPokemons[pokemonInParty-1] = player->computerPokemons[pokemonInComp-1];
  player->computerPokemons[pokemonInComp-1] = temp;
  
}

void pokeCenterAndMart(char npcMap[ROWS][COLUMNS], map_m *map, player *player, int inputKey){
  int amount;
  int pokemonInParty;
  int pokemonInComp;
  if(inputKey == 62 && map->pointerArr[player->posX][player->posY] == 'C'){
    
    while(inputKey != 60){
      clear();
      printw("Welcome to pokeCenter!\n");
      for(int i = 0; i < player->playerPokemons.size(); i++){
        player->playerPokemons[i].hp = player->playerPokemons[i].mainHp;
        player->playerPokemons[i].aliveStatus = 1;
      }
      player->numberOfPokemon = player->playerPokemons.size();
      playerPokemon = 0;
      printw("Your pokemons are now fully restored and ready for battle!\n");
      printw("You can access your Computer to choose which pokemon you want to join your party or stay behind.\n");
      printw("Press 1 to access the Computer");

      inputKey = getch();
      
      if(inputKey == '1'){
        clear();
        if(player->computerPokemons.size() == 0){
          printw("You have nothing in your Computer, go catch more wild pokemon and come again.");
          getch();
          break;
        }
        printw("Number of pokemon you have in Computer: %d\n", player->computerPokemons.size());
        for(int i = 0; i < player->computerPokemons.size(); i++){
          printw("Index: %d , Pokemon name: %s, Pokemon level: %d\n", i+1 , player->computerPokemons[i].name.c_str(), player->computerPokemons[i].level);
        }

        printw("\nYour current team in party:\n");
        for(int i = 0; i < player->playerPokemons.size(); i++){
          printw("Index: %d , Pokemon Name: %s\n", i+1, player->playerPokemons[i].name.c_str());
        }
        printw("\nIs there any pokemon you want to swap with? Press 1 to continue.\n");
        refresh();
         inputKey = getch();
        if(inputKey == '1'){
          printw("Enter the index number of your pokemon and the number of the pokemon in the Comptuer you want to swap with");
          scanw("%d %d", &pokemonInParty, &pokemonInComp);
          if((pokemonInParty < 0 || pokemonInComp < 0) && player->computerPokemons.size() < pokemonInComp){
            printw("Try again");
            getch();
          }
          else if( pokemonInParty > 0 && pokemonInComp > 0 && player->computerPokemons.size() >= pokemonInComp){
            swapPokemonPC(player, pokemonInParty, pokemonInComp);
            printw("Successfully swapped!");
            getch();
          }
          else{
            printw("Transfer fail..");
            getch();
          }
        }
      }
    }
  }
  if(inputKey == 62 && map->pointerArr[player->posX][player->posY] == 'M'){
    // clear();
    // printw("Welcome to pokeMart!\n");
    // printw("What do you want to buy? Press 1, 2, or 3 accordingly\n");
    // printw("Potions \n Revives\n Pokeballs\n");
    // refresh();
    // inputKey = getch();

    // player->potions = 5;
    // player->revives = 5;
    // player-> pokeballs = 5;
    // printw("Your items are refilled and ready to be used again!");
    while(inputKey != 60){
       clear();
    printw("Welcome to pokeMart!\n");
    printw("What do you want to buy? Press 1, 2, or 3 accordingly\n");
    printw("You currently have %d pokeBucks, %d potions, %d revives, and %d pokeballs\n", player->pokeBucks, player->potions, player->revives, player->pokeballs);
    printw("Potions: 1 for 100 pokeBucks\nRevives: 1 for 100 pokeBucks\nPokeballs: 1 for 100 pokeBucks\n");
    refresh();
        inputKey = getch();

      if(inputKey == '1'){
            clear();
            printw("You currently have %d pokeBucks, %d potions, %d revives, and %d pokeballs\n", player->pokeBucks, player->potions, player->revives, player->pokeballs);
            printw("How many would you like to buy? Enter a number.");
            scanw("%d", &amount);
            if(amount < 0){
              printw("Funny request, please try again");
              getch();
            }
            if(player->pokeBucks < (amount * 100)){
              printw("You do not have enough pokeBucks.");
              getch();
            }
            else if (player->pokeBucks > (amount * 100) && amount > 0){
              printw("You bought %d potions", amount);
              player->pokeBucks = player->pokeBucks - (amount * 100);
              player->potions += amount;
              getch();
            }
        
      
      
    }
    if(inputKey == '2'){
        clear();
      printw("You currently have %d pokeBucks, %d potions, %d revives, and %d pokeballs\n", player->pokeBucks, player->potions, player->revives, player->pokeballs);
        printw("How many would you like to buy? Enter a number.");
        scanw("%d", &amount);
        if(amount < 0){
          printw("Funny request, please try again");
          getch();
          break;
        }
        if(player->pokeBucks < (amount * 100)){
          printw("You do not have enough pokeBucks.");
          getch();
        }
        else if(player->pokeBucks > (amount * 100) && amount > 0){
          printw("You bought %d revives", amount);
          player->pokeBucks = player->pokeBucks - (amount * 100);
          player->revives += amount;
          getch();
        }
      
      
    }
    if(inputKey == '3'){
  
        clear();
      printw("You currently have %d pokeBucks, %d potions, %d revives, and %d pokeballs\n", player->pokeBucks, player->potions, player->revives, player->pokeballs);
        printw("How many would you like to buy? Enter a number.");
        scanw("%d", &amount);
        if(amount < 0){
          printw("Funny request, please try again");
          getch();
          break;
        }
        if(player->pokeBucks < (amount * 100)){
          printw("You do not have enough pokeBucks.");
          getch();
        }
        else if(player->pokeBucks > (amount * 100) && amount > 0){
          printw("You bought %d pokeballs", amount);
          player->pokeBucks = player->pokeBucks - (amount * 100);
          player->pokeballs += amount;
          getch();
        }
      
      
    }
   
    }
    
  }
}

int playerMovement(world_w *world, char npcMap[ROWS][COLUMNS], map_m *map, player *player, int inputKey, pokeDatabase * pokeData){
  
  // movementKey = getch();
  switch(inputKey){
    case '7':
    case 'y':
    if(player->posX-1 == 0 && player->posY-1 == map->exits[1]){
      player->posX = 19;
      player->posY = map->exits[1];
      exitUp(world, npcMap, player, pokeData);
      return 2;
      
    }
    if(player->posX-1 == map->exits[4] && player->posY-1 == 0){
      player->posX = map->exits[4];
      player->posY = 78;
      exitLeft(world, npcMap, player, pokeData);
      return 4;
      
    }
    if(checkPlayerSurrounding(map,player->posX-1, player->posY-1)){
        // npcMap[player->posX-1][player->posY-1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX-1, player->posY-1, npcMap);
        player->posX--;
        player->posY--;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        
        return 1;
    }
    break;
    
    case '8':
    case 'k':
    if(player->posX-1 == 0 && player->posY == map->exits[1]){
      player->posX = 19;
      player->posY = map->exits[1];
      exitUp(world, npcMap, player, pokeData);
      // world->mapYCoord++;
      return 2;
      
    }
    if(checkPlayerSurrounding(map,player->posX-1, player->posY)){
        // npcMap[player->posX-1][player->posY]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX-1, player->posY, npcMap);
        player->posX--;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '9':
    case 'u':
    if(player->posX-1 == 0 && player->posY+1 == map->exits[1]){
      player->posX = 19;
      player->posY = map->exits[1];
      exitUp(world, npcMap, player, pokeData);
      // world->mapYCoord++;
      return 2;
    }
    if(player->posX-1 == map->exits[6] && player->posY+1 == 79){
      player->posX = map->exits[6];
      player->posY = 1;
      exitRight(world,npcMap, player, pokeData);
      return 5;
    }
    if(checkPlayerSurrounding(map,player->posX-1, player->posY+1)){
        // npcMap[player->posX-1][player->posY+1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX-1, player->posY+1, npcMap);
        player->posX--;
        player->posY++;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '6':
    case 'l':
    if(player->posX == map->exits[6] && player->posY+1 == 79){
      player->posX = map->exits[6];
      player->posY = 1;
      exitRight(world,npcMap, player, pokeData);
      return 5;
    }
    if(checkPlayerSurrounding(map,player->posX, player->posY+1)){
        // npcMap[player->posX][player->posY+1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX, player->posY+1, npcMap);
        player->posY++;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '3':
    case 'n':
    if(player->posX+1 == map->exits[6] && player->posY+1 == 79){
      player->posX = map->exits[6];
      player->posY = 1;
      exitRight(world,npcMap, player, pokeData);
      return 5;
    }
    if(player->posX+1 == 20 && player->posY+1 == map->exits[3]){
      player->posX = 1;
      player->posY = map->exits[3];
      exitDown( world,npcMap, player, pokeData);
      return 3;
    }
    if(checkPlayerSurrounding(map,player->posX+1, player->posY+1)){
        // npcMap[player->posX+1][player->posY+1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX+1, player->posY+1, npcMap);
        player->posX++;
        player->posY++;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '2':
    case 'j':
     if(player->posX+1 == 20 && player->posY == map->exits[3]){
      player->posX = 1;
      player->posY = map->exits[3];
      exitDown(world, npcMap, player, pokeData);
      // world->mapYCoord++;
      return 3;
      
    }
    if(checkPlayerSurrounding(map,player->posX+1, player->posY)){
        // npcMap[player->posX+1][player->posY]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX+1, player->posY, npcMap);
        player->posX++;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '1':
    case 'b':
    if(player->posX+1 == map->exits[4] && player->posY-1 == 0){
      player->posX = map->exits[4];
      player->posY = 78;
      exitLeft(world, npcMap, player, pokeData);
      return 4;
      
    }
    if(player->posX+1 == 20 && player->posY-1 == map->exits[3]){
      player->posX = 1;
      player->posY = map->exits[3];
      exitDown(world, npcMap, player, pokeData);
      return 3;
      
    }
    if(checkPlayerSurrounding(map,player->posX+1, player->posY-1)){
        // npcMap[player->posX+1][player->posY-1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX+1, player->posY-1, npcMap);
        player->posX++;
        player->posY--;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

    case '4':
    case 'h':
    if(player->posX == map->exits[4] && player->posY-1 == 0){
      player->posX = map->exits[4];
      player->posY = 78;
      exitLeft(world, npcMap, player, pokeData);
      return 4;
      
    }
    if(checkPlayerSurrounding(map,player->posX, player->posY-1)){
        // npcMap[player->posX][player->posY-1]= '@';
        // npcMap[player->posX][player->posY] = ' ';
        updateNPCmap(player->posX, player->posY, player->posX, player->posY-1, npcMap);
        player->posY--;
        fillHikerValue(map,player);
        fillRivalNodes(map,player);
        return 1;
    }
    break;

  }    
  return 0;
}

int checkPlayerSurrounding(map_m *map, int posX, int posY){
  //posX < (ROWS - 1) && posY < (COLUMNS - 1) && posX > 0 && posY > 0
   if (posX < (ROWS - 1) && posY < (COLUMNS - 1) && posX > 0 && posY > 0 && map->pointerArr[posX][posY] != '%' && map->pointerArr[posX][posY] != '^') {
        return 1;
    } else {
        return 0;
    }
}
void scrollingUpAndDown(int count, char(*buffer)[200]){
  int offset = 0;
  while(1){
    for(int i = 0; i < 20; i++){
        mvprintw(i, 0, "%s", buffer[i + offset]);
    }
    switch(getch()){
    case KEY_UP:
    if(offset){
      offset--;
    } 
    break;

    case KEY_DOWN:
      if(offset < (count - 20)){
       offset++;
      }
    break;
                
    case 27:
    return;
    } 
  }
  
}


void getNPCpositions(int officialTrainerCount, char npcMap[ROWS][COLUMNS], map_m *map, player *player, int inputKey){

int npcPosX;
int npcPosY;
int npcSymbol;
int distanceX;
int distanceY;
char north[] = "north";
char west[] = "west";
char south[] = "south";
char east[] = "east";

char (*buffer) [200];
buffer = (char (*)[200]) malloc(map->trainers * sizeof(*buffer));
int input = getch();
  
  if(inputKey == 116){
    clear();
    refresh();

    for(int i = 0; i < map->trainers; i++){
      npcPosX = map->heap->arrNodes[i].currX;
      npcPosY = map->heap->arrNodes[i].currY;
      npcSymbol = map->heap->arrNodes[i].symbol;

      //Case 1: npc is above and on the left of PC
      if(player->posX > npcPosX && player->posY > npcPosY){
        distanceX = player->posX - npcPosX;
        distanceY= player->posY - npcPosY;
        // printw("%c, %d %s and %d %s\n", npcSymbol, distanceX, north, distanceY, west);
        snprintf(buffer[i], 200, "%c, %d %s and %d %s\n", npcSymbol, distanceX, north, distanceY, west);
      }

      //Case 2: npc is below and on the left of PC
      if(player->posX < npcPosX && player->posY > npcPosY){
        distanceX = npcPosX - player->posX;
        distanceY= player->posY - npcPosY;
        // printw("%c, %d %s and %d %s\n", npcSymbol, distanceX, south, distanceY, west);
        snprintf(buffer[i], 200, "%c, %d %s and %d %s\n", npcSymbol, distanceX, south, distanceY, west);
      }

      //Case 3: npc is above and on the right of PC
      if(player->posX > npcPosX  && player->posY < npcPosY){
        distanceX = player->posX - npcPosX;
        distanceY= npcPosY - player->posY;
        //printw("%c, %d %s and %d %s\n", npcSymbol, distanceX, north, distanceY, east);
        snprintf(buffer[i], 200, "%c, %d %s and %d %s\n", npcSymbol, distanceX, north, distanceY, east);
      }

      //Case 4: npc is below and on the right of PC
      if(player->posX < npcPosX  && player->posY < npcPosY){
        distanceX = npcPosX - player->posX;
        distanceY = npcPosY - player->posY;
        //printw("%c, %d %s and %d %s\n", npcSymbol, distanceX, south, distanceY, east);
        snprintf(buffer[i], 200, "%c, %d %s and %d %s\n", npcSymbol, distanceX, south, distanceY, east);
      }

      //Case 5: npc is above but same row as PC
      if(player->posX > npcPosX && player->posY == npcPosY){
        distanceX = player->posX - npcPosX;
        // printw("%c, %d %s\n", npcSymbol, distanceX, north);
        snprintf(buffer[i], 200, "%c, %d %s\n", npcSymbol, distanceX, north);
      }

      //Case 6: npc is below but same row as PC
      if(player->posX < npcPosX && player->posY == npcPosY){
        distanceX = npcPosX - player->posX;
        //printw("%c, %d %s\n", npcSymbol, distanceX, south);
        snprintf(buffer[i], 200, "%c, %d %s\n", npcSymbol, distanceX, south);
      }
       
      //Case 7: npc is on the left but same column as PC
      if(player->posX == npcPosX && player->posY > npcPosY){
        distanceY = player->posY - npcPosY;
        // printw("%c, %d %s\n", npcSymbol, distanceY, west);
        snprintf(buffer[i], 200, "%c, %d %s\n", npcSymbol, distanceY, west);
      }

      //Case 8: npc is on the right but same column as PC
      if(player->posX == npcPosX && player->posY < npcPosY){
        distanceY = npcPosY - player->posY;
        // printw("%c, %d %s\n", npcSymbol, distanceY, east);
        snprintf(buffer[i], 200, "%c, %d %s\n", npcSymbol, distanceY, east);
      }
      
      if(trainerCount < 21){
        mvprintw(i, 0, "%s\n", buffer[i]);
      }
    }
    if(trainerCount < 21){
        mvprintw(map->trainers , 0, "%s", "");
        while(getch() != 27){
        ;
        } 
    }
    else{
      mvprintw(21,0,"%s","");
      scrollingUpAndDown(map->trainers, buffer);
    }
    
    free(buffer);
  }  
}
void assignNodesHelper(int officialTrainerCount, map_m *map, world_w *world, groupNPC *groupNPC, int mapX, int mapY){
  int currentMapX = world->mapXCoord;
  int currentMapY = world->mapYCoord;

 world->maps[mapX][mapY]->heap = init_minheap(trainerCount);
    
 world->maps[mapX][mapY]->npcNode = (struct Nodes*) malloc(trainerCount * sizeof(struct Nodes));
    
  
  //node_n nodes[officialTrainerCount],
  // map->rivalNodes = (struct Nodes**) malloc(21 * sizeof(struct Nodes*));
  //   for(i = 0; i < 21; i++){
  //     map->rivalNodes[i] = (struct Nodes*) malloc(80 * sizeof(struct Nodes));
  //   }
  // world->maps[mapX][mapY]->npcNode = (struct Nodes*) malloc(officialTrainerCount * sizeof(struct Nodes));

  for(int i = 0; i < map->trainers; i++){
    int randomDirection = rand() % (5 - 1) + 1;
    if(groupNPC->arrNPCs[i].npcType == 'p'){
         world->maps[mapX][mapY]->npcNode[i].direction = 1;
    }
    if(groupNPC->arrNPCs[i].npcType == 'e' || groupNPC->arrNPCs[i].npcType == 'w'){
         world->maps[mapX][mapY]->npcNode[i].direction = randomDirection;
    }
     world->maps[mapX][mapY]->npcNode[i].currX = groupNPC->arrNPCs[i].coordX;
     world->maps[mapX][mapY]->npcNode[i].currY = groupNPC->arrNPCs[i].coordY;
     world->maps[mapX][mapY]->npcNode[i].symbol = groupNPC->arrNPCs[i].npcType;
     world->maps[mapX][mapY]->npcNode[i].dist = 0;


    // printw("NPC coords: (%d,%d)\n", groupNPC->arrNPCs[i].coordX, groupNPC->arrNPCs[i].coordY);
    // printw("NPC symbols: %c\n", groupNPC->arrNPCs[i].npcType);
    insert_minheap(world->maps[mapX][mapY]->heap,  world->maps[mapX][mapY]->npcNode[i]);
  }
    
  
}

void npcNodesFunction(int officialTrainerCount, world_w *world, map_m *map, MinHeap *heap, groupNPC *groupNPC, player *player, char npcMap[ROWS][COLUMNS] ,  pokeDatabase *pokeData){
  int count = 0;
  char inputKey;
  int x,y;
  // MinHeap *npcHeap;
  // npcHeap = init_minheap(officialTrainerCount);
  int currentMapX = world->mapXCoord;
  int currentMapY = world->mapYCoord;
   int playerCurrentMapX = 200 - currentMapX;
  int playerCurrentMapY = 200 - currentMapY;
  // heap = init_minheap(officialTrainerCount);

  // world->maps[currentMapX][currentMapY]->npcNode = (struct Nodes*) malloc (officialTrainerCount * sizeof(struct Nodes));

  // // for(int i = 0; i < officialTrainerCount; i++){
  // //   int randomDirection = rand() % (5 - 1) + 1;
  // //   if(groupNPC->arrNPCs[i].npcType == 'p'){
  // //       nodes[i].direction = 1;
  // //   }
  // //   if(groupNPC->arrNPCs[i].npcType == 'e' || groupNPC->arrNPCs[i].npcType == 'w'){
  // //       nodes[i].direction = randomDirection;
  // //   }
  // //   nodes[i].currX = groupNPC->arrNPCs[i].coordX;
  // //   nodes[i].currY = groupNPC->arrNPCs[i].coordY;
  // //   nodes[i].symbol = groupNPC->arrNPCs[i].npcType;
  // //   nodes[i].dist = 0;

  // for(int i = 0; i < officialTrainerCount; i++){
  //   int randomDirection = rand() % (5 - 1) + 1;
  //   if(groupNPC->arrNPCs[i].npcType == 'p'){
  //        world->maps[currentMapX][currentMapY]->npcNode[i].direction = 1;
  //   }
  //   if(groupNPC->arrNPCs[i].npcType == 'e' || groupNPC->arrNPCs[i].npcType == 'w'){
  //        world->maps[currentMapX][currentMapY]->npcNode[i].direction = randomDirection;
  //   }
  //    world->maps[currentMapX][currentMapY]->npcNode[i].currX = groupNPC->arrNPCs[i].coordX;
  //    world->maps[currentMapX][currentMapY]->npcNode[i].currY = groupNPC->arrNPCs[i].coordY;
  //    world->maps[currentMapX][currentMapY]->npcNode[i].symbol = groupNPC->arrNPCs[i].npcType;
  //    world->maps[currentMapX][currentMapY]->npcNode[i].dist = 0;


  //   // printw("NPC coords: (%d,%d)\n", groupNPC->arrNPCs[i].coordX, groupNPC->arrNPCs[i].coordY);
  //   // printw("NPC symbols: %c\n", groupNPC->arrNPCs[i].npcType);
  //   insert_minheap(heap,  world->maps[currentMapX][currentMapY]->npcNode[i]);
  // }
  int distance = 1;
  int check = 1;
  int itemBag = 0;
 
  //assignNodesHelper(officialTrainerCount, world->maps[currentMapX][currentMapY], world, groupNPC, currentMapX, currentMapY);
  //world->maps[currentMapX][currentMapY]->trainers = trainerCount;
  while(1){
    pokemonData wildPokemon;
    count = 0;
    itemBag = 0;
  
    // assignNodesHelper(officialTrainerCount, map, heap, groupNPC);
    // npcHeap = assignNodesHelper(officialTrainerCount, map, heap, groupNPC);
    // printw("Heap size: %d\n", npcHeap->size);
  
    while(count < officialTrainerCount){
      //  printw("Heap size: %d\n", npcHeap->size);
      // world->maps[currentMapX][currentMapY]
      npcMovement(world->maps[currentMapX][currentMapY]->heap,world->maps[currentMapX][currentMapY],npcMap, count);
      count++;
      
    }
    while(inputKey == 'f'){
   
      clear();
      refresh();
      printw("Fly to?");
      scanw("%d %d", &x, &y);
      if(x >= 200 && y >= 200){
        distance = (x - 200) + (y - 200);
        totalDistance = distance;
      }
      if(x <= 200 && y <= 200){
        distance = (200 - x) + (200 - y);
        totalDistance = distance;
      }
      if(x >= 200 && y <= 200){
        distance = (x - 200) + (200 - y);
        totalDistance = distance;
      }
      if(x <= 200 && y >= 200){
        distance = (200 - x) + (y - 200);    
        totalDistance = distance;
      }
      if(x >= 199 && x <= 201 && y >= 199 && y <= 201){
        distance = 0;
        totalDistance = 2;
      }
      // playerCurrentMapX = x - 200;
      // playerCurrentMapY = y - 200;
      currentMapX = x;
      currentMapY = y;
      flycommand(world, x, y, npcMap, player, pokeData, distance);
      clear();
      break;
      // world->mapXCoord = x;
      // world->mapYCoord = y;

    }
    printMapstr(world->maps[currentMapX][currentMapY],npcMap);
    refresh();
    printw("Current Map X and Y: %d, %d\n", currentMapX, currentMapY);
    printw("playerX : %d, Y: %d\n", player->posX, player->posY);
    printw("distance: %d\n", distance);
    // printw("Vector size: %d",world->maps[currentMapX][currentMapY]->pokemonList->size());
    // printw("Map Exit: %d", map->exits[1]);
  
    inputKey = getch();
    while (inputKey != '7' && inputKey != 'y' && inputKey != '8' && inputKey != 'k' && inputKey != '9' && inputKey != 'u' && inputKey != '6' && inputKey != 'l' && inputKey != '3' && inputKey != 'n' && inputKey != '2' && inputKey != 'j' && inputKey != '1' && inputKey != 'b' && inputKey != '4' && inputKey != 'h' && inputKey != '5' && inputKey != ' ' && inputKey != '.' && inputKey != 62 && inputKey != 't' && inputKey != 'q' && inputKey != 'f' && inputKey != 'i' && inputKey != 'p') {
            inputKey = getch();
    }
    
    if(inputKey == 62){
      pokeCenterAndMart(npcMap, world->maps[currentMapX][currentMapY], player, inputKey);
    }
    
    if(inputKey == 116){
      getNPCpositions(officialTrainerCount, npcMap, world->maps[currentMapX][currentMapY], player, inputKey);
      // clear();
      // refresh();
      // printw("hi");
      // refresh();

      // //clear();
      // printw("%d\n", trainerCount);
      // refresh();
      // getch();
      // break;
    }
    if(inputKey == 'i'){
      itemBag = openBagWindow(player);
    }

    if(inputKey == 'p'){
      swapPokemonInTeam(player);
    }
    
     //clear();
    if(inputKey == 'q'){
      break;
    }
   
   
    // if(check == 2){
    //   currentMapY++;
    //   // fillHikerValue(world->maps[currentMapX][currentMapY], &playerP); 
    //   // fillRivalNodes(world->maps[currentMapX][currentMapY], player);
    //   // int newRivalCost[ROWS][COLUMNS];
    //   // // copyNPCHikerCost(world->maps[currentMapX][currentMapY], hikerCost);
    //   // copyNPCRivalCost(world->maps[currentMapX][currentMapY], newRivalCost);
    //   // spawnNPCreworked(officialTrainerCount, player, map, newRivalCost, npcMap, groupNPC);
      
    // }
    if(check == 1){
      for(int i = player->posX-1; i > 0 && i < ROWS - 1 && i < player->posX+2; i++){
        for(int j = player->posY-1; j > 0 && j < COLUMNS - 1 && j < player->posY+2; j++){
          if(npcMap[i][j] == 'p' || npcMap[i][j] == 'h' || npcMap[i][j] == 'e' || npcMap[i][j] == 'w' || npcMap[i][j] == 'r' || npcMap[i][j] == 's'){
            refresh();
            clear();
            // while(inputKey != 27){
              // vector<pokemonData> npcPokemon;
              printw("Battle Begins\n");
              // clear();
              getch();

              battleBegin(pokeData, player, groupNPC, world->maps[currentMapX][currentMapY]);


              // printPlayerPokemonInfo(player);
              // printNPCPokemon(world->maps[currentMapX][currentMapY], groupNPC);
              // refresh();
              // npcPokemon = map->pokemonList[0];
              // printw("Size: %d", map->pokemonList[0].size());
              // for(int i = 0; i < map->pokemonList[0].size(); i++){
              //   printw("Pokemon name: %s", map->pokemonList[0].name.c_str());
              // }
              // inputKey = getch();
              // break;
            // }
            npcMap[i][j] = ' ';
            for(int k = 0; k < officialTrainerCount; k++){
              if(world->maps[currentMapX][currentMapY]->heap->arrNodes[k].currX == i && world->maps[currentMapX][currentMapY]->heap->arrNodes[k].currY == j){
                delete_element(world->maps[currentMapX][currentMapY]->heap,k);
                //officialTrainerCount--;
                world->maps[currentMapX][currentMapY]->trainers--;
                break;
              }
            }
          }
        }
      }
    }
    check = playerMovement(world, npcMap, world->maps[currentMapX][currentMapY], player, inputKey, pokeData);
    
    if(world->maps[currentMapX][currentMapY]->pointerArr[player->posX][player->posY] == ':'){
        int chance = chanceOfMeetingPokemon();

        if(chance == 1){
          wildPokemon = spawnPokemon(world->maps[currentMapX][currentMapY],pokeData);
          wildPokemonBattle(pokeData, player, wildPokemon);
        }
    }
    if(check == 2){
      
      currentMapY++;
      //assignNodesHelper(trainerCount, world->maps[currentMapX][currentMapY], world, groupNPC, currentMapX, currentMapY);
      
      // printw("playerX : %d, Y: %d\n", player->posX, player->posY);
      
    }
    if(check == 3){
      currentMapY--;
    }
    if(check == 4){
      currentMapX--;
    }
    if(check == 5){
      currentMapX++;
    }

    
    //printMapstr(world->maps[currentMapX][currentMapY], npcMap);
    
    refresh();

    clear();

  }
  endwin();
  //node_n tempNode = get_min(heap);
  // while(1){
  
}
  // while(count < trainerCount){t
void updateNPCmap(int posX, int posY, int newPosX, int newPosY, char npcMap[ROWS][COLUMNS]){
  npcMap[newPosX][newPosY] = npcMap[posX][posY];
  npcMap[posX][posY] = ' ';
}
    
void npcMovement(MinHeap *heap, map_m *map, char npcMap[ROWS][COLUMNS], int count){
    //count != trainerCount
    //tempNode.currX != player->posX || tempNode.currY != player->posY)
    
    node_n tempNode = get_min(heap);
    //printw("npcSymbol: %c\n", tempNode.symbol);
   //printw("npcDist: %d\n", tempNode.dist);
    delete_minimum(heap);
    int previousHikerNodeX = map->hikerNodes[tempNode.currX][tempNode.currY].previousX;
    int previousHikerNodeY = map->hikerNodes[tempNode.currX][tempNode.currY].previousY;
    int previousRivalNodeX = map->rivalNodes[tempNode.currX][tempNode.currY].previousX;
    int previousRivalNodeY = map->rivalNodes[tempNode.currX][tempNode.currY].previousY;

    if(tempNode.symbol == 'r'){
      tempNode.dist += map->rivalNodes[tempNode.currX][tempNode.currY].cost;
        if(npcMap[previousRivalNodeX][previousRivalNodeY] == ' '){
          npcMap[previousRivalNodeX][previousRivalNodeY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX = previousRivalNodeX;
          tempNode.currY = previousRivalNodeY;
          // map->npcNode[count].currX = previousRivalNodeX;
          // map->npcNode[count].currY = previousRivalNodeY;
          tempNode.previousX = map->rivalNodes[tempNode.currX][tempNode.currY].previousX;
          tempNode.previousY = map->rivalNodes[tempNode.currX][tempNode.currY].previousY;
        }
      
    
    }
    if(tempNode.symbol == 'h'){
      tempNode.dist += map->hikerNodes[previousHikerNodeX][previousHikerNodeY].cost;
      if(npcMap[previousHikerNodeX][previousHikerNodeY] == ' '){
        npcMap[previousHikerNodeX][previousHikerNodeY] = npcMap[tempNode.currX][tempNode.currY];
        npcMap[tempNode.currX][tempNode.currY] = ' ';
        tempNode.currX = previousHikerNodeX;
        tempNode.currY = previousHikerNodeY;
        // map->npcNode[count].currX = previousHikerNodeX;
        // map->npcNode[count].currY = previousHikerNodeY;
        tempNode.previousX = map->hikerNodes[tempNode.currX][tempNode.currY].previousX;
        tempNode.previousY = map->hikerNodes[tempNode.currX][tempNode.currY].previousY;
      }

      
    }
    while(tempNode.symbol == 'e'){
      tempNode.dist += map->rivalNodes[tempNode.currX][tempNode.currY].cost;
      if(tempNode.direction == 1 && (tempNode.currX + 1) < (ROWS - 1) && map->pointerArr[tempNode.currX + 1][tempNode.currY] != '%' && map->pointerArr[tempNode.currX + 1][tempNode.currY] != '^'){
        if(npcMap[tempNode.currX + 1][tempNode.currY] == ' '){
          npcMap[tempNode.currX + 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX++;
          // map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 2 && (tempNode.currX - 1) > 0 && map->pointerArr[tempNode.currX - 1][tempNode.currY] != '%' && map->pointerArr[tempNode.currX - 1][tempNode.currY] != '^'){
        if(npcMap[tempNode.currX - 1][tempNode.currY] == ' '){
          npcMap[tempNode.currX - 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX--;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 3 && (tempNode.currY + 1) < (COLUMNS - 1) && map->pointerArr[tempNode.currX][tempNode.currY + 1] != '%' && map->pointerArr[tempNode.currX][tempNode.currY + 1] != '^'){
        if(npcMap[tempNode.currX][tempNode.currY + 1] == ' '){
          npcMap[tempNode.currX][tempNode.currY + 1] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currY++;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 4 && (tempNode.currY - 1) > 0 && map->pointerArr[tempNode.currX][tempNode.currY - 1] != '%' && map->pointerArr[tempNode.currX][tempNode.currY - 1] != '^'){
        if(npcMap[tempNode.currX][tempNode.currY - 1] == ' '){
          npcMap[tempNode.currX][tempNode.currY - 1] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currY--;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      else{
        tempNode.direction = (rand() % 5) + 1;
        continue;
      }
       //printw("e moved\n");
    }

    while(tempNode.symbol == 'w'){
      tempNode.dist += map->rivalNodes[tempNode.currX][tempNode.currY].cost;
      if(tempNode.direction == 1 && (tempNode.currX + 1) < (ROWS - 1) && map->pointerArr[tempNode.currX + 1][tempNode.currY] != '%' && map->pointerArr[tempNode.currX + 1][tempNode.currY] != '^' && map->pointerArr[tempNode.currX + 1][tempNode.currY] == map->pointerArr[tempNode.currX][tempNode.currY]){
        if(npcMap[tempNode.currX + 1][tempNode.currY] == ' '){
          npcMap[tempNode.currX + 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX++;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 2 && (tempNode.currX - 1) > 0 && map->pointerArr[tempNode.currX - 1][tempNode.currY] != '%' && map->pointerArr[tempNode.currX - 1][tempNode.currY] != '^' && map->pointerArr[tempNode.currX - 1][tempNode.currY] == map->pointerArr[tempNode.currX][tempNode.currY]){
        if(npcMap[tempNode.currX - 1][tempNode.currY] == ' '){
          npcMap[tempNode.currX - 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX--;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 3 && (tempNode.currY + 1) < (COLUMNS - 1) && map->pointerArr[tempNode.currX][tempNode.currY + 1] != '%' && map->pointerArr[tempNode.currX][tempNode.currY + 1] != '^' && map->pointerArr[tempNode.currX][tempNode.currY + 1] == map->pointerArr[tempNode.currX][tempNode.currY]){
        if(npcMap[tempNode.currX][tempNode.currY + 1] == ' '){
          npcMap[tempNode.currX][tempNode.currY + 1] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currY++;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      if(tempNode.direction == 4 && (tempNode.currY - 1) > 0 && map->pointerArr[tempNode.currX][tempNode.currY - 1] != '%' && map->pointerArr[tempNode.currX][tempNode.currY - 1] != '^' && map->pointerArr[tempNode.currX][tempNode.currY - 1] == map->pointerArr[tempNode.currX][tempNode.currY]){
        if(npcMap[tempNode.currX][tempNode.currY - 1] == ' '){
          npcMap[tempNode.currX][tempNode.currY - 1] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currY--;
          //  map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        break;
      }
      else{
        tempNode.direction = (rand() % 5) + 1;
        continue;
      }
      
     //printw("W moved\n");
    }
    

    if(tempNode.symbol == 'p'){
      tempNode.dist += map->rivalNodes[tempNode.currX][tempNode.currY].cost;
      if(tempNode.direction == 1){
        if((tempNode.currX + 1) < (ROWS - 1) && map->rivalNodes[tempNode.currX + 1][tempNode.currY].cost != INFINITY && npcMap[tempNode.currX+1][tempNode.currY] == ' '){
            npcMap[tempNode.currX + 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
            npcMap[tempNode.currX][tempNode.currY] = ' ';
            tempNode.currX++;
             map->npcNode[count].currX = tempNode.currX;
            map->npcNode[count].currY = tempNode.currY;
        }
        else{
          if(npcMap[tempNode.currX - 1][tempNode.currY] == ' '){
            tempNode.direction = 2;
             
            
        
          }
        }
      }
      else{
        if((tempNode.currX - 1) > 0 && map->rivalNodes[tempNode.currX - 1][tempNode.currY].cost != INFINITY && npcMap[tempNode.currX-1][tempNode.currY] == ' '){
          npcMap[tempNode.currX - 1][tempNode.currY] = npcMap[tempNode.currX][tempNode.currY];
          npcMap[tempNode.currX][tempNode.currY] = ' ';
          tempNode.currX--;
          // map->npcNode[count].currX = tempNode.currX;
          // map->npcNode[count].currY = tempNode.currY;
        }
        else{
          if(npcMap[tempNode.currX + 1][tempNode.currY] == ' '){
            tempNode.direction = 1;
          
          }
        }
      }
      
      //printw("p moved\n");
    } 
    if(tempNode.symbol == 's'){
      tempNode.dist = INT_MAX;
    }

  
    // count++;
    
    insert_minheap(heap, tempNode);
   
   
  }
    // printMapstr(map,npcMap);
  


void fillRivalNodes(map_m *map, player *player){
  int i,j;
  MinHeap *heap;
  char symbol;
  int totalDistance;

  map->rivalNodes = (struct Nodes**) malloc(21 * sizeof(struct Nodes*));
    for(i = 0; i < 21; i++){
      map->rivalNodes[i] = (struct Nodes*) malloc(80 * sizeof(struct Nodes));
    }

  heap = init_minheap(21 * 80);

  for(i = 0; i < 21; i++){
    for(j = 0; j < 80; j++){
      symbol = map->pointerArr[i][j];
      if( symbol == '#' || symbol == '.' ){
          map->rivalNodes[i][j].cost = 10;
      }
      if(symbol == ':'){
          map->rivalNodes[i][j].cost = 20;
      }
      if(symbol == '^' || symbol == '%'){
          map->rivalNodes[i][j].cost = INFINITY;
      }
      if(symbol == 'C' || symbol == 'M'){
          map->rivalNodes[i][j].cost = 50;
      }
      map->rivalNodes[i][j].currX = i;
      map->rivalNodes[i][j].currY = j;
      map->rivalNodes[i][j].dist = INFINITY;
      

      if(i == player->posX && j == player->posY){
        map->rivalNodes[i][j].dist = 0;
        map->rivalNodes[i][j].cost = 10;

        //debug
        //printf("Coordinate of @ is (%d, %d)\n", i, j);

         //if node value not filled could be this.
      }
      insert_minheap(heap, map->rivalNodes[i][j]);

    }
  }
  while(heap->size != 0){
    // printf("%d", initialNode.cost);
    node_n tempNode = get_min(heap);
    // printf("current node is (%d, %d)\n", tempNode.currX, tempNode.currY);
    // printf("Heap size is %d\n", heap->size);
    // printf("First node's value is %d\n" , heap->arrrivalNodes[0].cost);
    // printf("Node distance is %d\n", tempNode.dist);
    // printf("Node's coordinates is (%d, %d)\n", tempNode.currX, tempNode.currY);
    for(i = (tempNode.currX) - 1; i < (tempNode.currX) + 2; i++){
      if(i < 0 || i > 20){
        continue;
      }
      for(j = (tempNode.currY) - 1; j < (tempNode.currY) + 2; j++){
         symbol = map->pointerArr[i][j];
        if(j < 0 || j > 79){
          continue;
        }
        if(symbol == '%' || symbol == '^'){
            continue;
        }
        if(map->rivalNodes[i][j].dist > tempNode.dist + tempNode.cost){
          totalDistance = tempNode.dist + tempNode.cost;
          decreaseKey(heap,map->rivalNodes[i][j].currX, map->rivalNodes[i][j].currY, tempNode.dist + tempNode.cost);
          map->rivalNodes[i][j].dist= totalDistance;      
          map->rivalNodes[i][j].previousX = tempNode.currX; 
          map->rivalNodes[i][j].previousY = tempNode.currY;
        }
      }

  
    }
  delete_minimum(heap);
  }
  //printMapRivalInt(map);
}

/*
Uses dijkstra's algorithm to fill the values of the shortest path it takes for
a hiker npc to reach the player.
*/
void fillHikerValue(map_m *map, player *player){
  int i,j;
  MinHeap *heap;
  char symbol;
  int totalDistance;

  map->hikerNodes = (struct Nodes**) malloc(21 * sizeof(struct Nodes*));
    for(i = 0; i < 21; i++){
      map->hikerNodes[i] = (struct Nodes*) malloc(80 * sizeof(struct Nodes));
    }
  heap = init_minheap(21 * 80);

  for(i = 0; i < 21; i++){
    for(j = 0; j < 80; j++){
      symbol = map->pointerArr[i][j];

      if( symbol == '#' || symbol == '.' || symbol == '^'){
          map->hikerNodes[i][j].cost = 10;
      }
      if(symbol == ':' || symbol == '^' || symbol == '%'){
          map->hikerNodes[i][j].cost = 15;
      }
      if(symbol == 'C' || symbol == 'M'){
          map->hikerNodes[i][j].cost = 50;
      }
      map->hikerNodes[i][j].currX = i;
      map->hikerNodes[i][j].currY = j;
      map->hikerNodes[i][j].dist = INFINITY;
      

      if(i == player->posX && j == player->posY){
        map->hikerNodes[i][j].dist = 0;
        map->hikerNodes[i][j].cost = 10;

        //debug
       // printw("Coordinate of @ is (%d, %d)\n", i, j);
        // printw("nodeX previous value: %d", map->hikerNodes[i][j].cost);
         //if node value not filled could be this.
      }
      insert_minheap(heap, map->hikerNodes[i][j]);

    }
  }
  while(heap->size != 0){
    // printf("%d", initialNode.cost);
    node_n tempNode = get_min(heap);
    // printf("current node is (%d, %d)\n", tempNode.currX, tempNode.currY);
    // printf("Heap size is %d\n", heap->size);
    // printf("First node's value is %d\n" , heap->arrhikerNodes[0].cost);
    // printf("Node distance is %d\n", tempNode.dist);
    // printf("Node's coordinates is (%d, %d)\n", tempNode.currX, tempNode.currY);
    for(i = (tempNode.currX) - 1; i < (tempNode.currX) + 2; i++){
      if(i < 0 || i > 20){
        continue;
      }
      for(j = (tempNode.currY) - 1; j < (tempNode.currY) + 2; j++){
        if(j < 0 || j > 79){
          continue;
        }
        if(map->hikerNodes[i][j].dist > tempNode.dist + tempNode.cost){
          totalDistance = tempNode.dist + tempNode.cost;
          decreaseKey(heap,map->hikerNodes[i][j].currX, map->hikerNodes[i][j].currY, tempNode.dist + tempNode.cost);
          map->hikerNodes[i][j].dist= totalDistance;
          map->hikerNodes[i][j].previousX = tempNode.currX; 
          map->hikerNodes[i][j].previousY = tempNode.currY;
        }
      }


    }
  delete_minimum(heap);
  }
  //printMapHikerInt(map);
}


/*
Defines a struct World which consists of an array of map pointers. It also stores the current map's 
x-coordinate and y-coordinate.
*/



/*
Create a world where all the maps in it are initially NULL.
*/
void createWorld(world_w *world){
    int i,j;

    for(i = 0; i < 401; i++){
        for(j = 0; j < 401; j++){
             world->maps[i][j]= NULL;
        }
    }
}


/*
A function that generates a map inside of the world.
*/
void newMap(world_w *world){
    char t[ROWS][COLUMNS];
    int exitCoordinates[8];
    
    fillBorder(t);
    randomizeSeed(t);
    randomExits(exitCoordinates,t);
    checkSurrounding(world, exitCoordinates);
    fillExit(t, exitCoordinates);
    fillPath(t, exitCoordinates);
    
   // spawnNPCreworked(numTrainers, t, npcMap);
    //spawnNPCatRandom(t, numTrainers);

    closeBorder(world,t,exitCoordinates);
    copyTerrain(world->maps[world->mapXCoord][world->mapYCoord],t);
    exitCoords(world->maps[world->mapXCoord][world->mapYCoord], exitCoordinates);
}



/*
Generate the npc map
*/
// void generateHikerMap(world_w *world){
//     int t[ROWS-2][COLUMNS-2];
//     fillValues(t);
//     copyNPC(world->maps[world->mapXCoord][world->mapYCoord],t);
// }

/*
Initialize current map at coordinate.
*/

void initializeCurrentMap(world_w *world, int x, int y){
    world->maps[x][y] = (struct Map*) malloc(sizeof(struct Map));
    allocateMapSpace(world->maps[x][y]);
    world->mapXCoord = x;
    world->mapYCoord = y;
}


/*
Check the surrounding map if it has been discovered before to fill the exits of a map accordingly.
*/
void checkSurrounding(world_w *world, int exitCoordinates[8]){
 
  int xCoord = world->mapXCoord;
  int yCoord = world->mapYCoord;


  if(xCoord-1 >= 0){
    if(xCoord >= 0 && xCoord < 401 && yCoord >= 0 && yCoord <401 && world->maps[xCoord-1][yCoord] != NULL){
      exitCoordinates[4] = world->maps[xCoord-1][yCoord]->exits[6];
    }
  }

  if(yCoord+1 <= 400){
    if(xCoord >= 0 && xCoord < 401 && yCoord >= 0 && yCoord <401 && world->maps[xCoord][yCoord+1] != NULL){

     exitCoordinates[1] = world->maps[xCoord][yCoord+1]->exits[3];
    }
  }

  if(xCoord+1 <= 400){

    if(xCoord >= 0 && xCoord < 401 && yCoord >= 0 && yCoord <401 && world->maps[xCoord+1][yCoord] != NULL){
      exitCoordinates[6] = world->maps[xCoord+1][yCoord]->exits[4];
    }
  }
  
  if(yCoord-1 >= 0){
    if(xCoord >= 0 && xCoord < 401 && yCoord >= 0 && yCoord <401 && world->maps[xCoord][yCoord-1] != NULL){
      exitCoordinates[3] = world->maps[xCoord][yCoord-1]->exits[1];
    }
  }
}

/*
This function is used to fill the exits with boulders, '%' once the user displays a map that is at the
end of the world.
*/

void closeBorder(world_w *world, char table[21][80], int exitCoordinates[8]){
  int xCoord = world->mapXCoord;
  int yCoord = world->mapYCoord;


  if(xCoord == 400){
    table[exitCoordinates[6]][exitCoordinates[7]] = '%';
  }
  if(xCoord == 0){
    table[exitCoordinates[4]][exitCoordinates[5]] = '%';
  }
  if(yCoord == 0){
    table[exitCoordinates[2]][exitCoordinates[3]] = '%';
  }
  if(yCoord == 400){
    table[exitCoordinates[0]][exitCoordinates[1]] = '%';
  }
}


/*
Fly to desired map based on coordinates given.
*/

void flycommand(world_w *world, int x, int y, char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData, int distance){
  int xCoord = world->mapXCoord;
  int yCoord = world->mapYCoord;
  // int distance = 0;
  groupNPC groupNPC;
  
  // if ( x < 200 && y < 200){
  //   distance = (200 - x) + (200 - y);
  // }
  // if(checkOutofBounds(x + 200) == 0 || checkOutofBounds(y + 200) == 0){
  //   xCoord = 200 + x;
  //   yCoord = 200 + y;
  // }

  // if(checkOutofBounds(xCoord) == 1){
  //   printw("Out of bounds\n");
  // } 
  //   else{
  //     world->mapXCoord = xCoord;
  //   }

  // if(checkOutofBounds(yCoord) == 1){
  //   printw("Out of bounds\n");
  // }
  //   else{
  //     world->mapYCoord = yCoord;
  //   }
  //  if(x >= 200 && y >= 200){
  //   distance = (x - 200) + (y - 200);
  //   totalDistance = distance;
  // }
  // if(x <= 200 && y <= 200){
  //   distance = (200 - x) + (200 - y);
  //   totalDistance = distance;
  // }
  // if(x >= 200 && y <= 200){
  //   distance = (x - 200) + (200 - y);
  //   totalDistance = distance;
  // }
  // if(x <= 200 && y >= 200){
  //   distance = (200 - x) + (y - 200);    
  //   totalDistance = distance;
  // }
  // if(x >= 199 && y <= 201 && x >= 199 && y <= 201){
  //   distance = 0;
  //   totalDistance = 2;
  // }

  int currentMapX = world->mapXCoord;
  int currentMapY = world->mapYCoord;

  if(world->maps[x][y] != NULL){
  
    // initializeCurrentMap(world, x , y);
    // assignMapCoor(world->maps[x][y], x, y);
    // newMap(world);
    // fillHikerValue(world->maps[x][y], player); 
    // fillRivalNodes(world->maps[x][y], player);
    // spawnNPCreworked(trainerCount,player, world->maps[x][y], t, &groupNPC);
    // randomSpawnValue(world->maps[x][y], player);
    // putPlayerOnOverlay(player, t);
    // assignNodesHelper(trainerCount, world->maps[x][y], world, &groupNPC, x, y);
    // world->maps[x][y]->trainers = trainerCount;
    // assignMapCoor(world->maps[x][y], x, y);
    restoreNPC(world->maps[x][y], t);
    randomSpawnValue(world->maps[x][y], player);
    putPlayerOnOverlay(player, t);
    world->mapXCoord = x;
    world->mapYCoord = y;
   }

  else{
    // assignMapCoor(world->maps[x][y], x, y);
    // restoreNPC(world->maps[x][y], t);
    // randomSpawnValue(world->maps[x][y], player);
    // putPlayerOnOverlay(player, t);
    initializeCurrentMap(world, x , y);
    assignMapCoor(world->maps[x][y], x, y);
    newMap(world);
    fillHikerValue(world->maps[x][y], player); 
    fillRivalNodes(world->maps[x][y], player);
    spawnNPCreworked(trainerCount,player, world->maps[x][y], t, &groupNPC, pokeData, distance);
    randomSpawnValue(world->maps[x][y], player);
    putPlayerOnOverlay(player, t);
    assignNodesHelper(trainerCount, world->maps[x][y], world, &groupNPC, x, y);
    // world->maps[x][y]->trainers = trainerCount;
    world->mapXCoord = x;
    world->mapYCoord = y;
  }
}

/*
Checks if the mapCoordinates reaches out of bounds.
*/
int checkOutofBounds(int mapCoordinate){
  if(mapCoordinate < 0 || mapCoordinate > 400){
    return 1;
  }
  return 0;
}

/*
Displays the map above the current map
*/

void exitUp(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData){

  int xCoord = world->mapXCoord;
  int yCoord = world->mapYCoord;
  int distance = 0;
  char npcMap[ROWS][COLUMNS];
  groupNPC groupNPC;
  // node_n npcNodes[trainerCount];
  MinHeap heap_h;
  //  if(xCoord >= 200 && yCoord >= 200){
  //   distance = (xCoord - 200) + (yCoord - 200);
  //   totalDistance = distance;
  // }
  // if(xCoord <= 200 && yCoord <= 200){
  //   distance = (200 - xCoord) + (200 - yCoord);
  //   totalDistance = distance;
  // }
  // if(xCoord >= 200 && yCoord <= 200){
  //   distance = (xCoord - 200) + (200 - yCoord);
  //   totalDistance = distance;
  // }
  // if(xCoord <= 200 && yCoord >= 200){
  //   distance = (200 - xCoord) + (yCoord - 200);    
  //   totalDistance = distance;
  // }
  // if(xCoord >= 199 && xCoord <= 201 && yCoord >= 199 && yCoord <= 201){
  //   distance = 0;
  //   totalDistance = 2;
  // }

  if(world->maps[world->mapXCoord][world->mapYCoord+1] != NULL){
    assignMapCoor(world->maps[xCoord][yCoord+1], xCoord, yCoord+1);
    //spawnNPCreworked(trainerCount,player, world->maps[xCoord][yCoord+1], newRivalCost, t, &groupNPC);
    restoreNPC(world->maps[xCoord][yCoord+1], t);
    putPlayerOnOverlay(player, t);
    //assignNodesHelper(trainerCount, world->maps[xCoord][yCoord+1], world, &groupNPC, xCoord, yCoord+1);
    // printMapstr(world->maps[world->mapXCoord][world->mapYCoord+1], t);
     world->mapYCoord++;
  }
  else{
    initializeCurrentMap(world, xCoord, yCoord+1);
    assignMapCoor(world->maps[xCoord][yCoord+1], xCoord, yCoord+1);
    newMap(world);
    fillHikerValue(world->maps[xCoord][yCoord+1], player); 
    fillRivalNodes(world->maps[xCoord][yCoord+1], player);
    spawnNPCreworked(trainerCount,player, world->maps[xCoord][yCoord+1], t, &groupNPC, pokeData, totalDistance);
    putPlayerOnOverlay(player, t);
    assignNodesHelper(trainerCount, world->maps[xCoord][yCoord+1], world, &groupNPC, xCoord, yCoord+1);
    // world->maps[xCoord][yCoord+1]->trainers = trainerCount;
    // npcMovement(&heap_h, world->maps[xCoord][yCoord+1], t);
    // printMapstr(world->maps[xCoord][yCoord+1], t);
    //world->mapYCoord++;
  }
}

/*
Displays the map below the current map
*/

void exitDown(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData){
    int xCoord = world->mapXCoord;
    int yCoord = world->mapYCoord;

  char npcMap[ROWS][COLUMNS];
  groupNPC groupNPC;
  // node_n npcNodes[trainerCount];
  MinHeap heap_h;

  if(world->maps[world->mapXCoord][world->mapYCoord-1] != NULL){
    assignMapCoor(world->maps[xCoord][yCoord-1], xCoord, yCoord-1);
    //spawnNPCreworked(trainerCount,player, world->maps[xCoord][yCoord-1], rivalCost, t, &groupNPC);
    restoreNPC(world->maps[xCoord][yCoord-1], t);
    putPlayerOnOverlay(player, t);
    //assignNodesHelper(trainerCount, world->maps[xCoord][yCoord-1], world, &groupNPC, xCoord, yCoord-1);
    
    // printMapstr(world->maps[world->mapXCoord][world->mapYCoord-1], t);
    world->mapYCoord--;
  }
  else{
   
    initializeCurrentMap(world, xCoord, yCoord-1);
    assignMapCoor(world->maps[xCoord][yCoord-1], xCoord, yCoord-1);
    newMap(world);
    fillHikerValue(world->maps[xCoord][yCoord-1], player); 
    fillRivalNodes(world->maps[xCoord][yCoord-1], player);
    spawnNPCreworked(trainerCount,player, world->maps[xCoord][yCoord-1], t, &groupNPC, pokeData, totalDistance);
    putPlayerOnOverlay(player, t);
    assignNodesHelper(trainerCount, world->maps[xCoord][yCoord-1], world, &groupNPC, xCoord, yCoord-1);
    // world->maps[xCoord][yCoord-1]->trainers = trainerCount;
    // printMapstr(world->maps[xCoord][yCoord-1], t);
    //world->mapYCoord--;
  }
}

/*
Displays the map left of the current map
*/
void exitLeft(world_w *world, char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData){
    int xCoord = world->mapXCoord;
    int yCoord = world->mapYCoord;
  char npcMap[ROWS][COLUMNS];
  groupNPC groupNPC;
  // node_n npcNodes[trainerCount];
  MinHeap heap_h;
  
  if(world->maps[world->mapXCoord-1][world->mapYCoord] != NULL){
    assignMapCoor(world->maps[xCoord-1][yCoord], xCoord-1, yCoord);
    restoreNPC(world->maps[xCoord-1][yCoord], t);
    putPlayerOnOverlay(player, t);
    //printMapstr(world->maps[world->mapXCoord-1][world->mapYCoord], t);
    world->mapXCoord--;
  }
  else{
   
    initializeCurrentMap(world, xCoord-1, yCoord);
    assignMapCoor(world->maps[xCoord-1][yCoord], xCoord-1, yCoord);
    newMap(world);
     fillHikerValue(world->maps[xCoord-1][yCoord], player); 
    fillRivalNodes(world->maps[xCoord-1][yCoord], player);
    spawnNPCreworked(trainerCount,player, world->maps[xCoord-1][yCoord], t, &groupNPC, pokeData, totalDistance);
    putPlayerOnOverlay(player, t);
    assignNodesHelper(trainerCount, world->maps[xCoord-1][yCoord], world, &groupNPC, xCoord-1, yCoord);
    // world->maps[xCoord-1][yCoord]->trainers = trainerCount;
    //printMapstr(world->maps[xCoord-1][yCoord], t);
    // world->mapXCoord--;
  }
}

/*
Displays the map right of the current map
*/
void exitRight(world_w *world,  char t[ROWS][COLUMNS], player *player, pokeDatabase * pokeData){
    int xCoord = world->mapXCoord;
    int yCoord = world->mapYCoord;
  char npcMap[ROWS][COLUMNS];
  groupNPC groupNPC;
  // node_n npcNodes[trainerCount];
  MinHeap heap_h;
  if(world->maps[world->mapXCoord+1][world->mapYCoord] != NULL){
    assignMapCoor(world->maps[xCoord+1][yCoord], xCoord+1, yCoord);
    restoreNPC(world->maps[xCoord+1][yCoord], t);
    putPlayerOnOverlay(player, t);
    //printMapstr(world->maps[world->mapXCoord+1][world->mapYCoord], t);
    world->mapXCoord++;
  }
  else{
   
    initializeCurrentMap(world, xCoord+1, yCoord);
    assignMapCoor(world->maps[xCoord+1][yCoord], xCoord+1, yCoord);
    newMap(world);
    fillHikerValue(world->maps[xCoord+1][yCoord], player); 
    fillRivalNodes(world->maps[xCoord+1][yCoord], player);
    spawnNPCreworked(trainerCount,player, world->maps[xCoord+1][yCoord],t, &groupNPC, pokeData, totalDistance);
    putPlayerOnOverlay(player, t);
    assignNodesHelper(trainerCount, world->maps[xCoord+1][yCoord], world, &groupNPC, xCoord+1, yCoord);
    // world->maps[xCoord+1][yCoord]->trainers = trainerCount;
    //printMapstr(world->maps[xCoord+1][yCoord], t);
    // world->mapXCoord++;
  }
}

void init_terminal(void){
  initscr();
  //raw();
  //noecho();
  keypad(stdscr, TRUE);
  start_color();
  
}


/*
Distance calculation 
|x_1 - x_2 | + | y_1 - y_2| 

So if map at 200, 200 , if we go to 201,201 then distance will be 2;
If at 200,200 and go up then it will be 1.
*/

pokemonData spawnPokemon(map_m * map, pokeDatabase * pokeData){
  //Pokemon Stats
  int hp;
  int atk;
  int def;
  int spd;
  int specialAtk;
  int specialDef;
  
  //IVs
  int hpIV;
  int attackIV;
  int defenseIV;
  int speedIV;
  int specialAtkIV;
  int specialDefIV;
  
  //Spawn pokemon, moves, lvl
  string pokeSpawned;
  int minLvl;
  int pokemonID;
  int pokemonMoveID1;
  int pokemonMoveID2;
  string pokemonMoveName1;
  string pokemonMoveName2;
  int pokemonLvl;
  int maxLvl;
  vector <pokemonMoves> specificPokemonMove;
  int randomMoveSelect;
  int randomPokemon = (rand() % 1092) + 0;
  int shinyChance = chanceOfMeetingShinyPokemon();
  int count = 0;
  int input;
  vector<pokeStats> specificPokemonStats;

  pokemonData wildPokemon;

  Pokemon pokemonSpawned;

   if(totalDistance > 200){
    minLvl = (totalDistance - 200) / 2;
    maxLvl = 100;
    pokemonLvl = minLvl + rand() % (maxLvl - minLvl + 1);
  }
  if(totalDistance <= 200){
    minLvl = 1;
    maxLvl = totalDistance / 2;
    pokemonLvl = minLvl + rand() % (maxLvl - minLvl + 1);
  }
  
  pokemonSpawned = pokeData->pokemons[randomPokemon];
  pokemonID = pokemonSpawned.id;
  pokeSpawned = pokemonSpawned.pokemon_name;
  
  while(1){
    for(int i = 0; i < pokeData->pokeMoves.size(); i++){
      if(pokeData->pokeMoves[i].getPokemonMoveId() == pokemonID && pokeData->pokeMoves[i].getPokeMoveMethodId() == 1 && pokeData->pokeMoves[i].getLevel() <= pokemonLvl){
          specificPokemonMove.push_back(pokeData->pokeMoves[i]);
      }
    }
    if(specificPokemonMove.empty()){
      pokemonLvl++;
      continue;
    }
    else{
      break;
    }
  }



  
  if(specificPokemonMove.size() == 1){
    randomMoveSelect = rand() % specificPokemonMove.size();
    pokemonMoveID1 = specificPokemonMove[randomMoveSelect].getMoveId();
    pokemonMoveName1 = pokeData->moves[pokemonMoveID1].getPokeMove();
  }
  else{
    randomMoveSelect = rand() % specificPokemonMove.size();
    pokemonMoveID1 = specificPokemonMove[randomMoveSelect].getMoveId();

    randomMoveSelect = rand() % specificPokemonMove.size();
    pokemonMoveID2 = specificPokemonMove[randomMoveSelect].getMoveId();

    
    pokemonMoveName1 = pokeData->moves[pokemonMoveID1].getPokeMove();
    pokemonMoveName2 = pokeData->moves[pokemonMoveID2].getPokeMove();
  }
  
  wildPokemon.name = pokeSpawned;
  wildPokemon.level = pokemonLvl;
  wildPokemon.id = pokemonID;
  wildPokemon.move1 = pokemonMoveName1;
  wildPokemon.move2 = pokemonMoveName2;
  wildPokemon.priority1 = pokeData->moves[pokemonMoveID1].getPriority();
  wildPokemon.priority2 = pokeData->moves[pokemonMoveID2].getPriority();
  wildPokemon.power1 = pokeData->moves[pokemonMoveID1].getPower();
  wildPokemon.power2 = pokeData->moves[pokemonMoveID2].getPower();
  wildPokemon.aliveStatus = 1;
  

  hpIV = randomIVgenerator();
  attackIV = randomIVgenerator();
  defenseIV = randomIVgenerator();
  speedIV = randomIVgenerator();
  specialAtkIV = randomIVgenerator();
  specialDefIV = randomIVgenerator(); 

  for(int i = 0; i < pokeData->allPokeStats.size(); i++){
    if(pokeData->allPokeStats[i].getPokeID() == pokemonID){
      specificPokemonStats.push_back(pokeData->allPokeStats[i]);
    } 
  }
  
  wildPokemon.hp = ((((specificPokemonStats[count].getBaseStat() + hpIV) * 2) * pokemonLvl) / 100) + pokemonLvl + 10;
  wildPokemon.mainHp = wildPokemon.hp;
  count++;

  wildPokemon.attack = ((((specificPokemonStats[count].getBaseStat() + attackIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  wildPokemon.defense = ((((specificPokemonStats[count].getBaseStat() + defenseIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  wildPokemon.speed =  ((((specificPokemonStats[count].getBaseStat() + speedIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  wildPokemon.speAtk = ((((specificPokemonStats[count].getBaseStat() + specialAtkIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  wildPokemon.speDef = ((((specificPokemonStats[count].getBaseStat() + specialDefIV) * 2) * pokemonLvl) / 100) + 5;




  refresh();
  clear();

  printw("Pokemon encounter!");
  getch();
  
  refresh();
  clear();
  // if(shinyChance == 1){
  //   printw("You encountered a SHINY %s!\n", pokeSpawned.c_str());
  //   printw("Pokemon Level: %d\n", pokemonLvl);  
  //   printw("Moves: %s , %s \n", pokemonMoveName1.c_str(), pokemonMoveName2.c_str()); 
  //   printw("Hp: %d\nAttack: %d\nDefense: %d\nSpeed: %d\nSpecial Atk: %d\nSpecial Defense:%d\n", wildPokemon.hp, wildPokemon.attack, wildPokemon.defense, wildPokemon.speed, wildPokemon.speAtk, wildPokemon.speDef); 
  //   getch();
  // }
  // else{
  //   printw("You encountered a %s\n", pokeSpawned.c_str());
  //   printw("Pokemon Level: %d\n", pokemonLvl);  
  //   printw("Moves: %s , %s \n", pokemonMoveName1.c_str(), pokemonMoveName2.c_str()); 
  //   printw("Hp: %d\nAttack: %d\nDefense: %d\nSpeed: %d\nSpecial Atk: %d\nSpecial Defense:%d\n", wildPokemon.hp, wildPokemon.attack, wildPokemon.defense, wildPokemon.speed, wildPokemon.speAtk, wildPokemon.speDef); 
  //   getch();
  // }
  
  return wildPokemon;
}


//Start of the game giving the player to choose their pokemon
void pokemonPrompt(player *player, pokeDatabase * pokeData){
  int randomPokemon = (rand() % 1092) + 0; 
  int pokemonID;
  int input;
  vector<pokemonData> pokemonToChoose;
  string pokeName;
  string pokemon1;
  string pokemon2;
  string pokemon3;

  pokemonToChoose.push_back(generatePokemon(pokeData, totalDistance));
  pokemonToChoose.push_back(generatePokemon(pokeData, totalDistance));
  pokemonToChoose.push_back(generatePokemon(pokeData, totalDistance));
  printw("Size of player pokemon: %d\n", player->playerPokemons.size());
  printw("Choose a pokemon: %s, %s, or %s by pressing 1, 2, or 3" , pokemonToChoose[0].name.c_str(), pokemonToChoose[1].name.c_str(), pokemonToChoose[2].name.c_str());
  // player->playerPokemons.push_back(pokemonToChoose[0]);
    input = getch();
  while(input != '1' || input != '2' || input != '3'){
    if(input == '1'){
      player->playerPokemons.push_back(pokemonToChoose[0]);
      player->numberOfPokemon++;
      clear();
      printw("You chose %s!\n", pokemonToChoose[0].name.c_str());
      printw("Press any button to continue.");
      refresh();
      break;
    }
    if(input == '2'){
      player->playerPokemons.push_back(pokemonToChoose[1]);
      player->numberOfPokemon++;
      clear();
      printw("You chose %s!\n", pokemonToChoose[1].name.c_str());
      printw("Press any button to continue.");
       refresh();
        break;
    }
    if(input == '3'){
      player->playerPokemons.push_back(pokemonToChoose[2]);
      player->numberOfPokemon++;
      clear();
      printw("You chose %s\n", pokemonToChoose[2].name.c_str());
      printw("Press any button to continue.");
       refresh();
        break;
    }
    clear();
    printw("Follow the instructions again o-o\n");
    printw("Choose a pokemon: %s, %s, or %s by pressing 1, 2, or 3" , pokemonToChoose[0].name.c_str(), pokemonToChoose[1].name.c_str(), pokemonToChoose[2].name.c_str());
    refresh();
    input = getch();
    
  }
  
  input = getch();
  clear();
  //  printw("Size of player pokemon: %d\n", player->playerPokemons.size());
  // printw("Check pokemon %s!", player->playerPokemons[0].name.c_str());
  refresh();
}

void printWildPokemonData(pokemonData wildPokemon){
  printw("\nWild Pokemon Name: %s\n", wildPokemon.name.c_str());
  printw("Level: %d\n", wildPokemon.level);
  printw("Moves: %s , %s \n", wildPokemon.move1.c_str(), wildPokemon.move2.c_str());
  printw("Hp: %d\nAttack: %d\nDefense: %d\nSpeed: %d\nSpecial Atk: %d\nSpecial Defense:%d\n", wildPokemon.hp, wildPokemon.attack, wildPokemon.defense, wildPokemon.speed, wildPokemon.speAtk, wildPokemon.speDef); 
}

void wildPokemonBattle(pokeDatabase * pokeData, player *player, pokemonData wildPokemon){
  int turn = 0;
  int escapeAttempt = 1;
  char inputKey;

  if(player->numberOfPokemon == 0){
    printw("You can't battle go to Poke Center to heal your pokemon");
    getch();
  }

  while(player->numberOfPokemon > 0 && wildPokemon.aliveStatus != 0 && turn == 0){
    turn = 0;
    clear();
    printPlayerPokemonInfo(player);
    printWildPokemonData(wildPokemon);
    optionSelecter();
    refresh();
    inputKey = getch();
    if(inputKey == '1'){
      turn = fightWildPokemon(pokeData, player, wildPokemon);
      if(turn == 1){
        break;
      }
    } 
    if(inputKey == '2'){
      turn = wildPokemonBagWindow(player, wildPokemon);
      // if(turn == 1){

      // }
      if(turn == 2){
        break;
      }
    }
    if(inputKey == '3'){
      turn = runFromPokemon(player, wildPokemon, escapeAttempt);
      if(turn == 1){
        printw("You managed to escape!");
        getch();
        break;
      }
    }
    if(inputKey == '4'){
      turn = switchPokemon(player);
    }
    turn = 0;
    clear();
    refresh();
  }
}
int checkWildPokemonMoveType(pokeDatabase *pokeData, pokemonData wildPokemon){
  vector<int> typeIDs;
  for(int i = 0; i < pokeData->pokeTypes.size(); i++){
    if(pokeData->pokeTypes[i].pokemon_id == wildPokemon.id){
      typeIDs.push_back(pokeData->pokeTypes[i].type_id);
    }
  }

  for(int i = 0; i < pokeData->moves.size(); i++){
    if(wildPokemon.move1.c_str() == pokeData->moves[i].getPokeMove()){

      for(int j = 0; j < typeIDs.size(); j++){
        if(typeIDs[j] = pokeData->moves[i].type_id){
          return 1.5;
        }
      }
    }
  }
  return 1;
}

int fightWildPokemon(pokeDatabase * pokeData, player * player, pokemonData wildPokemon){
  char inputKey;
  clear();
  printw("What move would you like to use? Press 1 or 2 accordingly, other buttons to go back\n");
  printw("Move 1: %s\n",  player->playerPokemons[playerPokemon].move1.c_str());
  printw("Move 2: %s\n",  player->playerPokemons[playerPokemon].move2.c_str());
  refresh();

  inputKey = getch();

  int randomChance = rand() % 2 + 1;
  int randomNumber = rand() % 100 + 85;
  int STABpc;
  int STABnpc;
  
  STABpc = checkPokemonMoveTypePlayer(pokeData, player);
  STABnpc = checkWildPokemonMoveType(pokeData, wildPokemon);

  int chanceToMiss = rand() % 100;
  int pcMove1Acc = checkFirstMoveAccuracy(pokeData, player);
  int pcMove2Acc = checkSecondMoveAccuracy(pokeData, player);
  // int npcMove1Acc = checkFirstMoveAccuracyNPC(pokeData, groupNPC, map);
  // int npcMove2Acc = checkSecondMoveAccuracyNPC(pokeData, groupNPC, map);

  int turn;

  if(inputKey == '1'){

    if(player->playerPokemons[playerPokemon].priority1 < wildPokemon.priority1){
      player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){
                  wildPokemon.aliveStatus = 0;
                  printw("\nYou won!\n");
                  getch();
                  return 1;
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
    }



    if(player->playerPokemons[playerPokemon].priority1 > wildPokemon.priority1){
      wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

      if(wildPokemon.hp > 0){
        player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

        if(player->playerPokemons[playerPokemon].hp <= 0){
          player->numberOfPokemon--;
          if(player->numberOfPokemon == 0){
            printw("\nYou lost..\n");
            getch();
          }
          else{
            printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
            player->playerPokemons[playerPokemon].aliveStatus = 0;
            getch();
            turn = switchPokemon(player);
          }
        }
      }


      if(wildPokemon.hp <= 0){
       wildPokemon.aliveStatus = 0;
          printw("\nYou won!\n");
          getch();
          return 1;
        
      }
    }
    else if(player->playerPokemons[playerPokemon].priority1 == wildPokemon.priority1){
        int turnCount = 0;
      if(player->playerPokemons[playerPokemon].speed > wildPokemon.speed && turnCount == 0){
        wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

        if(wildPokemon.hp > 0){
          player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

          if(player->playerPokemons[playerPokemon].hp <= 0){
            player->numberOfPokemon--;
            if(player->numberOfPokemon == 0){
              turnCount = 1;
              printw("\nYou lost..\n");
              getch();
            }
            else{
              printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
              player->playerPokemons[playerPokemon].aliveStatus = 0;
              getch();
              turn = switchPokemon(player);
            }
          }
        }

        if(wildPokemon.hp <= 0){
          wildPokemon.aliveStatus = 0;
            turnCount = 1;
            printw("\nYou won!\n");
            getch();
            return 1;
        }
      }

      if(player->playerPokemons[playerPokemon].speed < wildPokemon.speed && turnCount == 0){
       player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){     
                wildPokemon.aliveStatus = 0;
                  turnCount = 1;     
                  printw("\nYou won!\n");
                  getch();
                return 1;
                
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
      }
    
    //Might be problem here cause npc pokemon not defeated sometimes.
     if(player->playerPokemons[playerPokemon].speed == wildPokemon.speed && turnCount == 0){
      
        if(randomChance == 1){
          // printw("testChance1");
          // getch();
          wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

           if(wildPokemon.hp > 0){
            player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

            if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
          }

          if(wildPokemon.hp <= 0){  
            wildPokemon.aliveStatus = 0;
              turnCount = 1;
              printw("\nYou won!\n");
              getch();
              return 1;
           
          }
        }
        else if(randomChance == 2){
          //Works
          // printw("testChance2");
          // getch();
           player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power1 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){
                wildPokemon.aliveStatus = 0;
                  turnCount = 1;
                  printw("\nYou won!\n");
                    getch();  
                    return 1;
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
        }
      }
    }
  }


   if(inputKey == '2'){

    if(player->playerPokemons[playerPokemon].priority2 < wildPokemon.priority2){
      player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){
                wildPokemon.aliveStatus = 0;
                  printw("\nYou won!\n");
                  getch();
                  return 1;
               
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
    }



    if(player->playerPokemons[playerPokemon].priority2 > wildPokemon.priority2){
      wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

      if(wildPokemon.hp > 0){
        player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

        if(player->playerPokemons[playerPokemon].hp <= 0){
          player->numberOfPokemon--;
          if(player->numberOfPokemon == 0){
            printw("\nYou lost..\n");
            getch();
          }
          else{
            printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
            player->playerPokemons[playerPokemon].aliveStatus = 0;
            getch();
            turn = switchPokemon(player);
          }
        }
      }


      if(wildPokemon.hp <= 0){
        wildPokemon.aliveStatus = 0;
          printw("\nYou won!\n");
          getch();
          return 1;
      
      }
    }
    else if(player->playerPokemons[playerPokemon].priority2 == wildPokemon.priority2){
      // printw("testEqPrio");
          getch();
          int turnCount = 0;
      if(player->playerPokemons[playerPokemon].speed > wildPokemon.speed && turnCount == 0){
        wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

        if(wildPokemon.hp > 0){
          player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

          if(player->playerPokemons[playerPokemon].hp <= 0){
            player->numberOfPokemon--;
            if(player->numberOfPokemon == 0){
              turnCount = 1;
              printw("\nYou lost..\n");
              getch();
            }
            else{
               printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
               player->playerPokemons[playerPokemon].aliveStatus = 0;
               getch();
               turn = switchPokemon(player);
            }
          }
        }

        if(wildPokemon.hp <= 0){
       wildPokemon.aliveStatus = 0;
            turnCount = 1;
            printw("\nYou won!\n");
            getch();
            return 1;
        }
      }

      if(player->playerPokemons[playerPokemon].speed < wildPokemon.speed && turnCount == 0){
       player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){
                wildPokemon.aliveStatus = 0;
                  turnCount = 1;
                  printw("\nYou won!\n");
                  getch();
                  return 1;
                
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
      }
    
    //Might be problem here cause npc pokemon not defeated sometimes.
     if(player->playerPokemons[playerPokemon].speed == wildPokemon.speed && turnCount == 0){
      
        if(randomChance == 1){
          // printw("testChance1");
          // getch();
          wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

           if(wildPokemon.hp > 0){
            player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

            if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
          }

          if(wildPokemon.hp <= 0){
            wildPokemon.aliveStatus = 0;
              turnCount = 1;
              printw("\nYou won!\n");
              getch();
              return 1;
            
          }
        }
        else if(randomChance == 2){
          //Works
          // printw("testChance2");
          // getch();
           player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * wildPokemon.level) / 5) + 2) *  wildPokemon.power2 * ( wildPokemon.attack /  wildPokemon.defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              wildPokemon.hp = (wildPokemon.hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(wildPokemon.hp <= 0){
                wildPokemon.aliveStatus = 0;
                  turnCount = 1;
                  printw("\nYou won!\n");
                  getch();
                  return 1;
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                  player->playerPokemons[playerPokemon].aliveStatus = 0;
                  getch();
                  turn = switchPokemon(player);
              }
            }
        }
      }
    }
  }  
  return 0;
}

int wildPokemonBagWindow(player* player, pokemonData wildPokemon){
  char inputKey;
  int checkPokemonHealed = 0;
  int catchRate = 0;

  clear();
  printw("These are your items in your bag, use them by pressing 1, 2 or 3 accordingly. Press any other button to go back.\n");
  printw("Potions : %d\n", player->potions);
  printw("Revives : %d\n", player->revives);
  printw("Pokeballs: %d\n", player->pokeballs);
  refresh();

  
  inputKey = getch();
  if(inputKey == '1'){
    if(player->potions == 0){
      printw("You have no more potions");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
      clear();
      printw("\nWho do you want to heal?\n");
      pokemonInterface(player);
      printw("Choose the pokemon by pressing from 1-6\n");
      refresh();
      inputKey = getch();
      
      //Still need to think about blocking potion option if person press '2' and we only have 1 pokemon.
      if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = potionOnPokemon(player, inputKey);
          // return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
      
    }
    return 0;
  }
  
  if(inputKey == '2'){
    if(player->revives == 0){
      printw("You have no more revives");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
    clear();
    printw("\nWho do you want to revive?\n");
    pokemonInterface(player);
    printw("Choose the pokemon by pressing from 1-6\n");
    refresh();
    inputKey = getch();

    if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = revivePokemon(player, inputKey);
          // return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
    }
    return 0;
  }

  if(inputKey == '3'){
    if(player->pokeballs == 0){
      printw("You don't have any pokeballs");
      return 0;
    }
    if(player->playerPokemons.size() < 6){
     
      catchRate = pokemonCatchRate();
      player->pokeballs--;
      if(catchRate == 0){
        printw("Catch failed, try again.");
        getch();
        return 0;
      }
      else if(catchRate == 1){
        player->playerPokemons.push_back(wildPokemon);
        player->numberOfPokemon++;
        printw("Catch success!");
        getch();
        return 2;
      }
      
    }
    if(player->playerPokemons.size() >= 6){
     
      catchRate = pokemonCatchRate();
      player->pokeballs--;
      if(catchRate == 0){
        printw("Catch failed, try again.");
        getch();
        return 0;
      }
      else if(catchRate == 1){
        player->computerPokemons.push_back(wildPokemon);
        player->numberOfPokemon++;
        printw("The pokemon is added to the Computer in the Poke Center.");
        printw("Access your new pokemon at the Poke Center.");
        getch();
        return 2;
      }
     
    }
    else{
      printw("Wild pokemon fled");
      getch();
      return 2;
    }
    
  }
  return 0;
}

int pokemonCatchRate(){

  int randomNumber = rand() % 255;
  int number = rand() % 10 + 1;

  if(number > 5){
    if(randomNumber > 150){
      return 1;
    }
  }
  
  return 0;
}

int openBagWindow(player *player){

  char inputKey;
  int checkPokemonHealed = 0;
  clear();
  printw("These are your items in your bag, use them by pressing 1, 2 or 3 accordingly. Press any other button to go back.\n");
  printw("Potions : %d\n", player->potions);
  printw("Revives : %d\n", player->revives);
  printw("Pokeballs: %d\n", player->pokeballs);
  refresh();

  inputKey = getch();
  if(inputKey == '1'){
    if(player->potions == 0){
      printw("You have no more potions");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
      clear();
      printw("\nWho do you want to heal?\n");
      pokemonInterface(player);
      printw("Choose the pokemon by pressing from 1-6\n");
      refresh();
      inputKey = getch();
      
      //Still need to think about blocking potion option if person press '2' and we only have 1 pokemon.
      if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = potionOnPokemon(player, inputKey);
          return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
      
    }
    return 0;
  }
  
  if(inputKey == '2'){
    if(player->revives == 0){
      printw("You have no more revives");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
    clear();
    printw("\nWho do you want to revive?\n");
    pokemonInterface(player);
    printw("Choose the pokemon by pressing from 1-6\n");
    refresh();
    inputKey = getch();

    if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = revivePokemon(player, inputKey);
          // return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
    }
    return 0;
  }

  if(inputKey == '3'){
    printw("This is just a pokeball to catch pokemon");
    getch();
    return 0;
  }
  return 0;

}

int runFromPokemon(player * player, pokemonData wildPokemon, int escapeAttempt){

  int attempts = escapeAttempt;
  int escapeOdd;
  int randomOdd = rand() % 256;

  escapeOdd = ( (player->playerPokemons[playerPokemon].speed * 32) / ((wildPokemon.speed / 4) % 256) ) + 30 * attempts;

  if(randomOdd < escapeOdd){
    return 1;
  }
  printw("You failed, try again.");
  getch();
  return 0;
}

void printPlayerPokemonInfo(player *player){
  printw("Player pokemon:\n");
  printw("Pokemon name: %s\n", player->playerPokemons[playerPokemon].name.c_str());
  printw("HP: %d\n", player->playerPokemons[playerPokemon].hp);
  printw("Attack: %d\n", player->playerPokemons[playerPokemon].attack);
  printw("Defense: %d\n", player->playerPokemons[playerPokemon].defense);
  printw("Speed: %d\n", player->playerPokemons[playerPokemon].speed);
  printw("Level: %d\n", player->playerPokemons[playerPokemon].level);
  printw("Move 1: %s\n",  player->playerPokemons[playerPokemon].move1.c_str());
  printw("Priority1 : %d\n",player->playerPokemons[playerPokemon].priority1);
  printw("Power1 : %d\n",player->playerPokemons[playerPokemon].power1);
  printw("Move 2: %s\n",  player->playerPokemons[playerPokemon].move2.c_str());
  printw("Priority2 : %d\n",player->playerPokemons[playerPokemon].priority2);
  printw("Power2 : %d\n",player->playerPokemons[playerPokemon].power2);
  printw("Number of pokemon player has: %d\n", player->numberOfPokemon);
  
  // printw()
  // printw()
  // printw()
  // printw()
  // printw()
}

void printNPCPokemon(map_m *map, groupNPC *groupNPC){
  
  

  printw("\nNPC pokemon:\n");
  printw("Pokemon name: %s\n", groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].name.c_str());
  printw("HP: %d\n", groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp);
  printw("Level: %d\n", groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level);
  printw("Move 1: %s\n",  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].move1.c_str());
  printw("Power1 : %d\n", groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1);
  printw("Move 2: %s\n",  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].move2.c_str());
  printw("Power2 : %d\n", groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2);

  printw("Number of pokemon NPC has: %d\n", groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon);
 }

void optionSelecter(){
  printw("\nWhat do you want to do?\n");
  printw("Press 1 to fight\n");
  printw("Press 2 to use bag\n");
  printw("Press 3 to run\n");
  printw("Press 4 to go to Pokemons\n");
}

void battleBegin(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map){
  char inputKey;
  int turn = 0;
  int npcAtk = 0;
  // int npcPokemon = 0; //pokemon NPC is currently using
  // int playerPokemon = 0;  //pokemon player is currently using

  if(player->numberOfPokemon == 0){
    printw("You can't battle go to Poke Center to heal your pokemon");
    getch();
  }

  while(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon > 0 && player->numberOfPokemon > 0 && turn == 0){
    turn = 0;
    npcAtk = 0;
    clear();
    printPlayerPokemonInfo(player);
    printNPCPokemon(map, groupNPC);
    optionSelecter();
    refresh();
    inputKey = getch();

    if(inputKey == '1'){
      fightOption(pokeData, player, groupNPC, map);
      // if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon < 1 || player->playerPokemons.size() < 1){
      //   break;
      // }
    }

    if(inputKey == '2'){
    
      
        turn = bagWindow(player);
        if(turn == 1){
          //Need test still after done with other parts like if heal pokemon or revive, then npc attack your pokemon.
          npcAtk = simulateNPCPokemonAttack(pokeData, player, groupNPC, map);
          if(npcAtk == 1){
            break;
          }
        }
    }

    if(inputKey == '3'){
      printw("You can't run away from a battle with a trainer!");
      getch();
    }

    if(inputKey == '4'){
      turn = switchPokemon(player);
      if(turn == 1){
        
        npcAtk = simulateNPCPokemonAttack(pokeData, player, groupNPC, map);
        if(npcAtk == 1){
          break;
        }
      }
    }
    turn = 0;
    clear();
    refresh();
  }

  npcPokemon = 0;
  
  playerPokemon = checkAlivePokemon(player);
  //need to fix like after losing then the pokemon that comes out must be alive.
  //can use for loop, add a alive status, loop through vector, check which pokemon alive, then call
  //the first alive pokemon we see. 
}
int checkAlivePokemon(player *player){
  for(int i = 0; i < player->playerPokemons.size(); i++){
    if(player->playerPokemons[i].aliveStatus == 1){
      return i;
    }
  }
  return 0;
}

int switchPokemon(player *player){
  char inputKey;
  int currentPokemon;
  int index = 0;
  int turn = 0;
  while(turn == 0){

  
  clear();
  printw("\nWhich pokemon do you want to switch with?\n");
  pokemonInterface(player);
  printw("Choose the pokemon you want to switch by pressing from 1-6\n");
  refresh();
  inputKey = getch();

  if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6' ){
    //   if(player->playerPokemons[playerPokemon].aliveStatus == 0){
    //     printw("You can't switch a fainted pokemon!");
    //     turn = 0;
    //    getch();
    //  }
     
     
     currentPokemon = playerPokemon;
     
     index = inputKey - 49;
     if(index + 1 > player->playerPokemons.size()){
      printw("You only have %d pokemon.", player->playerPokemons.size());
      turn = 0;
      getch();
      return 0;
    }
     if(player->playerPokemons[index].aliveStatus == 1 && currentPokemon != index){
      printw("Come back %s!\n", player->playerPokemons[currentPokemon].name.c_str());
      printw("Go %s!\n", player->playerPokemons[index].name.c_str());
      playerPokemon = index;
      turn = 1;
      getch();
      // return 1;
     }
     else if(currentPokemon == index && player->playerPokemons[index].aliveStatus == 1){
      printw("This pokemon is already in battle!");
      turn = 0;
       getch();
       return 0;
     }
     else{
      printw("You can't switch a fainted pokemon!");
      turn = 0;
       getch();
      //  return 0;
     }
  }
  else{
    if(player->playerPokemons[index].aliveStatus == 0){
      printw("You have to switch to another pokemon");
      turn = 0;
      getch();
    }
    else{
      turn = 1;
      return 0;
    }
    
  }
}
  return 1;
 
  // if(inputKey == '1'){

  // }

}

int bagWindow(player *player){

  char inputKey;
  int checkPokemonHealed = 0;
  
  clear();
  printw("These are your items in your bag, use them by pressing 1, 2 or 3 accordingly. Press any other button to go back.\n");
  printw("Potions : %d\n", player->potions);
  printw("Revives : %d\n", player->revives);
  printw("Pokeballs: %d\n", player->pokeballs);
  refresh();

  
  inputKey = getch();
  if(inputKey == '1'){
    if(player->potions == 0){
      printw("You have no more potions");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
      clear();
      printw("\nWho do you want to heal?\n");
      pokemonInterface(player);
      printw("Choose the pokemon by pressing from 1-6\n");
      refresh();
      inputKey = getch();
      
      //Still need to think about blocking potion option if person press '2' and we only have 1 pokemon.
      if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = potionOnPokemon(player, inputKey);
          // return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
      
    }
    return 0;
  }
  
  if(inputKey == '2'){
    if(player->revives == 0){
      printw("You have no more revives");
      getch();
      return 0;
    }
    while(checkPokemonHealed != 1){
    clear();
    printw("\nWho do you want to revive?\n");
    pokemonInterface(player);
    printw("Choose the pokemon by pressing from 1-6\n");
    refresh();
    inputKey = getch();

    if(inputKey == '1' || inputKey == '2' || inputKey == '3' || inputKey == '4' || inputKey == '5' || inputKey == '6'){
          checkPokemonHealed = revivePokemon(player, inputKey);
          // return 1;
      }
      if(checkPokemonHealed == 0){
        return 0;
      }
       if(checkPokemonHealed == 1){
        return 1;
      }
    }
    return 0;
  }

  if(inputKey == '3'){
    printw("You can't capture the NPC's pokemon!");
    getch();
    return 0;
  }
  return 0;
}

int simulateNPCPokemonAttack(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map){
  int turn = 0;
  int randomNumber = rand() % 100 + 85;
  int STABnpc;
   STABnpc = checkPokemonMoveTypeNPC(pokeData, groupNPC, map);
   player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;


           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                printw("\nYou lost..\n");
                getch();
                return 1;
              } 
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
                return 0;
              }
            }
      return 0;
}

int revivePokemon(player *player, char input){
  int index = input - 49; 

  if(index + 1 > player->playerPokemons.size()){
    printw("You only have %d pokemon.", player->playerPokemons.size());
    getch();
    return 0;
  }

  if(player->playerPokemons[index].mainHp == player->playerPokemons[index].hp){
      printw("This pokemon has full health.");
      getch();
      return 0;
    }
  
  else if(player->playerPokemons[index].hp <= 0){
    player->playerPokemons[index].hp = player->playerPokemons[index].mainHp / 2;
    player->revives--;
    player->numberOfPokemon++;
    player->playerPokemons[index].aliveStatus = 1;
    printw("You revived your pokemon!");
    getch();
    return 1;
  }
  else{
    printw("This pokemon is still alive.");
    getch();
    return 0;
  }
  return 0;
}

int potionOnPokemon(player *player, char input){

int index = input - 49;  
if(index + 1 > player->playerPokemons.size()){
    printw("You only have %d pokemons.", player->playerPokemons.size());
    getch();
    return 0;
  }
// printw("index: %d", index);
   if(player->playerPokemons[index].mainHp == player->playerPokemons[index].hp){
      printw("This pokemon has full health.");
      getch();
      return 0;
    }
  else if(player->playerPokemons[index].mainHp <= 20 && player->playerPokemons[index].hp > 0){
      player->playerPokemons[index].hp = player->playerPokemons[index].mainHp;
      player->potions--;
      printw("Your pokemon health is fully restored!");
      getch();
      return 1;
    }
    else if(player->playerPokemons[index].mainHp > 20 && player->playerPokemons[index].hp > 0){
      player->playerPokemons[index].hp += 20;
      player->potions--;
      printw("Your pokemon gained back 20 hp!");
      getch();
      return 1;
    }
    
    else{
      printw("You can't use a potion on a dead pokemon!");
      getch();
      return 0;
    }
    return 0;
}


void pokemonInterface(player* player){

  printw("Pokemon on your team:\n") ;
  
  for(int i = 0; i < player->playerPokemons.size(); i++){
    printw("Pokemon %d: %s, Level: %d, Hp: %d\n", i+1, player->playerPokemons[i].name.c_str(), player->playerPokemons[i].level, player->playerPokemons[i].hp);
  }

}



void fightOption(pokeDatabase * pokeData, player *player, groupNPC *groupNPC, map_m *map){
  char inputKey;
  clear();
  printw("What move would you like to use? Press 1 or 2 accordingly, other buttons to go back\n");
  printw("Move 1: %s\n",  player->playerPokemons[playerPokemon].move1.c_str());
  printw("Move 2: %s\n",  player->playerPokemons[playerPokemon].move2.c_str());
  refresh();
  
  inputKey = getch();
  int npcMove = rand() % 1;
  int randomChance = rand() % 2 + 1;
  int randomNumber = rand() % 100 + 85;
  int STABpc;
  int STABnpc;
  
  STABpc = checkPokemonMoveTypePlayer(pokeData, player);
  STABnpc = checkPokemonMoveTypeNPC(pokeData, groupNPC, map);

  int chanceToMiss = rand() % 100;
  int pcMove1Acc = checkFirstMoveAccuracy(pokeData, player);
  int pcMove2Acc = checkSecondMoveAccuracy(pokeData, player);
  int npcMove1Acc = checkFirstMoveAccuracyNPC(pokeData, groupNPC, map);
  int npcMove2Acc = checkSecondMoveAccuracyNPC(pokeData, groupNPC, map);

  int npcTotalPokemon = groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon;
  int pokeBucksChance = rand() % 200 + 50;
  int moneyEarned;

  int turn;

  if(inputKey == '1'){

    if(player->playerPokemons[playerPokemon].priority1 < groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority1){
      player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
                  player->pokeBucks += pokeBucksChance * npcTotalPokemon;
                  printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
    }



    if(player->playerPokemons[playerPokemon].priority1 > groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority1){
      groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

      if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
        player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

        if(player->playerPokemons[playerPokemon].hp <= 0){
          player->numberOfPokemon--;
          if(player->numberOfPokemon == 0){
            printw("\nYou lost..\n");
            getch();
          }
          else{
            printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
            player->playerPokemons[playerPokemon].aliveStatus = 0;
            getch();
            turn = switchPokemon(player);
          }
        }
      }


      if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
        groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
        if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
          printw("\nYou won!\n");
          moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
          getch();
        }
        else{
          npcPokemon++;
          printw("Npc brought out a new pokemon!");
          getch();
        }
      }
    }
    else if(player->playerPokemons[playerPokemon].priority1 == groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority1){
      // printw("testEqPrio");
      //     getch();
        int turnCount = 0;
      if(player->playerPokemons[playerPokemon].speed > groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
        groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

        if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
          player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

          if(player->playerPokemons[playerPokemon].hp <= 0){
            player->numberOfPokemon--;
            if(player->numberOfPokemon == 0){
              turnCount = 1;
              printw("\nYou lost..\n");
              getch();
            }
            else{
              printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
              player->playerPokemons[playerPokemon].aliveStatus = 0;
              getch();
              turn = switchPokemon(player);
            }
          }
        }

        if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
          groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
          if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
            turnCount = 1;
            printw("\nYou won!\n");
            moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
            getch();
          }
          else{
            npcPokemon++;
            turnCount = 1;
            printw("Npc brought out a new pokemon!");
            getch();
          }
        }
      }

      if(player->playerPokemons[playerPokemon].speed < groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
       player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  turnCount = 1;
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  turnCount = 1;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
      }
    
    //Might be problem here cause npc pokemon not defeated sometimes.
     if(player->playerPokemons[playerPokemon].speed == groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
      
        if(randomChance == 1){
          // printw("testChance1");
          // getch();
          groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

           if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
            player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

            if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
          }

          if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
            groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
            if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
              turnCount = 1;
              printw("\nYou won!\n");
              moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
              getch();
            }
            else{
              npcPokemon++;
              turnCount = 1;
              printw("Npc brought out a new pokemon!");
              getch();
            }
          }
        }
        else if(randomChance == 2){
          //Works
          // printw("testChance2");
          // getch();
           player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power1 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power1 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  turnCount = 1;
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  turnCount = 1;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
        }
      }
    }
  }


   if(inputKey == '2'){

    if(player->playerPokemons[playerPokemon].priority2 < groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority2){
      player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                printw("\nYou lost..\n");
                getch();
              }
              else{
                printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
    }



    if(player->playerPokemons[playerPokemon].priority2 > groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority2){
      groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

      if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
        player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

        if(player->playerPokemons[playerPokemon].hp <= 0){
          player->numberOfPokemon--;
          if(player->numberOfPokemon == 0){
            printw("\nYou lost..\n");
            getch();
          }
          else{
            printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
            player->playerPokemons[playerPokemon].aliveStatus = 0;
            getch();
            turn = switchPokemon(player);
          }
        }
      }


      if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
        groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
        if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
          printw("\nYou won!\n");
          moneyEarned = pokeBucksChance * npcTotalPokemon;
          player->pokeBucks += pokeBucksChance * npcTotalPokemon;
          printw("You earned %d!", moneyEarned);
          getch();
        }
        else{
          npcPokemon++;
          printw("Npc brought out a new pokemon!");
          getch();
        }
      }
    }
    else if(player->playerPokemons[playerPokemon].priority2 == groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].priority2){
      // printw("testEqPrio");
      //     getch();
          int turnCount = 0;
      if(player->playerPokemons[playerPokemon].speed > groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
        groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

        if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
          player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

          if(player->playerPokemons[playerPokemon].hp <= 0){
            player->numberOfPokemon--;
            if(player->numberOfPokemon == 0){
              turnCount = 1;
              printw("\nYou lost..\n");
              getch();
            }
            else{
               printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
               player->playerPokemons[playerPokemon].aliveStatus = 0;
               getch();
               turn = switchPokemon(player);
            }
          }
        }

        if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
          groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
          if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
            turnCount = 1;
            printw("\nYou won!\n");
            moneyEarned = pokeBucksChance * npcTotalPokemon;
            player->pokeBucks += pokeBucksChance * npcTotalPokemon;
             printw("You earned %d!", moneyEarned);
            getch();
          }
          else{
            npcPokemon++;
            turnCount = 1;
            printw("Npc brought out a new pokemon!");
            getch();
          }
        }
      }

      if(player->playerPokemons[playerPokemon].speed < groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
       player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  turnCount = 1;
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
                  player->pokeBucks += pokeBucksChance * npcTotalPokemon;
                  printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  turnCount = 1;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
      }
    
    //Might be problem here cause npc pokemon not defeated sometimes.
     if(player->playerPokemons[playerPokemon].speed == groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].speed && turnCount == 0){
      
        if(randomChance == 1){
          // printw("testChance1");
          // getch();
          groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

           if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp > 0){
            player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

            if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                player->playerPokemons[playerPokemon].aliveStatus = 0;
                getch();
                turn = switchPokemon(player);
              }
            }
          }

          if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
            groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
            if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
              turnCount = 1;
              printw("\nYou won!\n");
              moneyEarned = pokeBucksChance * npcTotalPokemon;
              player->pokeBucks += pokeBucksChance * npcTotalPokemon;
              printw("You earned %d!", moneyEarned);
              getch();
            }
            else{
              npcPokemon++;
              turnCount = 1;
              printw("Npc brought out a new pokemon!");
              getch();
            }
          }
        }
        else if(randomChance == 2){
          //Works
          // printw("testChance2");
          // getch();
           player->playerPokemons[playerPokemon].hp = player->playerPokemons[playerPokemon].hp  - ((((((2 * groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].level) / 5) + 2) *  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].power2 * ( groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].attack /  groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABnpc * 1;

           if(player->playerPokemons[playerPokemon].hp > 0){
              groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp = (groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp) - ((((((2 * player->playerPokemons[playerPokemon].level) / 5) + 2) * player->playerPokemons[playerPokemon].power2 * (player->playerPokemons[playerPokemon].attack / player->playerPokemons[playerPokemon].defense)) / 100) + 2) * critChance() * randomNumber * STABpc * 1;

              if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].hp <= 0){
                groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon--;
                if(groupNPC->arrNPCs[(map->trainers) - 1].numberofPokemon == 0){
                  turnCount = 1;
                  printw("\nYou won!\n");
                  moneyEarned = pokeBucksChance * npcTotalPokemon;
                  player->pokeBucks += pokeBucksChance * npcTotalPokemon;
                  printw("You earned %d!", moneyEarned);
                  getch();
                }
                else{
                  npcPokemon++;
                  turnCount = 1;
                  printw("Npc brought out a new pokemon!");
                  getch();
                }
              }
           }

           if(player->playerPokemons[playerPokemon].hp <= 0){
              player->numberOfPokemon--;
              if(player->numberOfPokemon == 0){
                turnCount = 1;
                printw("\nYou lost..\n");
                getch();
              }
              else{
                 printw("Your pokemon %s fainted.\n", player->playerPokemons[playerPokemon].name.c_str());
                  player->playerPokemons[playerPokemon].aliveStatus = 0;
                  getch();
                  turn = switchPokemon(player);
              }
            }
        }
      }
    }
  }
}

int checkFirstMoveAccuracy(pokeDatabase * pokeData, player *player){

  for(int i = 0; i < pokeData->moves.size(); i++){
    if(player->playerPokemons[playerPokemon].move1.c_str() == pokeData->moves[i].getPokeMove()){
      return pokeData->moves[i].getAccuracy();
    }
  }
  return 0;
}

int checkFirstMoveAccuracyNPC(pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map){

  for(int i = 0; i < pokeData->moves.size(); i++){
    if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].move1.c_str() == pokeData->moves[i].getPokeMove()){
      return pokeData->moves[i].getAccuracy();
    }
  }
  return 0;
}

int checkSecondMoveAccuracy(pokeDatabase * pokeData, player *player){
  for(int i = 0; i < pokeData->moves.size(); i++){
    if(player->playerPokemons[playerPokemon].move2.c_str() == pokeData->moves[i].getPokeMove()){
      return pokeData->moves[i].getAccuracy();
    }
  }
  return 0;
}

int checkSecondMoveAccuracyNPC(pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map){
  for(int i = 0; i < pokeData->moves.size(); i++){
    if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].move2.c_str() == pokeData->moves[i].getPokeMove()){
      return pokeData->moves[i].getAccuracy();
    }
  }
  return 0;
}


int checkPokemonMoveTypePlayer(pokeDatabase * pokeData,player *player){

  // check my pokemon type first by comparing with pokeTypes dataset.
  // if my pokemon id is the same as the pokemon id in dataset, 

  vector<int> typeIDs;
  for(int i = 0; i < pokeData->pokeTypes.size(); i++){
    if(pokeData->pokeTypes[i].pokemon_id == player->playerPokemons[playerPokemon].id){
      typeIDs.push_back(pokeData->pokeTypes[i].type_id);
    }
  }

  for(int i = 0; i < pokeData->moves.size(); i++){
    if(player->playerPokemons[playerPokemon].move1.c_str() == pokeData->moves[i].getPokeMove()){

      for(int j = 0; j < typeIDs.size(); j++){
        if(typeIDs[j] = pokeData->moves[i].type_id){
          return 1.5;
        }
      }
    }
  }
  return 1;
}

int checkPokemonMoveTypeNPC (pokeDatabase * pokeData, groupNPC *groupNPC, map_m *map){

  // check my pokemon type first by comparing with pokeTypes dataset.
  // if my pokemon id is the same as the pokemon id in dataset, 

  vector<int> typeIDs;
  for(int i = 0; i < pokeData->pokeTypes.size(); i++){
    if(pokeData->pokeTypes[i].pokemon_id == groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].id){
      typeIDs.push_back(pokeData->pokeTypes[i].type_id);
    }
  }

  for(int i = 0; i < pokeData->moves.size(); i++){
    if(groupNPC->arrNPCs[(map->trainers) - 1].trainerPokemon[npcPokemon].move1.c_str() == pokeData->moves[i].getPokeMove()){

      for(int j = 0; j < typeIDs.size(); j++){
        if(typeIDs[j] = pokeData->moves[i].type_id){
          return 1.5;
        }
      }
    }
  }
  return 1;
}

int critChance(){
  int x = rand() % 10;

  if( x == 0){
    return 1.5;
  }
  return 1;
}

void spawnPlayerItems(player *player){

  player->potions = 5;
  player->revives = 5;
  player->pokeballs = 20;
}

// void damageFormula(){
//   ((((((2 * lvl) / 5) + 2) * power * (attack / defense)) / 50) + 2) * crit * random * STAB * Type.
// }

int main(int argc, char *argv[]){
  
    init_terminal();
    
    char npcMap[ROWS][COLUMNS];;  
    int hikerCost[ROWS][COLUMNS];
    int rivalCost[ROWS][COLUMNS];
   
    char inputKey;
    int officialTrainerCount = trainerCount;

    groupNPC groupNPC;
    world_w currentWorld;  
    
    node_n npcNodes[officialTrainerCount];
    MinHeap heap_h;
    player playerP;

    pokeDatabase *pokeData = new pokeDatabase();

 

    // pokemonList = pokeData->savePokemonFile();
    // moveList = pokeData->saveMovesFile();
    // pokemonMovesList = pokeData->savePokeMoves();
    // pokeStatList = pokeData->savePokemonStats();
    // statsList = pokeData->saveStats();
    // pokemonSpeciesList = pokeData->savePokeSpecies();
    // pokeExperienceList = pokeData->saveExperience();
    // pokemonTypeNameList = pokeData->saveTypeNames();
    // pokemonTypesList = pokeData->savePokemonTypes();

    createWorld(&currentWorld);
    initializeCurrentMap(&currentWorld, 200,200);
    assignMapCoor(currentWorld.maps[200][200], 200, 200);
    newMap(&currentWorld);
   randomSpawnValue(currentWorld.maps[200][200], &playerP);
   //printMapstr(currentWorld.maps[200][200], npcMap);
   // printw("\n");
    fillHikerValue(currentWorld.maps[200][200], &playerP); 
    fillRivalNodes(currentWorld.maps[200][200], &playerP);
    copyNPCHikerCost(currentWorld.maps[200][200], hikerCost);
    copyNPCRivalCost(currentWorld.maps[200][200], rivalCost); 

   
    if(argc == 3){
      officialTrainerCount = atoi(argv[2]);
      // spawnNPCreworked(trainerCount, &playerP, rivalCost, npcMap, &groupNPC);
    }
    // else{
    //   spawnNPCreworked(trainerCount, &playerP, rivalCost, npcMap, &groupNPC);
    // }
   
  
    spawnNPCreworked(officialTrainerCount, &playerP, currentWorld.maps[200][200], npcMap, &groupNPC, pokeData, totalDistance);
     
    putPlayerOnOverlay(&playerP, npcMap);
    
    pokemonPrompt(&playerP, pokeData);
    spawnPlayerItems(&playerP);
    assignNodesHelper(trainerCount, currentWorld.maps[200][200], &currentWorld, &groupNPC, 200, 200);

    // generatePokemonList(officialTrainerCount, &npcPokemon, pokeData);
    npcNodesFunction(officialTrainerCount, &currentWorld, currentWorld.maps[200][200], &heap_h, &groupNPC, &playerP, npcMap, pokeData);
    //   // printNPConly(npcMap);

   

   
    return 0;
}