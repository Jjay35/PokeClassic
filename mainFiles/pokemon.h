#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class Pokemon{
public:
    int id;
    string pokemon_name;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;

// public:
    Pokemon(){};
    Pokemon(int id, string pokemon_name, int species_id, int height, int weight, int base_experience, int order, int is_default){
        this->id = id;
        this->pokemon_name = pokemon_name;
        this->species_id = species_id;
        this->height = height;
        this->weight = weight;
        this->base_experience = base_experience;
        this->order = order;
        this->is_default = is_default;
    }

    int getId(){
        return id;
    }
    void setId(int id){
        this->id = id;
    }
    string getPokeName(){
        return pokemon_name;
    }
    void setPokeName(string pokemon_name){
        this->pokemon_name = pokemon_name;
    }
    int getSpecies_id(){
        return id;
    }
    void setSpecies_id(int species_id){
        this->species_id = species_id;
    }
    int getHeight(){
        return height;
    }
    void setHeight(int height){
        this->height = height;
    }
    int getWeight(){
        return weight;
    }
    void setWeight(int weight){
        this->weight = weight;
    }
    int getBaseExp(){
        return base_experience;
    }
    void setBaseExp(int base_experience){
        this->base_experience = base_experience;
    }
    int getOrder(){
        return order;
    }
    void setOrder(int order){
        this->order = order;
    }
    int getIs_default(){
        return is_default;
    }
    void setIs_default(int is_default){
        this->is_default = is_default;
    }


    ~Pokemon(){}

    string toString() const{
        return "Id: " + to_string(id) + "\n"
            + "Identifier: " + pokemon_name + "\n"
            + "Species_id: " + to_string(species_id) + "\n"
            + "Height: " + to_string(height) + "\n"
            + "Weight: " + to_string(weight) + "\n"
            + "Base_Experience: " + to_string(base_experience) + "\n"
            + "Order: " + to_string(order) + "\n"
            + "Is_default: " + to_string(is_default) + "\n";
    } 
};

// int main(){
//     Pokemon p[5];

//      p[0].setId(1);

//     for(int i = 0; i < 5; i++){
//         cout << p[i].toString() << endl;
//         cout << p[0].getId() << endl;
//     }

//     return 0;
// }

