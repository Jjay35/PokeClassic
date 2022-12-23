#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class Stats{
public:

    int id;
    int damage_class_id;
    string identifier;
    int is_battle_only;
    int game_index;

// public:
    Stats(){};
    Stats(int id, int damage_class_id, string identifier, int is_battle_only, int game_index){
        this->id = id;
        this->damage_class_id = damage_class_id;
        this->identifier = identifier;
        this->is_battle_only = is_battle_only;
        this->game_index = game_index;
    
    }

    int getId(){
        return id;
    }
   


    ~Stats(){}

    string toString() const{
        return "Id: " + to_string(id) + "\n"
            + "damage_class_id: " + to_string(damage_class_id) + "\n"
            + "identifier: " + identifier+ "\n"
            + "is_battle_only: " + to_string(is_battle_only) + "\n"
            + "game_index: " + to_string(game_index) + "\n";
          
           
    } 
};




