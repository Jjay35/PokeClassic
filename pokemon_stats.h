#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class pokeStats{
public:

    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;

// public:
    pokeStats(){};
    pokeStats(int pokemon_id, int stat_id, int base_stat, int effort){
        this->pokemon_id = pokemon_id;
        this->stat_id = stat_id;
        this->base_stat = base_stat;
        this->effort = effort;
    
    }
    int getPokeID(){
        return pokemon_id;
    }
    int getStatId(){
        return stat_id;
    }

    int getBaseStat(){
        return base_stat;
    }


    ~pokeStats(){}

    string toString() const{
        return "Pokemon_Id: " + to_string(pokemon_id) + "\n"
            + "stat_id: " + to_string(stat_id) + "\n"
            + "base_stat: " + to_string(base_stat) + "\n"
            + "effort: " + to_string(effort) + "\n";
            
          
           
    } 
};




