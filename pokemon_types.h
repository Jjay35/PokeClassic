#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class pokemonTypes{
public:
    int pokemon_id;
    int type_id;
    int slot;


    pokemonTypes(){};
    pokemonTypes(int pokemon_id, int type_id, int slot){
        this->pokemon_id = pokemon_id;
        this->type_id = type_id;
        this->slot = slot;
    
    }

   


    ~pokemonTypes(){}

    string toString() const{
        
        return "Id: " + to_string(pokemon_id) + "\n"
            + "type_id: " + to_string(type_id) + "\n"
            + "slot: " + to_string(slot) + "\n";
          
           
    } 
};




