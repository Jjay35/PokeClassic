#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class pokemonMoves{
public:
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;

// public:
    pokemonMoves(){};
    pokemonMoves(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order){
        this->pokemon_id = pokemon_id;
        this->version_group_id = version_group_id;
        this->move_id = move_id;
        this->pokemon_move_method_id = pokemon_move_method_id;
        this->level = level;
        this->order = order;
    
    }

    int getPokemonMoveId(){
        return pokemon_id;
    }
    int getPokeMoveMethodId(){
        return pokemon_move_method_id;
    }

    int getMoveId(){
        return move_id;
    }
    
    int getLevel(){
        return level;
    }


    ~pokemonMoves(){}

    string toString() const{
        return "Id: " + to_string(pokemon_id) + "\n"
            + "version_group_id: " + to_string(version_group_id) + "\n"
            + "move_id: " + to_string(move_id) + "\n"
            + "pokemon_move_method_id: " + to_string(pokemon_move_method_id) + "\n"
            + "level: " + to_string(level) + "\n"
            + "order: " + to_string(order) + "\n";
           
    } 
};




