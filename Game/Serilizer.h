#pragma once
#include "../Amber/Framework.h"

class Serilizer {

    public:

        static void CreateNewPlayer(std::string name){

            // player data
            Datafile pd;
            pd["name"].SetString(name);


            std::string coord_string;

            for(int y = 0; y < Inventory::GetRowCount(); y++){
                for(int x = 0; x < Inventory::GetRowLength(); x++){
                    
                    coord_string = "[" + std::to_string(x) + "]" + "[" + std::to_string(y) + "]";
                    

                    pd["inventory"][coord_string]["item_code"].SetInt(0, 0);                    
                    pd["inventory"][coord_string]["count"].SetInt(0, 0);    
                }
            }

            Datafile::Write(pd, "Data/Players/" + name);
        }
        

};