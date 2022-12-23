#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class pokemonSpecies{
public:
    
    int id;
    string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;

// public:
    pokemonSpecies(){};
    pokemonSpecies(int id, string identifier,int generation_id,int evolves_from_species_id,int evolution_chain_id,int color_id,int shape_id,int habitat_id,int gender_rate,
                    int capture_rate,int base_happiness,int is_baby,int hatch_counter,int has_gender_differences,int growth_rate_id,int forms_switchable,int is_legendary,int is_mythical,int order,int conquest_order){
        this->id = id;
        this->identifier = identifier;
        this->generation_id = generation_id;
        this->evolves_from_species_id = evolves_from_species_id;
        this->evolution_chain_id = evolution_chain_id;
        this->color_id = color_id;
        this->shape_id = shape_id;
        this->habitat_id = habitat_id;
        this->gender_rate = gender_rate;
        this->capture_rate = capture_rate;
        this->base_happiness = base_happiness;
        this->is_baby = is_baby;
        this->hatch_counter = hatch_counter;
        this->has_gender_differences = has_gender_differences;
        this->growth_rate_id = growth_rate_id;
        this->forms_switchable = forms_switchable;
        this->is_legendary = is_legendary;
        this->is_mythical = is_mythical;
        this->order = order;
        this->conquest_order = conquest_order;

    }

    int getPokemonId(){
        return id;
    }


    ~pokemonSpecies(){}

    string toString() const{
        return "Id: " + to_string(id) + "\n"
            + "identifier: " + identifier + "\n"
            + "generation_id: " + to_string(generation_id) + "\n"
            + "evolves_from_species_id: " + to_string(evolves_from_species_id) + "\n"
            + "evolution_chain_id: " + to_string(evolution_chain_id) + "\n"
            + "color_id: " + to_string(color_id) + "\n"
            + "shape_id: " + to_string(shape_id) + "\n"
            + "habitat_id: " + to_string(habitat_id) + "\n"
            + "gender_rate: " + to_string(gender_rate) + "\n"
            + "capture_rate: " + to_string(capture_rate) + "\n"
            + "base_happiness: " + to_string(base_happiness) + "\n"
            + "is_baby: " + to_string(is_baby) + "\n"
            + "hatch_counter: " + to_string(hatch_counter) + "\n"
            + "has_gender_differences: " + to_string(has_gender_differences) + "\n"
            + "growth_rate_id: " + to_string(growth_rate_id) + "\n"
            + "forms_switchable: " + to_string(forms_switchable) + "\n"
            + "is_legendary: " + to_string(is_legendary) + "\n"
            + "is_mythical: " + to_string(is_mythical) + "\n"
            + "order: " + to_string(order) + "\n"
            + "conquest_order: " + to_string(conquest_order) + "\n";
            
        
           
           
    } 
};




