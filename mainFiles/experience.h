#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
class Experience{
public:

    int growth_rate_id;
    int level;
    int experience;


    Experience(){};
    Experience(int growth_rate_id, int level, int experience){
        this->growth_rate_id = growth_rate_id;
        this->level = level;
        this->experience = experience;
    
    }

   


    ~Experience(){}

    string toString() const{
        return "growth_rate_id: " + to_string(growth_rate_id) + "\n"
            + "level: " + to_string(level) + "\n"
            + "experience: " + to_string(experience) + "\n";
          
           
    } 
};




