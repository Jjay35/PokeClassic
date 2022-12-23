#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "node.h"
#include "npcTest.h"
#include <vector>


#define INFINITY 9999

const int ROWS = 21;
const int COLUMNS = 80;
const int numberOfSeeds = 13;


/*
Initialize a struct map that contains char* and char** to fill in the values of a 2d array.
The mapCoordinates stores the mapCoordinates in a 200,200 for us whereas the user will see their map as 0,0
The exits store the exitCoordinates of the map.
*/

typedef struct Map{
    char* arr;
    char** pointerArr;
    int* mapCoordinates;
    int* exits;
    int trainers;
    MinHeap *heap;
    node_n *npcNode;

    node_n **rivalNodes;
    node_n **hikerNodes;
    // vector<pokemonData> listOfPokemon;
    // vector<vector<pokemonData>>* pokemonList; 
}map_m;

/*
Functions of pokemon generation
*/
void statGenerator(vector<pokeStats> pokemonStats, pokemonData pokemonInfo);
pokemonData generatePokemon(pokeDatabase* pokeData, int distance);


/*
Functions from main c file
*/
void fillBorder(char table[ROWS][COLUMNS]);
void printMap(char table[ROWS][COLUMNS]);
char randomizeSymbol(char table[ROWS][COLUMNS]);
void fillSeedLocation(int coordinates[25], char table[ROWS][COLUMNS]);
void randomizeSeed(char table[ROWS][COLUMNS]);
void growSeed(int coordinates[25], char table[ROWS][COLUMNS]);
void growRegion(char table[ROWS][COLUMNS], int x, int y, int counter);
int scanMap(char table[ROWS][COLUMNS]);
void randomExits(int exitCoordinates[8], char table[ROWS][COLUMNS]);
void fillExit(char table[ROWS][COLUMNS], int exitCoordinates[8]);
void fillPath (char table[ROWS][COLUMNS], int exitCoordinates[8]);
void placePokeMart(char table[ROWS][COLUMNS], int x, int y);
void placePokeCenter(char table[ROWS][COLUMNS], int x, int y);


/*
map.h functions based on struct 
*/
void allocateMapSpace(map_m *map);
void allocateHikerSpace(map_m *map);
void printMapInt(map_m *map);
void printMapstr(map_m *map,  char t[ROWS][COLUMNS]);
void fillValues(int t[ROWS-2][COLUMNS-2]);
void copyNPC(map_m *map, int table[ROWS-2][COLUMNS-2]);
void copyTerrain(map_m *map, char table[ROWS][COLUMNS]);
void assignMapCoor(map_m *map, int xCoor, int yCoor);
void exitCoords(map_m *map,  int exitsCoordinates[8]);

void printNPConly(char t[ROWS][COLUMNS]);


int colorEachElement(char x);

void initializeColorPair(){
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
}
/*
Allocate memory space for a map and the exits of the map.
*/
void allocateMapSpace(map_m *map){
 
    map->pointerArr = (char**) malloc(21 * sizeof(char*));
    for(int i = 0; i < 21; i++){
        map->pointerArr[i] = (char*) malloc(80 * sizeof(char));
    }
    //map->npcNode = (node_n *) malloc (5 * sizeof(node_n));
    map->exits = (int*) malloc (8 * sizeof(int));

    // map->rivalCoordsRow = (int**) malloc(19 * sizeof(int*));

    // for(int i = 0; i < 19; i++){
    //     map->rivalCoordsRow[i] = (int*) malloc(78 * sizeof(int));
    // }   
    
}


/*
Prints the hiker's and rival's map values
*/
void printMapHikerInt(map_m *map){
    int i,j;
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-1; j++){
                if(map->hikerNodes[i][j].dist == INFINITY || map->hikerNodes[i][j].dist < - 10000){
                    printw("   ");
                }
                else{
                    printw("%02d ", map->hikerNodes[i][j].dist % 100);
                }
                
        
        }
        printw("\n");
    }
}

void printMapRivalInt(map_m *map){
    int i,j;
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-1; j++){
                if(map->rivalNodes[i][j].dist == INFINITY || map->rivalNodes[i][j].dist < - 10000){
                    printw("   ");
                }
                else{
                    printw("%02d ", map->rivalNodes[i][j].dist % 100);
                }
                
        
        }
        printw("\n");
    }
}

void printMapRivalCost(map_m *map){
    int i,j;
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-1; j++){
                if(map->rivalNodes[i][j].cost == INFINITY || map->rivalNodes[i][j].cost < - 10000){
                    printw("I");
                }
                else{
                     printw("%02d ", map->rivalNodes[i][j].cost);
                }
           
                
        
        }
        printw("\n");
    }
}


/*
Fill the values of the npc map
*/
void fillValues(int t[ROWS-2][COLUMNS-2]){
    int i,j;

    for(i=0; i<ROWS-2; i++){
        for(j=0; j<COLUMNS-2; j++){
            t[i][j] = 1;
        }
    }
}

/*
Spawn PC on the path of the map at random
*/
void randomSpawnValue(map_m *map, player *player){
    char c;
    

    while(c != '#'){
        int col = rand() % (79 - 1 + 1) + 1;
        int row = rand() % (19 - 1 + 1) + 1;

        c = map->pointerArr[row][col];
        if(map->pointerArr[row][col] == '#'){
            player->posX = row;
            player->posY = col;
        
            // map->pointerArr[row][col] = '@';
        }
    }
}

/*
Prints the map with its current coordinate.
*/

void printMapstr(map_m *map, char t[ROWS][COLUMNS]){
    int i,j;
    initializeColorPair();
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){ 

            if(t[i][j] == 'h' || t[i][j] == 'r' || t[i][j] == 'w' || t[i][j] == 'e' || t[i][j] == 'p' || t[i][j] == 's' || t[i][j] == '@'){
                attron(COLOR_PAIR(colorEachElement(t[i][j])));
                printw("%c", t[i][j]);
                attroff(COLOR_PAIR(colorEachElement(t[i][j])));
            }

            else{
                attron(COLOR_PAIR(colorEachElement(map->pointerArr[i][j])));
                printw("%c" , map->pointerArr[i][j]);
                attroff(COLOR_PAIR(colorEachElement(map->pointerArr[i][j])));
            }
               
        
        }
        printw("\n");
    }
    printw("\n");
    // printw("Current map:");
    // printw(" %d %d", map->mapCoordinates[0], map->mapCoordinates[1]);
    // printw("\n");
    // printw("Enter command: ");
}

/*
Copies npc values in a map format to map pointer
*/
// void copyNPC(map_m *map, int table[ROWS-2][COLUMNS-2]){
//     int i,j;
//     for(i = 0; i < ROWS-2; i++){
//         for(j = 0; j < COLUMNS-2; j++){
//            map->rivalCoordsRow[i][j] = table[i][j];
//         }
//     }  
// }


/*
Copies the terrain to the map pointer
*/
void copyTerrain(map_m *map, char table[ROWS][COLUMNS]){
    int i,j;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
           map->pointerArr[i][j] = table[i][j];
        }
    }   
}

/*
Assign map coordinates from the World.
*/

void assignMapCoor(map_m *map, int xCoor, int yCoor){
    map->mapCoordinates = (int *)malloc(2 * sizeof(int));
    map->mapCoordinates[0] = xCoor - 200;
    map->mapCoordinates[1] = yCoor - 200;
}

/*
Stores the exit coordinates
*/
void exitCoords(map_m *map,  int exitsCoordinates[8]){
    int i;
    for(i = 0; i < 8; i++){
        map->exits[i] = exitsCoordinates[i];
    }
}

int colorEachElement(char x){

    if(x == 'p' || x == 'w' || x == 'h' || x == 'r' || x == 'e' || x == 's'){
        return 1;
    }

    if(x == 'C' || x == 'M'){
        return 2;
    }

    if( x == ':' || x == '^' || x == '.'){
        return 3;
    }

    if( x == '@'){
        return 4;
    }

    if(x == '#'){
        return 6;
    }

    if( x == '%'){
        return 7;
    }

   return 0;
}


/*
----Terrain generation code below.---
*/


void fillBorder(char table[ROWS][COLUMNS]){

    int i,j;
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            if((i == 0) || (j == 0) || (i == 20)|| (j == 79)){
                table[i][j] = '%';
            }
            else{
                table[i][j] = ' ';
            }
           
        }
    }
}

/*
This is a function used to print the map
*/

void printMap(char table[ROWS][COLUMNS]){
    int i,j;
    initializeColorPair();
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
                attron(COLOR_PAIR(colorEachElement(table[i][j])));
                printw("%c" , table[i][j]);
                attroff(COLOR_PAIR(colorEachElement(table[i][j])));       
        }
        printw("\n");
    }
    printw("\n");
}

/*
This function is used to randomize numbers and return the char symbol to the specific seed location.

*/
char randomizeSymbol(char table[ROWS][COLUMNS]){
    int x = (rand() % 6) + 1;

    if (x == 1 || x == 2){
        return ':';
       
    }
    if (x == 5 || x == 6){    
        return '.';
    }
    if (x == 3){
        return '^';
    }
    if (x == 4){
        return '%';
    }
    return 0;

}

/*
This function is used to fill the seed location designated on the map.
*/

void fillSeedLocation(int coordinates[25], char table[ROWS][COLUMNS]){
    int i;
    int k = 0;

    for(i = 0; i < 13; i++){
        table[coordinates[k]][coordinates[k+1]] = randomizeSymbol(table);
        k += 2;
    }
    
}

/*
This function is to gather an array of random coordinates to store the seed locations around the map.
It will then call the fillSeedLocation() function to fill the specific regions at specificied coordinates.
Then it will call the growSeed() function in order to grow the regions.
*/

void randomizeSeed(char table[ROWS][COLUMNS]){
    int i;
    int x,y;
    int j = 0;
    int coordinates[25];
  
   srand(time(NULL));
    for(i = 0; i < numberOfSeeds; i++){
        x = (rand() % 19) + 1;
        y = (rand() % 78) + 1;
        coordinates[j] = x;
        coordinates[j+1] = y;
        j += 2;
    }

    fillSeedLocation(coordinates, table);
    growSeed(coordinates,table);
}

/*
The growSeed function will continue to grow the region at the specified coordinate of a seed.
It uses the scanMap() function to check the whole map if it is empty or not.
If it is, this will execute the growRegion() function where it will grow the surrounding of each seed location one by one
based on the coordinates of the seed. 
The counter determines the size of the surrounding seed location.

*/

void growSeed(int coordinates[25], char table[ROWS][COLUMNS]){
    int i;
    int k = 0;
    int counter = 1;
    while(scanMap(table) == 1){
        for(i = 0; i < 13; i++){
            growRegion(table, coordinates[k], coordinates[k+1], counter);
            k += 2;
        }
        k = 0;
       counter++;
    }
}


/*
This is the growRegion function, it checks the surroundings of the current char element and fills it up
with its respective symbol if theres is an empty space around it. 

*/

void growRegion(char table[ROWS][COLUMNS], int x, int y, int counter){
    int i,j;
   
    char symbol = table[x][y];

    for(i = x - counter; i <= x + counter; i++){
        if(i < 1 || i > 19){
            continue;
        }
        for(j = y - counter; j <= y + counter; j++){
            if(j < 1 || j > 78){
                continue;
            }
            if(table[i][j] == ' '){
                table[i][j] = symbol;
            }
           

        }
    }
    
}

/*
This is the scanMap() function. It checks the map if there is an empty space.
If it there is an empty space, it will return 1 which is true. 
Else, it will return 0 which is false.

*/

int scanMap(char table[ROWS][COLUMNS]){
    int i,j;

    for(i = 1; i < 20; i++){
        for(j = 1; j < 79; j++){
            if(table[i][j] == ' '){
                
                return 1; 
            }
        }
    }
    return 0;
}

/*
This function is used to generate the exits of the map at randomized locations.
It has an array called exitCoordinates where it stores the coordinates for each exit at North,South,West and East.
It uses the function fillExit to fill the exits in the map with '#'
Then calls the function fillPath to generate a path from the exits.
*/

void randomExits(int exitCoordinates[8], char table[ROWS][COLUMNS]){
    int i,j;
    int k = 1;
    int l = 4;
    srand(time(NULL));

    exitCoordinates[0] = 0;
    exitCoordinates[2] = 20;
    exitCoordinates[5] = 0;
    exitCoordinates[7] = 79;
    
    for(i = 0; i < 2; i++){
        exitCoordinates[k] = (rand() % 78) + 1;
        k += 2;
    }

    for(j = 0; j < 2; j++){
        exitCoordinates[l] = (rand() % 19) + 1;
        l += 2;
    }

}

/*
This function is used to fill the exit on the map with '#' based on the coordinates given.
*/

void fillExit(char table[ROWS][COLUMNS], int exitCoordinates[8]){
    int i;
    int k = 0;


    for(i = 0; i < 4; i++){
        table[exitCoordinates[k]][exitCoordinates[k+1]] = '#';
        k += 2;
    }
    

}

/*
In this function, we fill the path of the map from the given exits.
First, we grow the path from the top and bottom till they meet at the same row.
Once they meet at the same row, we combine them.
We then call a placePokeMart function to generate a pokeMart at any random location at the same row the paths met.

We do the same thing next to grow a path from left and right but instead they meet at the same column.
And we combine them once they meet at the same column.
Then instead we call a placePokeCenter function to generate a pokeCenter at any random location at the same column the paths met.
*/

void fillPath (char table[ROWS][COLUMNS], int exitCoordinates[8]){
    int i;
    int x1,y1;
    int randomLocation;
    x1 = (rand() % (ROWS/2)) + 1;
    y1 = (rand() % (COLUMNS/2)) + 1;

    for(i = 0; i <= x1; i++){
        table[i][exitCoordinates[1]] = '#';
    }
    
    for(i = exitCoordinates[2]; i >= x1; i--){
        table[i][exitCoordinates[3]] = '#';
    }

    if(table[x1][exitCoordinates[1]] == table[x1][exitCoordinates[3]]){
        if(exitCoordinates[1] < exitCoordinates[3]){
            for(i = exitCoordinates[1]; i < exitCoordinates[3]; i++){
                table[x1][i] = '#';
            }
            randomLocation = (rand() % (exitCoordinates[3] - exitCoordinates[1] + 1)) + exitCoordinates[1];
        }
        else{
            for(i = exitCoordinates[3]; i < exitCoordinates[1]; i++){
                table[x1][i] = '#';
            }
            randomLocation = (rand() % (exitCoordinates[1] - exitCoordinates[3] + 1)) + exitCoordinates[3];
        }

        
        placePokeMart(table, x1, randomLocation);
    }


    for(i = exitCoordinates[5]; i <= y1; i++){
        table[exitCoordinates[4]][i] = '#';
    }
    for(i = exitCoordinates[7]; i >= y1; i--){
        table[exitCoordinates[6]][i] = '#';
    }

    if(table[exitCoordinates[4]][y1] == table[exitCoordinates[6]][y1]){
        if(exitCoordinates[4] < exitCoordinates[6]){
            for(i = exitCoordinates[4]; i < exitCoordinates[6]; i++){
                table[i][y1] = '#';
            }
            randomLocation = (rand() % (exitCoordinates[6] - exitCoordinates[4] + 1)) + exitCoordinates[4];
        }
        else{
            for(i = exitCoordinates[6]; i < exitCoordinates[4]; i++){
                table[i][y1] = '#';
            }
            randomLocation = (rand() % (exitCoordinates[4] - exitCoordinates[6] + 1)) + exitCoordinates[6];
        }
        
        placePokeCenter(table, randomLocation, y1);
    }
}

/*
This is a function used to place the pokeMart in a 2x1 fashion beside the path.
*/

void placePokeMart(char table[ROWS][COLUMNS], int x, int y){
    int i;
    if(table[x+1][y] != '#' && table[x+2][y] != '#'){
        for(i = x+1; i < x+3; i++){
            table[i][y] = 'M';
        }
    }
    else if(table[x-1][y] != '#' && table[x-2][y] != '#'){
        for(i = x-1; i > x-3; i--){
            table[i][y] = 'M';
        }
    }
}

/*
This is a function used to place the pokeCenter in a 1x2 fashion beside the path.
*/

void placePokeCenter(char table[ROWS][COLUMNS], int x, int y){
    int i;

    if(table[x][y+1] != '#' && table[x][y+2] != '#'){
        for(i = y+1; i < y+3; i++){
            table[x][i] = 'C';
        }
    }
    else if(table[x][y-1] != '#' && table[x][y-2] != '#'){
        for(i = y-1; i > y-3; i--){
            table[x][i] = 'C';
        }
    }
}



char npcSymbol(char table[ROWS][COLUMNS]){
    int x = (rand() % 8) + 1;

    if (x == 1 || x == 2){    
        return 'r';
    }
    if (x == 3 || x == 4){    
        return 'h';
    }
    if (x == 5){
        return 'p';
    }
    if (x == 6){
        return 'w';
    }
    if (x == 7){
        return 's';
    }
    if (x == 8){
        return 'e';
    }
    return 0;
}

char hikerSpawn(char table[ROWS][COLUMNS]){
    return 'h';
}

void putPlayerOnOverlay(player *player, char npcMap[ROWS][COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            if(i == player->posX && j == player->posY){
                npcMap[i][j]= '@';
            }
        }
    }
}

void spawnNPCreworked(int numTrainers, player *player, map_m *map, char npcMap[ROWS][COLUMNS], groupNPC *groupNPC, pokeDatabase *pokeData, int distance){

   int randomNumber;
    groupNPC->arrNPCs= (npc *) malloc (numTrainers * sizeof(npc) );
    

    //printw("This is @ coordinate, (%d,%d)\n", player->posX, player->posY);
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            // if(i == player->posX && j == player->posY){
            //     npcMap[i][j]= '@';
            // }
            if(map->rivalNodes[i][j].cost == INFINITY){
                npcMap[i][j] = 'I';
            }
            else{
                npcMap[i][j] = ' ';
            }
         
        }
    }

    for(int i = 0; i < numTrainers; i++){
        int col = rand() % (78 - 1 + 1) + 1;
        int row = rand() % (19 - 1 + 1) + 1;

        while(npcMap[row][col] == 'I'){
            col = rand() % (78 - 1 + 1) + 1;
            row = rand() % (19 - 1 + 1) + 1;
        }
        
        // if(rivalCost[row][col] == INFINITY){
        //     numberOfNPC.arrNPCs[i].coordX = row;
        //     numberOfNPC.arrNPCs[i].coordY = col;
        //     npcMap[row][col] = hikerSpawn(npcMap);
        //     numberOfNPC.arrNPCs[i].npcType = npcMap[row][col];
          
        // }
        
            groupNPC->arrNPCs[i].coordX = row;
            groupNPC->arrNPCs[i].coordY = col;
            npcMap[row][col] = npcSymbol(npcMap);
            groupNPC->arrNPCs[i].npcType = npcMap[row][col];

            randomNumber = rand() % 6 + 1;
            for(int j = 0; j < randomNumber; j++){
                
                pokemonData temp = generatePokemon(pokeData, distance);
                groupNPC->arrNPCs[i].trainerPokemon[j] = temp;
                groupNPC->arrNPCs[i].numberofPokemon++;

            //     clear();
            //   printw("NPC:%c, Number:%d, Pokemon: %s", npcMap[row][col], i, groupNPC->arrNPCs[i].trainerPokemon[j].name.c_str());
            //    usleep(700000);
            //     refresh();
            }
            
            
            // groupNPC->arrNPCs[i].trainerPokemon.push_back(generatePokemon(pokeData, distance));
            
            // if(npcMap[row][col]== 'h'){
            //     groupNPC->arrNPCs[i].prevX = map->hikerNodes[row][col].previousX;
            //     printw("previousX : %d", map->hikerNodes[row][col].previousX);
            //     groupNPC->arrNPCs[i].prevY = map->hikerNodes[row][col].previousY;
            // }else{
            //     if(groupNPC->arrNPCs[i].npcType == 'r'){
            //         groupNPC->arrNPCs[i].prevX = map->rivalNodes[row][col].previousX;
            //         groupNPC->arrNPCs[i].prevY = map->rivalNodes[row][col].previousY;
            //     }
            // }
    }
  
     map->trainers = numTrainers;
   //printNPConly(npcMap);
}

void restoreNPC(map_m *map,  char npcMap[ROWS][COLUMNS]){

int xLoc;
int yLoc;
   
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            // if(i == player->posX && j == player->posY){
            //     npcMap[i][j]= '@';
            // }
            // if(i == map->npcNode[k].currX && j == map->npcNode[k].currY){
            //     npcMap[i][j] = map->npcNode[k].symbol;
            //     k++;
            // }
            if(map->rivalNodes[i][j].cost == INFINITY){
                npcMap[i][j] = 'I';
            }
            else{
                npcMap[i][j] = ' ';
            }
         
        }
    }
    for(int k = 0; k < map->trainers; k++){
        xLoc = map->heap->arrNodes[k].currX;
        yLoc = map->heap->arrNodes[k].currY;
        npcMap[xLoc][yLoc] = map->heap->arrNodes[k].symbol;
    }
}


void printNPConly(char t[ROWS][COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            attron(COLOR_PAIR(colorEachElement(t[i][j])));
            printw("%c", t[i][j]);
            attroff(COLOR_PAIR(colorEachElement(t[i][j])));
        }
        printw("\n");
    }
    printw("\n");
}

void copyNPCRivalCost(map_m *map, int cost[ROWS][COLUMNS]){
    for(int i = 0; i < 21; i++){
        for(int j = 0; j < 80; j++){
            cost[i][j] = map->rivalNodes[i][j].cost;
        }
    }
}

void copyNPCHikerCost(map_m *map, int cost[ROWS][COLUMNS]){
    for(int i = 0; i < 21; i++){
        for(int j = 0; j < 80; j++){
            cost[i][j] = map->hikerNodes[i][j].cost;

        }
    }
}

int randomNumber3(){
    int x = (rand() % 3) + 1;

    if(x == 1){
      return 1;
    }
    if(x == 2){
      return 2;
    }
    if(x == 3){
      return 4;
    }

    return 0;
}

int randomNumber2(){
    int x = (rand() % 3) + 1;

    if(x == 1){
      return 1;
    }
    if(x == 2){
      return 3;
    }
    if(x == 3){
      return 4;
    }

    return 0;
}

/*10% chance of spawning pokemon*/
int chanceOfMeetingPokemon(){
    int x = (rand() % 10) + 1;

    if( x == 1){
        return 1;
    }
    return 0;
}

int chanceOfMeetingShinyPokemon(){
    int x = rand() % 8192;

    if ( x == 0){
        return 1;
    }
    return 0;
}

int randomIVgenerator(){
  int x;

  x = rand() % 15;
  return x;
}


pokemonData generatePokemon(pokeDatabase* pokeData, int distance){
  int randomPokemon = (rand() % 1092) + 0; 
  string pokemonName;
  int pokemonID;
  int minLvl;
  int maxLvl;
  int pokemonLvl;
  int randomMoveID1;
  int randomMoveID2;
  string pokemonMove1;
  string pokemonMove2;

  int count = 0;  
  int hpIV;
  int attackIV;
  int defenseIV;
  int speedIV;
  int specialAtkIV;
  int specialDefIV;  
  vector <pokemonMoves> specificPokemonMove;
  vector <pokeStats> specificPokemonStats;
  pokemonData pokemonInfo;

 
  
  if(distance > 200){
    minLvl = (distance - 200) / 2;
    maxLvl = 100;
    pokemonLvl = minLvl + rand() % (maxLvl - minLvl + 1);
     //printw("minlvl: %d, maxlvl: %d, pokelvl: %d", minLvl, maxLvl, pokemonLvl);
  }
  if(distance <= 200){
    minLvl = 1;
    maxLvl = distance / 2;
    pokemonLvl = minLvl + rand() % (maxLvl - minLvl + 1);
  }
// pokemonLvl = distance;

  pokemonID = pokeData->pokemons[randomPokemon].id;

//   while(1){
    for(int i = 0; i < pokeData->pokeMoves.size(); i++){
      if(pokeData->pokeMoves[i].getPokemonMoveId() == pokemonID && pokeData->pokeMoves[i].getPokeMoveMethodId() == 1 && pokeData->pokeMoves[i].getLevel() <= pokemonLvl){
          specificPokemonMove.push_back(pokeData->pokeMoves[i]);
      }
    }
//     if(specificPokemonMove.empty()){
//       pokemonLvl++;
//       continue;
//     }
//     else{
//       break;
//     }
//   }

  randomMoveID1 = pokeData->pokeMoves[rand() % pokeData->pokeMoves.size()].getMoveId();
  randomMoveID2 = pokeData->pokeMoves[rand() % pokeData->pokeMoves.size()].getMoveId();

  pokemonMove1 = pokeData->moves[randomMoveID1].getPokeMove();
  pokemonMove2 = pokeData->moves[randomMoveID2].getPokeMove();

  pokemonInfo.name = pokeData->pokemons[randomPokemon].getPokeName();
  pokemonInfo.id = pokemonID;
  pokemonInfo.level = pokemonLvl;
  pokemonInfo.move1 = pokemonMove1;
  pokemonInfo.move2 = pokemonMove2;
  pokemonInfo.priority1 = pokeData->moves[randomMoveID1].getPriority();
  pokemonInfo.priority2 = pokeData->moves[randomMoveID2].getPriority();
  pokemonInfo.power1 = pokeData->moves[randomMoveID1].getPower();
  pokemonInfo.power2 = pokeData->moves[randomMoveID2].getPower();
  pokemonInfo.aliveStatus = 1;

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
  
  pokemonInfo.hp = ((((specificPokemonStats[count].getBaseStat() + hpIV) * 2) * pokemonLvl) / 100) + pokemonLvl + 10;
  pokemonInfo.mainHp = pokemonInfo.hp;
  count++;

 pokemonInfo.attack = ((((specificPokemonStats[count].getBaseStat() + attackIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  pokemonInfo.defense = ((((specificPokemonStats[count].getBaseStat() + defenseIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  pokemonInfo.speed =  ((((specificPokemonStats[count].getBaseStat() + speedIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  pokemonInfo.speAtk = ((((specificPokemonStats[count].getBaseStat() + specialAtkIV) * 2) * pokemonLvl) / 100) + 5;
  count++;
  pokemonInfo.speDef = ((((specificPokemonStats[count].getBaseStat() + specialDefIV) * 2) * pokemonLvl) / 100) + 5;
//   statGenerator(specificPokemonStats, pokemonInfo);

  return pokemonInfo;
}

void statGenerator(vector<pokeStats> pokemonStats, pokemonData pokemonInfo){
  int count = 0;
  
  while(count < 6){
    if(count == 0){
      pokemonInfo.hp = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + pokemonInfo.level+ 10;
      count++;
    }
    if(count == 1){
      pokemonInfo.attack = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + 5;
      count++;
    }
    if(count == 2){
      pokemonInfo.defense = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + 5;
      count++;
    }
    if(count == 3){
      pokemonInfo.speed = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + 5;
      count++;
    }
    if(count == 4){
      pokemonInfo.speAtk = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + 5;
      count++;
    }
    if(count == 5){
      pokemonInfo.speDef = ((((pokemonStats[count].getBaseStat() + randomIVgenerator()) * 2) * pokemonInfo.level) / 100) + 5;
      count++;
    }
  }
}
