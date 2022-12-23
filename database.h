#include "pokemon.h"
#include "moves.h"
#include "pokemonMoves.h"
#include "pokemonSpecies.h"
#include "experience.h"
#include "type_names.h"
#include "pokemon_stats.h"
#include "stats.h"
#include "pokemon_types.h"
# include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>

class pokeDatabase{
    public:

    Pokemon newPokemon;
    Moves newMoves;
    pokemonMoves newPokeMoves;
    pokemonTypes newPokeTypes;
    pokeStats newPokeAllStats;
    pokemonTypeNames newPokeTypeNames;
    pokemonSpecies newPokeSpecies;
    Stats newPokemonStats;
    Experience newPokeExperience;


    vector<Pokemon> pokemons;
    vector<Moves> moves;
    vector<pokemonMoves> pokeMoves;
    vector<pokemonTypes> pokeTypes;
    vector<pokeStats> allPokeStats;
    vector<pokemonTypeNames> pokeTypeNames;
    vector<pokemonSpecies> pokeSpecies;
    vector<Stats> pokemonStats;
    vector<Experience> pokeExperience;

    pokeDatabase(){






  ifstream myFile_handler;
  ifstream envHandler;
  string myline;
  string fileName = "pokemon";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
    
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

           // checkInputInt(myline, id);
            getline(inputString, tempString, ',');
            newPokemon.id = atoi(tempString.c_str());
            
            getline(inputString, newPokemon.pokemon_name, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.species_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.height = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.weight = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.base_experience = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.order = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemon.is_default  = atoi(tempString.c_str());

            Pokemon pokemon(newPokemon.id,newPokemon.pokemon_name, newPokemon.species_id, 
                        newPokemon.height, newPokemon.weight, newPokemon.base_experience, newPokemon.order, newPokemon.is_default);

            pokemons.push_back(pokemon);

            myline = "";
           
     }
        // for(auto pokemon : pokemons){
        //     cout << pokemon.toString() << endl;
        // }
        myFile_handler.close();
      
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int id;
            string pokemon_name;
            int species_id;
            int height;
            int weight;
            int base_experience;
            int order;
            int is_default;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

           // checkInputInt(myline, id);
            getline(inputString, tempString, ',');
            newPokemon.id = atoi(tempString.c_str());
            
            getline(inputString, newPokemon.pokemon_name, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.species_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.height = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.weight = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.base_experience = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokemon.order = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemon.is_default  = atoi(tempString.c_str());

            Pokemon pokemon(newPokemon.id,newPokemon.pokemon_name, newPokemon.species_id, 
                        newPokemon.height, newPokemon.weight, newPokemon.base_experience, newPokemon.order, newPokemon.is_default);

            pokemons.push_back(pokemon);

            myline = "";
           
        }
        // for(auto pokemon : pokemons){
        //     cout << pokemon.toString() << endl;
        // }
        envHandler.close();
   
     }




//   myFile_handler = "";
//   envHandler = "";
  myline = "" ;
  fileName = "moves";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

   if(myFile_handler.is_open()){
       
         getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
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
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            newMoves.id = atoi(tempString.c_str());
            
            getline(inputString, newMoves.pokeMove, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.gen_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.type_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = " ";
            }
            newMoves.power = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.pp = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.accuracy = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.priority  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.target_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.damage_class_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.effect_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.effect_chance  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.contest_type_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.contest_effect_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.super_contest_effect_id  = atoi(tempString.c_str());


            //5,7,12,14,15f
            Moves move( newMoves.id, newMoves.pokeMove,  newMoves.gen_id, newMoves.type_id, newMoves.power, newMoves.pp,  newMoves.accuracy,  newMoves.priority,  newMoves.target_id,  newMoves.damage_class_id,  newMoves.effect_id,  newMoves.effect_chance,newMoves.contest_type_id, newMoves.contest_effect_id, newMoves.super_contest_effect_id);

            moves.push_back(move);

            myline = "";
          
        }
        // for(auto move : moves){
        //     cout << move.toString() << endl;
        // }
           myFile_handler.close();
         
        // cout << "Total lines: " << lines << endl;
     }
     else{
         string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

        
         getline(envHandler, myline);
        while(getline(envHandler, myline)){
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
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

                      getline(inputString, tempString, ',');
            newMoves.id = atoi(tempString.c_str());
            
            getline(inputString, newMoves.pokeMove, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.gen_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.type_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = " ";
            }
            newMoves.power = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.pp = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.accuracy = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.priority  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.target_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.damage_class_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newMoves.effect_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.effect_chance  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.contest_type_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.contest_effect_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newMoves.super_contest_effect_id  = atoi(tempString.c_str());


            //5,7,12,14,15f
            Moves move( newMoves.id, newMoves.pokeMove,  newMoves.gen_id, newMoves.type_id, newMoves.power, newMoves.pp,  newMoves.accuracy,  newMoves.priority,  newMoves.target_id,  newMoves.damage_class_id,  newMoves.effect_id,  newMoves.effect_chance,newMoves.contest_type_id, newMoves.contest_effect_id, newMoves.super_contest_effect_id);

            moves.push_back(move);

            myline = "";
          
        }
        // for(auto move : moves){
        //     cout << move.toString() << endl;
        // }
        envHandler.close();
      
     }



//   myFile_handler = "";
//   envHandler = "";
  myline = "";
  fileName = "pokemon_moves";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
        // for(int i = 0; i < 100; i++){
        //     getline(myFile_handler, myline);
        //     cout << myline << endl;
        //     lines++;
        // }
      
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int pokemon_id;
            int version_group_id;
            int move_id;
            int pokemon_move_method_id;
            int level;
            int order;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            newPokeMoves.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            newPokeMoves.version_group_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokeMoves.move_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.pokemon_move_method_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.level = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.order = atoi(tempString.c_str());


            //5,7,12,14,15f
            pokemonMoves pokeMove(newPokeMoves.pokemon_id,  newPokeMoves.version_group_id,  newPokeMoves.move_id, newPokeMoves.pokemon_move_method_id, newPokeMoves.level,  newPokeMoves.order);

            pokeMoves.push_back(pokeMove);

            myline = "";
            
        }
        // for(auto pokeMove : pokeMoves){
        //     cout << pokeMove.toString() << endl;
        // }
        myFile_handler.close();
        
        // cout << "Total lines: " << lines << endl;
     }
     else {
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

     
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int pokemon_id;
            int version_group_id;
            int move_id;
            int pokemon_move_method_id;
            int level;
            int order;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            newPokeMoves.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            newPokeMoves.version_group_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            newPokeMoves.move_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.pokemon_move_method_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.level = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeMoves.order = atoi(tempString.c_str());


            //5,7,12,14,15f
            pokemonMoves pokeMove(newPokeMoves.pokemon_id,  newPokeMoves.version_group_id,  newPokeMoves.move_id, newPokeMoves.pokemon_move_method_id, newPokeMoves.level,  newPokeMoves.order);

            pokeMoves.push_back(pokeMove);

            myline = "";
            
           
        }
        // for(auto pokeMove : pokeMoves){
        //     cout << pokeMove.toString() << endl;
        // }
        envHandler.close();
        
     }



//   myFile_handler = "";
//    envHandler = "";
  myline = "";
  fileName = "pokemon_species";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
        
       
         getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
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
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            newPokeSpecies.id = atoi(tempString.c_str());
            
            getline(inputString, newPokeSpecies.identifier, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.generation_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.evolves_from_species_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.evolution_chain_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.color_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.shape_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.habitat_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.gender_rate  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.capture_rate = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.base_happiness  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_baby  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.hatch_counter = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.has_gender_differences = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.growth_rate_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.forms_switchable  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_legendary  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_mythical  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.order = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.conquest_order = atoi(tempString.c_str());


            //5,7,12,14,15f
            pokemonSpecies species( newPokeSpecies.id, newPokeSpecies.identifier, newPokeSpecies.generation_id, newPokeSpecies.evolves_from_species_id, newPokeSpecies.evolution_chain_id, newPokeSpecies.color_id, newPokeSpecies.shape_id, newPokeSpecies.habitat_id, newPokeSpecies.gender_rate,
                     newPokeSpecies.capture_rate, newPokeSpecies.base_happiness, newPokeSpecies.is_baby, newPokeSpecies.hatch_counter, newPokeSpecies.has_gender_differences, newPokeSpecies.growth_rate_id, newPokeSpecies.forms_switchable, newPokeSpecies.is_legendary, newPokeSpecies.is_mythical, newPokeSpecies.order, newPokeSpecies.conquest_order);

            pokeSpecies.push_back(species);

            myline = "";
           
        }
        // for(auto species : pokeSpecies){
        //     cout << species.toString() << endl;
        // }
        myFile_handler.close();
      
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

     
         getline(envHandler, myline);
        while(getline(envHandler, myline)){
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
            string tempString = "";
 
            stringstream inputString(myline);
            getline(inputString, tempString, ',');
            newPokeSpecies.id = atoi(tempString.c_str());
            
            getline(inputString, newPokeSpecies.identifier, ',');

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.generation_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.evolves_from_species_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.evolution_chain_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.color_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.shape_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.habitat_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.gender_rate  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.capture_rate = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.base_happiness  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_baby  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.hatch_counter = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.has_gender_differences = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.growth_rate_id  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.forms_switchable  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_legendary  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.is_mythical  = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.order = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeSpecies.conquest_order = atoi(tempString.c_str());


            //5,7,12,14,15f
            pokemonSpecies species( newPokeSpecies.id, newPokeSpecies.identifier, newPokeSpecies.generation_id, newPokeSpecies.evolves_from_species_id, newPokeSpecies.evolution_chain_id, newPokeSpecies.color_id, newPokeSpecies.shape_id, newPokeSpecies.habitat_id, newPokeSpecies.gender_rate,
                     newPokeSpecies.capture_rate, newPokeSpecies.base_happiness, newPokeSpecies.is_baby, newPokeSpecies.hatch_counter, newPokeSpecies.has_gender_differences, newPokeSpecies.growth_rate_id, newPokeSpecies.forms_switchable, newPokeSpecies.is_legendary, newPokeSpecies.is_mythical, newPokeSpecies.order, newPokeSpecies.conquest_order);

            pokeSpecies.push_back(species);

            myline = "";
           
        }
        // for(auto species : pokeSpecies){
        //     cout << species.toString() << endl;
        // }
        envHandler.close();
       
     }



//    myFile_handle = "" ;
//   envHandler = "" ;
  myline = "";
  fileName = "experience";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

   if(myFile_handler.is_open()){
        
       
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int growth_rate_id;
            int level;
            int experience;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.growth_rate_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.level = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.experience = atoi(tempString.c_str());


            //5,7,12,14,15f
            Experience experienceLine(newPokeExperience.growth_rate_id, newPokeExperience.level, newPokeExperience.experience);

            pokeExperience.push_back(experienceLine);

            myline = "";
           
        }
        // for(auto experienceLine : totalExperience){
        //     cout << experienceLine.toString() << endl;
        // }
        myFile_handler.close();
       
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

        
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int growth_rate_id;
            int level;
            int experience;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.growth_rate_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.level = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeExperience.experience = atoi(tempString.c_str());


            //5,7,12,14,15f
            Experience experienceLine(newPokeExperience.growth_rate_id, newPokeExperience.level, newPokeExperience.experience);

            pokeExperience.push_back(experienceLine);

            myline = "";
           
        }
        // for(auto experienceLine : totalExperience){
        //     cout << experienceLine.toString() << endl;
        // }
        envHandler.close();
      
     }


//   myFile_handler = "" ;
//   envHandler = "";
  myline = "";
  fileName = "type_names";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
       
        
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int type_id;
            int local_language_id;
            string name;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypeNames.type_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypeNames.local_language_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, newPokeTypeNames.name, ',');
            


            //5,7,12,14,15f
            pokemonTypeNames pokeTypeName(newPokeTypeNames.type_id, newPokeTypeNames.local_language_id, newPokeTypeNames.name);

            pokeTypeNames.push_back(pokeTypeName);

            myline = "";
           
        }
        // for(auto pokeTypeName : pokeTypeNames){
        //     cout << pokeTypeName.toString() << endl;
        // }
        myFile_handler.close();
     
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

     
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int type_id;
            int local_language_id;
            string name;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypeNames.type_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypeNames.local_language_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, newPokeTypeNames.name, ',');
            


            //5,7,12,14,15f
            pokemonTypeNames pokeTypeName(newPokeTypeNames.type_id, newPokeTypeNames.local_language_id, newPokeTypeNames.name);

            pokeTypeNames.push_back(pokeTypeName);

            myline = "";
           
        }
        // for(auto pokeTypeName : pokeTypeNames){
        //     cout << pokeTypeName.toString() << endl;
        // }
        envHandler.close();
     
     }



//   myFile_handler = "";
//   envHandler = "";
  myline = "";
  fileName = "pokemon_stats";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
        
        
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int pokemon_id;
            int stat_id;
            int base_stat;
            int effort;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.stat_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.base_stat = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.effort = atoi(tempString.c_str());

            
            pokeStats pokeStat(newPokeAllStats.pokemon_id, newPokeAllStats.stat_id, newPokeAllStats.base_stat, newPokeAllStats.effort);

            allPokeStats.push_back(pokeStat);

            myline = "";
           
        }
        // for(auto pokeStat: allPokeStats){
        //     cout << pokeStat.toString() << endl;
        // }
        myFile_handler.close();
       
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

       
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int pokemon_id;
            int stat_id;
            int base_stat;
            int effort;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.stat_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.base_stat = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeAllStats.effort = atoi(tempString.c_str());

            
            pokeStats pokeStat(newPokeAllStats.pokemon_id, newPokeAllStats.stat_id, newPokeAllStats.base_stat, newPokeAllStats.effort);

            allPokeStats.push_back(pokeStat);

            myline = "";
           
        }
        // for(auto pokeStat: allPokeStats){
        //     cout << pokeStat.toString() << endl;
        // }
        envHandler.close();
     
     }


//   myFile_handler = "";
//   envHandler = "";
  myline = "";
  fileName = "stats";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

  if(myFile_handler.is_open()){
        
       
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int id;
            int damage_class_id;
            string identifier;
            int is_battle_only;
            int game_index;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.damage_class_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, newPokemonStats.identifier, ',');
           
            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.is_battle_only = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.game_index = atoi(tempString.c_str());

            
            Stats stat(newPokemonStats.id, newPokemonStats.damage_class_id, newPokemonStats.identifier, newPokemonStats.is_battle_only, newPokemonStats.game_index);

            pokemonStats.push_back(stat);

            myline = "";
           
        }
        // for(auto stat: allStats){
        //     cout << stat.toString() << endl;
        // }
        myFile_handler.close();
     
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

        
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int id;
            int damage_class_id;
            string identifier;
            int is_battle_only;
            int game_index;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.damage_class_id = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, newPokemonStats.identifier, ',');
           
            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.is_battle_only = atoi(tempString.c_str());

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokemonStats.game_index = atoi(tempString.c_str());

            
            Stats stat(newPokemonStats.id, newPokemonStats.damage_class_id, newPokemonStats.identifier, newPokemonStats.is_battle_only, newPokemonStats.game_index);

            pokemonStats.push_back(stat);

            myline = "";
           
        }
        // for(auto stat: allStats){
        //     cout << stat.toString() << endl;
        // }
        envHandler.close();
        
     }



//    myFile_handler = "";
//   envHandler = "";
  myline = "";
  fileName = "pokemon_types";
  myFile_handler.open("/share/cs327/pokedex/pokedex/data/csv/" + fileName + ".csv");

   if(myFile_handler.is_open()){
      
        
        getline(myFile_handler, myline);
        while(getline(myFile_handler, myline)){
            int pokemon_id;
            int type_id;
            int slot;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.type_id = atoi(tempString.c_str());
           
            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.slot = atoi(tempString.c_str());

            
            pokemonTypes pokeType(newPokeTypes.pokemon_id, newPokeTypes.type_id, newPokeTypes.slot);

            pokeTypes.push_back(pokeType);

            myline = "";
           
        }
        // for(auto pokeType: pokeTypes){
        //     cout << pokeType.toString() << endl;
        // }
        myFile_handler.close();
   
        // cout << "Total lines: " << lines << endl;
     }
     else{
        string homePath;
        homePath = getenv("HOME");
        envHandler.open(homePath + "/.poke327/pokedex/pokedex/data/csv/" + fileName + ".csv");

      
        getline(envHandler, myline);
        while(getline(envHandler, myline)){
            int pokemon_id;
            int type_id;
            int slot;
            string tempString = "";
            // cout << myline << endl;
            stringstream inputString(myline);

            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.pokemon_id = atoi(tempString.c_str());
        

            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.type_id = atoi(tempString.c_str());
           
            tempString = "";
            getline(inputString, tempString, ',');
            if(tempString == ""){
                tempString = "214783647";
            }
            newPokeTypes.slot = atoi(tempString.c_str());

            
            pokemonTypes pokeType(newPokeTypes.pokemon_id, newPokeTypes.type_id, newPokeTypes.slot);

            pokeTypes.push_back(pokeType);

            myline = "";
           
        }
        // for(auto pokeType: pokeTypes){
        //     cout << pokeType.toString() << endl;
        // }
        envHandler.close();
        
     }

    }

};