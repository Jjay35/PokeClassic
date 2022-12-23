#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class pokemonTypeNames{
public:

    int type_id;
    int local_language_id;
    string name;

// public:
    pokemonTypeNames(){};
    pokemonTypeNames(int type_id, int local_language_id, string name){
        this->type_id = type_id;
        this->local_language_id = local_language_id;
        this->name = name;
    
    }

   


    ~pokemonTypeNames(){}

    string toString() const{
        return "type_id: " + to_string(type_id) + "\n"
            + "local_language_id: " + to_string(local_language_id) + "\n"
            + "name: " + name + "\n";
          
           
    } 
};




