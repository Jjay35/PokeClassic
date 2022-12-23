#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class Moves{
public:
    int id;
    string pokeMove;
    int gen_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;

// public:
    Moves(){};
    Moves(int id, string pokeMove, int gen_id, int type_id,int power,int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance, int contest_type_id, int contest_effect_id, int super_contest_effect_id){
        this->id = id;
        this->pokeMove = pokeMove;
        this->gen_id = gen_id;
        this->type_id = type_id;
        this->power = power;
        this->pp = pp;
        this->accuracy = accuracy;
        this->priority = priority;
        this->target_id = target_id;
        this->damage_class_id = damage_class_id;
        this->effect_id = effect_id;
        this->effect_chance = effect_chance;
        this->contest_type_id = contest_type_id;
        this->contest_effect_id = contest_effect_id;
        this->super_contest_effect_id = super_contest_effect_id;
    }

    string getPokeMove(){
        return pokeMove;
    }
    
    int getPriority(){
        return priority;
    }

    int getPower(){
        return power;
    }

    int getAccuracy(){
        return accuracy;
    }
    ~Moves(){}

    string toString() const{
        return "Id: " + to_string(id) + "\n"
            + "Identifier: " + pokeMove + "\n"
            + "Generation_id: " + to_string(gen_id) + "\n"
            + "type_id: " + to_string(type_id) + "\n"
            + "power: " + to_string(power) + "\n"
            + "pp: " + to_string(pp) + "\n"
            + "accuracy: " + to_string(accuracy) + "\n"
            + "priority: " + to_string(priority) + "\n"
            + "target_id: " + to_string(target_id) + "\n"
            + "damage_class_id: " + to_string(damage_class_id) + "\n"
            + "effect_id: " + to_string(effect_id) + "\n"
            + "effect_chance: " + to_string(effect_chance) + "\n"
            + "contest_type_id: " + to_string(contest_type_id) + "\n"
            + "contest_effect_id: " + to_string(contest_effect_id) + "\n"
            + "super_contest_effect_id: " + to_string(super_contest_effect_id) + "\n";
    } 
};




