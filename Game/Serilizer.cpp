#include "Serilizer.h"
#include "Player/Inventory.h"
#include "World/World.h"
#include "Networking/GameClient.h"
#include "Utility/UtilityStation.h"

Datafile Serilizer::current_save;
bool Serilizer::autosaving = false;
int Serilizer::autosaving_chunk_x = 0;
int Serilizer::audosaving_chunk_y = 0;



std::string Serilizer::GetCollisionFreeFilename(std::string target_name, std::string extension, std::string directory){
    
    
    // replacing all spaces with underscores

    for(int i = 0; i < target_name.size(); i++){
        if(target_name[i] == ' '){
            target_name[i] = '_';
        }
    }
    
    std::string filename = target_name;
    
    int collision_count = 0;
    bool naming_collision = true;
    
    while(naming_collision){

        naming_collision = false;


        for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(directory)){

            if(directory + filename + extension == FilepathFromDirectoryEntry(dir_entry)){

                collision_count++;
                naming_collision = true;
                filename = target_name + "(" + std::to_string(collision_count) + ")";
                break;
            }
        }
    }
    return filename;
}

std::string Serilizer::FilepathFromDirectoryEntry(std::filesystem::__cxx11::directory_entry entry){
    // route dir_item into string stream
    std::stringstream ss;
    std::string output;

    ss << entry << std::endl;
    ss >> output;

    // substr to remove quotes
    return output.substr(1, output.size() - 2);
}

std::string Serilizer::ExtractExtension(std::string filename){

    // traverses from the end of the string until the '.' character is found
    for(int i = filename.size() - 1; i >= 0; i--){
        if(filename[i] == '.'){
            return filename.substr(i, filename.size());
        }
    }
    return "";

}


void Serilizer::CreateNewPlayer(std::string name){

    std::string filename = GetCollisionFreeFilename(name, ".player", Settings::SAVE_PATH + "/Players/");

    // player data
    Datafile pd;
    pd["name"].SetString(name);


    std::string coord_string;

    for(int y = 0; y < Inventory::GetRowCount(); y++){
        for(int x = 0; x < Inventory::GetRowLength(); x++){
            
            coord_string = "[" + std::to_string(x) + "]" + "[" + std::to_string(y) + "]";
            

            pd["inventory"][coord_string]["item_code"].SetInt(0);                    
            pd["inventory"][coord_string]["count"].SetInt(0);    
        }
    }

    // give default items
    pd["inventory"]["[0][0]"]["item_code"].SetInt(item_Copper_Picaxe);
    pd["inventory"]["[0][0]"]["count"].SetInt(1);  

    pd["inventory"]["[1][0]"]["item_code"].SetInt(item_Copper_Axe);
    pd["inventory"]["[1][0]"]["count"].SetInt(1);  

    pd["inventory"]["[2][0]"]["item_code"].SetInt(item_Copper_Sword);
    pd["inventory"]["[2][0]"]["count"].SetInt(1);  

    Datafile::Write(pd, Settings::SAVE_PATH + "/Players/" + filename + ".player");
}

void Serilizer::LoadPlayer(std::string filepath, GameClient* client){

    // player data
    Datafile pd;
    
    if(!Datafile::Read(pd, filepath)){
        std::cout << "ERROR : Failed to read player from " << filepath << " Serilizer::LoadWorldIntoMemory()\n"; 
    };

    std::string coord_string;

    Inventory* inventory = client->GetInventory();

    for(int y = 0; y < Inventory::GetRowCount(); y++){
        for(int x = 0; x < Inventory::GetRowLength(); x++){
            
            coord_string = "[" + std::to_string(x) + "]" + "[" + std::to_string(y) + "]";

            inventory->GetSlot(x, y)->item_code = (ItemCode)pd["inventory"][coord_string]["item_code"].GetInt(0);
            inventory->GetSlot(x, y)->count = pd["inventory"][coord_string]["count"].GetInt(0);
        }
    }
}

void Serilizer::SaveWorld(Serilizer::DataPair world_datapair, World* world){
    
    Datafile wd;

    wd["name"].SetString(world_datapair.name);

    // header
    wd["header"]["time of day"].SetInt(TimeManager::GetTimeOfDay());
    wd["header"]["width"].SetInt(world->GetWorldProfile()->width);
    wd["header"]["height"].SetInt(world->GetWorldProfile()->height);
    wd["header"]["spawnpoint"].SetInt(world->GetSpawnCoordinate().x, 0);
    wd["header"]["spawnpoint"].SetInt(world->GetSpawnCoordinate().y, 1);


    // loop through each chunk, saving tiles + other chunk specifics
    for(int chunk_x = 0; chunk_x < world->GetChunks()->size(); chunk_x++){
        for(int chunk_y = 0; chunk_y < world->GetChunks()->at(chunk_x).size(); chunk_y++){
            
            
            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);

            /*
            // ignore untouched chunks
            if(!chunk->GetSavingDirty()){
                continue;
            }
            chunk->SetSavingDirty(false);
            */

            std::string chunk_string = "chunk[" + std::to_string(chunk_x) + "]" + "[" + std::to_string(chunk_y) + "]";

            // save foliage
            int f = 0;
            wd["world"][chunk_string]["foliage"]["count"].SetInt(chunk->GetFoliageMap().size());
            for(auto& foliage : chunk->GetFoliageMap()){
                wd["world"][chunk_string]["foliage"]["[" + std::to_string(f) + "]"].SetInt(foliage.first, 0);
                wd["world"][chunk_string]["foliage"]["[" + std::to_string(f) + "]"].SetInt(foliage.second, 1);
                f++;
            }

            UtilityStation* station;
            wd["world"][chunk_string]["utility"]["count"].SetInt(chunk->GetUtilityStations()->size());
            for(int i = 0; i < chunk->GetUtilityStations()->size(); i++){
                station = chunk->GetUtilityStations()->at(i);
                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["type"].SetInt(station->GetItemCode());

                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].SetInt(station->GetTransform()->position.x, 0);
                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].SetInt(station->GetTransform()->position.y, 1);
            }


            // save each tile layer
            for(int i = 0; i < 3; i++){
                // select the correct tilemap
                Tilemap* tilemap = tilemap = chunk->GetTilemap(SetLocation::MAIN);
                std::string tilemap_name = "m";

                switch(i){
                    case 1: {
                        tilemap_name = "b";
                        tilemap = chunk->GetTilemap(SetLocation::BACKGROUND);
                        break;
                    }
                    case 2: {
                        tilemap_name = "f";
                        tilemap = chunk->GetTilemap(SetLocation::FOREGROUND);
                        break;
                    }
                }

                // loop through each tile
                for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){
                    
                    // rather than storing each tile, we can use run length encoding to reduce duplicates
                    /*
                        e.g.

                        dirt, dirt, dirt, dirt, dirt ... could become 5*dirt
                    */

                    std::string row_name = "[" + std::to_string(y) + "]";

                    int type_counter = 1;
                    int array_index = 0;
                    signed_byte current_tile = tilemap->GetTile(0, y);

                    for(int x = 1; x < world->GetWorldProfile()->tilemap_profile.width; x++){

                        signed_byte new_tile = tilemap->GetTile(x, y);
                        
                        if(new_tile == current_tile){
                            type_counter++;
                        }
                        
                        // right set
                        if(new_tile != current_tile || x == world->GetWorldProfile()->tilemap_profile.width - 1){

                            std::string encoded_set = std::to_string(type_counter) + "*" + std::to_string(current_tile);
                            
                            wd["world"][chunk_string]["rows"][row_name][tilemap_name].SetString(encoded_set, array_index);

                            type_counter = 1;
                            array_index++;
                        }

                        current_tile = new_tile;
                    }
                }

            }
        }
    }

    // saving minimap explored layer 
    for(int chunk_y = 0; chunk_y < world->GetChunks()->at(0).size(); chunk_y++){

        // loop through each tile
        for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){
            
            std::string row_name = "[" + std::to_string(y + chunk_y * world->GetWorldProfile()->tilemap_profile.height) + "]";

            int type_counter = 1;
            int array_index = 0;
            int current_alpha = world->GetMinimap()->GetExploredPixelGrid()->GetPixel(0, y + chunk_y * world->GetWorldProfile()->tilemap_profile.height).a;
            int total_type = 0;

            for(int chunk_x = 0; chunk_x < world->GetChunks()->size(); chunk_x++){
                for(int x = 0; x < world->GetWorldProfile()->tilemap_profile.width; x++){

                    if(chunk_x == 0 && x == 0){
                        continue;
                    }

                    int new_alpha = world->GetMinimap()->GetExploredPixelGrid()->GetPixel(x + chunk_x * world->GetWorldProfile()->tilemap_profile.width, 
                                                                                                y + chunk_y * world->GetWorldProfile()->tilemap_profile.height).a;
                
                    if(new_alpha == current_alpha){
                        type_counter++;
                    }
                    
                    // right set
                    if(new_alpha != current_alpha || x + chunk_x * world->GetWorldProfile()->tilemap_profile.width == world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->width - 1){

                        std::string encoded_set = std::to_string(type_counter) + "*" + std::to_string(current_alpha);
                        
                        wd["minimap"]["rows"][row_name].SetString(encoded_set, array_index);
                        total_type += type_counter;

                        if(total_type > world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->width){
                            std::cout << "type overflow: " << total_type << "\n";
                        }

                        type_counter = 1;
                        array_index++;
                    }

                    current_alpha = new_alpha;

                }
            }
        }

    }

    Datafile::Write(wd, world_datapair.filepath);
}

void Serilizer::SaveStructure(Serilizer::DataPair structure_datapair, World* world){

    Datafile wd;

    wd["name"].SetString(structure_datapair.name);

    // header
    wd["header"]["width"].SetInt(world->GetWorldProfile()->width);
    wd["header"]["height"].SetInt(world->GetWorldProfile()->height);

    // loop through each chunk, saving tiles + other chunk specifics
    for(int chunk_x = 0; chunk_x < world->GetChunks()->size(); chunk_x++){
        for(int chunk_y = 0; chunk_y < world->GetChunks()->at(chunk_x).size(); chunk_y++){
            
            
            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);

            std::string chunk_string = "chunk[" + std::to_string(chunk_x) + "]" + "[" + std::to_string(chunk_y) + "]";

            // save foliage
            int f = 0;
            wd["world"][chunk_string]["foliage"]["count"].SetInt(chunk->GetFoliageMap().size());
            for(auto& foliage : chunk->GetFoliageMap()){
                wd["world"][chunk_string]["foliage"]["[" + std::to_string(f) + "]"].SetInt(foliage.first, 0);
                wd["world"][chunk_string]["foliage"]["[" + std::to_string(f) + "]"].SetInt(foliage.second, 1);
                f++;
            }

            UtilityStation* station;
            wd["world"][chunk_string]["utility"]["count"].SetInt(chunk->GetUtilityStations()->size());
            for(int i = 0; i < chunk->GetUtilityStations()->size(); i++){
                station = chunk->GetUtilityStations()->at(i);
                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["type"].SetInt(station->GetItemCode());

                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].SetInt(station->GetTransform()->position.x, 0);
                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].SetInt(station->GetTransform()->position.y, 1);
            }


            // save each tile layer
            for(int i = 0; i < 3; i++){
                // select the correct tilemap
                Tilemap* tilemap = tilemap = chunk->GetTilemap(SetLocation::MAIN);
                std::string tilemap_name = "m";

                switch(i){
                    case 1: {
                        tilemap_name = "b";
                        tilemap = chunk->GetTilemap(SetLocation::BACKGROUND);
                        break;
                    }
                    case 2: {
                        tilemap_name = "f";
                        tilemap = chunk->GetTilemap(SetLocation::FOREGROUND);
                        break;
                    }
                }

                // loop through each tile
                for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){
                    
                    // rather than storing each tile, we can use run length encoding to reduce duplicates
                    /*
                        e.g.

                        dirt, dirt, dirt, dirt, dirt ... could become 5*dirt
                    */

                    std::string row_name = "[" + std::to_string(y) + "]";

                    int type_counter = 1;
                    int array_index = 0;
                    signed_byte current_tile = tilemap->GetTile(0, y);

                    for(int x = 1; x < world->GetWorldProfile()->tilemap_profile.width; x++){

                        signed_byte new_tile = tilemap->GetTile(x, y);
                        
                        if(new_tile == current_tile){
                            type_counter++;
                        }
                        
                        // right set
                        if(new_tile != current_tile || x == world->GetWorldProfile()->tilemap_profile.width - 1){

                            std::string encoded_set = std::to_string(type_counter) + "*" + std::to_string(current_tile);
                            
                            wd["world"][chunk_string]["rows"][row_name][tilemap_name].SetString(encoded_set, array_index);

                            type_counter = 1;
                            array_index++;
                        }

                        current_tile = new_tile;
                    }
                }

            }
        }
    }

    Datafile::Write(wd, structure_datapair.filepath);

    std::cout << structure_datapair.filepath << "\n";
    std::cout << "saved structure\n";

}

void Serilizer::LoadStructureAsWorld(std::string filename, World* world){

    Datafile wd;
    
    if(!Datafile::Read(wd, filename)){
        std::cout << "ERROR : Failed to read structure from " << filename << " Serilizer::LoadWorldIntoMemory()\n"; 
        return;
    };

    int width = wd["header"]["width"].GetInt();
    int height = wd["header"]["height"].GetInt();

    world->Create(false, width, height);
    
    // loop through each chunk
    for(int chunk_x = 0; chunk_x < width; chunk_x++){
        for(int chunk_y = 0; chunk_y < height; chunk_y++){
            
            std::string chunk_string = "chunk[" + std::to_string(chunk_x) + "]" + "[" + std::to_string(chunk_y) + "]";
            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);


            // load foliage --------------------------------------------------------------------
            int count = wd["world"][chunk_string]["foliage"]["count"].GetInt();
            for(int i = 0; i < count; i++){
                int map_val = wd["world"][chunk_string]["foliage"]["[" + std::to_string(i) + "]"].GetInt(0);
                Foliage type = (Foliage)wd["world"][chunk_string]["foliage"]["[" + std::to_string(i) + "]"].GetInt(1);
            
                chunk->AddFoliageViaMapIndex(type, map_val);
            }


            // for each layer
            for(int i = 0; i < 3; i++){
                

                Tilemap* tilemap = chunk->GetTilemap(SetLocation::MAIN);
                std::string tilemap_name = "m";
                switch(i){
                    case 1: {
                        tilemap_name = "b";
                        tilemap = chunk->GetTilemap(SetLocation::BACKGROUND);
                        break;
                    }
                    case 2: {
                        tilemap_name = "f";
                        tilemap = chunk->GetTilemap(SetLocation::FOREGROUND);
                        break;
                    }
                }

                if(tilemap == nullptr){
                    std::cout << "ERROR: tilemap variable is nullptr, Serilizer::LoadWorldIntoMemory\n";
                }


                // loop through each row
                for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){

                    std::string row_name = "[" + std::to_string(y) + "]";

                    int index = 0;
                    std::string result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                    index++;
                    int x_step = 0;


                    while(result != "" && index < world->GetWorldProfile()->tilemap_profile.width && x_step < world->GetWorldProfile()->tilemap_profile.width){

                        int length_of_type = std::stoi(result.substr(0, result.find("*")));
                        int type = std::stoi(result.substr(result.find("*") + 1, result.length()));

                        if(x_step + length_of_type > world->GetWorldProfile()->tilemap_profile.width){
                            std::cout << "row length overflow, skipping remaining row\n";
                            break;
                        }

                        tilemap->SetArea((signed_byte)type, x_step, x_step + length_of_type, y, y + 1);

                        result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                        
                        x_step += length_of_type;
                        index++;

                    }

                }
            }

        }
    }
}

bool Serilizer::SpawnStructureIntoWorld(std::string filename, sf::Vector2i coordinate_to_spawn, World* world){

    Datafile wd;
    
    if(!Datafile::Read(wd, Settings::SAVE_PATH + "/Structures/" + filename + ".structure")){
        std::cout << "ERROR : Failed to read structure from " << filename << " Serilizer::LoadWorldIntoMemory()\n"; 
        return false;
    };

    int width = wd["header"]["width"].GetInt();
    int height = wd["header"]["height"].GetInt();
    
    // loop through each chunk
    for(int chunk_x = 0; chunk_x < width; chunk_x++){
        for(int chunk_y = 0; chunk_y < height; chunk_y++){
            
            std::string chunk_string = "chunk[" + std::to_string(chunk_x) + "]" + "[" + std::to_string(chunk_y) + "]";

            // for each layer
            for(int i = 0; i < 3; i++){
                

                SetLocation set_location = SetLocation::MAIN;
                std::string tilemap_name = "m";
                
                switch(i){
                    case 1: {
                        tilemap_name = "b";
                        set_location = SetLocation::BACKGROUND;
                        break;
                    }
                    case 2: {
                        tilemap_name = "f";
                        set_location = SetLocation::FOREGROUND;
                        break;
                    }
                }

                // loop through each row
                for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){

                    std::string row_name = "[" + std::to_string(y) + "]";

                    int index = 0;
                    std::string result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                    index++;
                    int x_step = 0;


                    while(result != "" && index < 50 && x_step < 50){

                        int length_of_type = std::stoi(result.substr(0, result.find("*")));
                        int type = std::stoi(result.substr(result.find("*") + 1, result.length()));

                        if(x_step + length_of_type > 50){
                            std::cout << "row length overflow, skipping remaining row\n";
                            break;
                        }

                        for(unsigned int x = x_step; x < x_step + length_of_type; x++){

                            sf::Vector2i coordinate = coordinate_to_spawn + sf::Vector2i(x + chunk_x * world->GetWorldProfile()->tilemap_profile.width, y + chunk_y * world->GetWorldProfile()->tilemap_profile.height);
                            if(!world->CoordInBounds(coordinate.x, coordinate.y)){
                                break;
                            }
                            world->SetTile((signed_byte)type, coordinate.x, coordinate.y, set_location, SetMode::OVERRIDE, true, true, false);         
                        }

                        result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                        
                        x_step += length_of_type;
                        index++;

                    }

                }
            }

        }
    }
    return true;
}

void Serilizer::CreateNewWorld(std::string name, World* world){

    std::string filename = GetCollisionFreeFilename(name, ".world", Settings::SAVE_PATH + "/Worlds/");
    TimeManager::SetTimeOfDay(1200);
    SaveWorld({name, Settings::SAVE_PATH + "/Worlds/" + filename + ".world"}, world);
}

void Serilizer::CreateNewStructure(std::string name, World* world){

    std::string filename = GetCollisionFreeFilename(name, ".structure", Settings::SAVE_PATH + "/Structures/");
    SaveStructure({name, Settings::SAVE_PATH + "/Structures/" + filename + ".structure"}, world);
}

void Serilizer::LoadWorldIntoMemory(std::string filename, World* world){
    
    Datafile wd;
    
    if(!Datafile::Read(wd, filename)){
        std::cout << "ERROR : Failed to read world from " << filename << " Serilizer::LoadWorldIntoMemory()\n"; 
        return;
    };

    int width = wd["header"]["width"].GetInt();
    int height = wd["header"]["height"].GetInt();
    int spawn_x = wd["header"]["spawnpoint"].GetInt(0);
    int spawn_y = wd["header"]["spawnpoint"].GetInt(1);

    TimeManager::SetTimeOfDay(wd["header"]["time of day"].GetInt());

    world->Create(false, width, height);
    world->SetSpawnCoordinate(spawn_x, spawn_y);


    std::cout << "world created, looking at chunks\n";

    // loop through each chunk
    for(int chunk_x = 0; chunk_x < width; chunk_x++){
        for(int chunk_y = 0; chunk_y < height; chunk_y++){
            
            std::string chunk_string = "chunk[" + std::to_string(chunk_x) + "]" + "[" + std::to_string(chunk_y) + "]";
            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);


            // load foliage --------------------------------------------------------------------
            int count = wd["world"][chunk_string]["foliage"]["count"].GetInt();
            for(int i = 0; i < count; i++){
                int map_val = wd["world"][chunk_string]["foliage"]["[" + std::to_string(i) + "]"].GetInt(0);
                Foliage type = (Foliage)wd["world"][chunk_string]["foliage"]["[" + std::to_string(i) + "]"].GetInt(1);
            
                chunk->AddFoliageViaMapIndex(type, map_val);
            }


            // for each layer
            for(int i = 0; i < 3; i++){
                

                Tilemap* tilemap = chunk->GetTilemap(SetLocation::MAIN);
                std::string tilemap_name = "m";
                switch(i){
                    case 1: {
                        tilemap_name = "b";
                        tilemap = chunk->GetTilemap(SetLocation::BACKGROUND);
                        break;
                    }
                    case 2: {
                        tilemap_name = "f";
                        tilemap = chunk->GetTilemap(SetLocation::FOREGROUND);
                        break;
                    }
                }

                if(tilemap == nullptr){
                    std::cout << "ERROR: tilemap variable is nullptr, Serilizer::LoadWorldIntoMemory\n";
                }


                // loop through each row
                for(int y = 0; y < world->GetWorldProfile()->tilemap_profile.height; y++){

                    std::string row_name = "[" + std::to_string(y) + "]";

                    int index = 0;
                    std::string result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                    index++;
                    int x_step = 0;


                    while(result != "" && index < world->GetWorldProfile()->tilemap_profile.width && x_step < world->GetWorldProfile()->tilemap_profile.width){

                        int length_of_type = std::stoi(result.substr(0, result.find("*")));
                        int type = std::stoi(result.substr(result.find("*") + 1, result.length()));

                        if(x_step + length_of_type > world->GetWorldProfile()->tilemap_profile.width){
                            std::cout << "row length overflow, skipping remaining row\n";
                            break;
                        }

                        tilemap->SetArea((signed_byte)type, x_step, x_step + length_of_type, y, y + 1);

                        result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                        
                        x_step += length_of_type;
                        index++;

                    }

                }
            }

        }
    }

}

std::vector<Serilizer::DataPair> Serilizer::LoadPlayerList(){

    std::vector<DataPair> found_players;

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(Settings::SAVE_PATH + "/Players/")){

        std::string filepath = FilepathFromDirectoryEntry(dir_entry);

        if(ExtractExtension(filepath) == ".player"){

            Datafile pd;

            if(!Datafile::Read(pd, filepath, "header")){
                std::cout << "ERROR : Could not read from file: " << filepath << " Serilizer::LoadPlayerList\n";
            };

            found_players.push_back({pd["name"].GetString(), filepath});
        }
    }
    return found_players;

}

std::vector<Serilizer::DataPair> Serilizer::LoadWorldList(){

    std::vector<DataPair> found_worlds;

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(Settings::SAVE_PATH + "/Worlds/")){

        std::string filepath = FilepathFromDirectoryEntry(dir_entry);

        if(ExtractExtension(filepath) == ".world"){

            Datafile wd;

            if(!Datafile::Read(wd, filepath, "header")){
                std::cout << "ERROR : Could not read from file: " << filepath << " Serilizer::LoadWorldList\n";
            };

            found_worlds.push_back({wd["name"].GetString(), filepath});
        }
    }
    return found_worlds;
}

std::vector<Serilizer::DataPair> Serilizer::LoadStructureList(){

    std::vector<DataPair> found_structures;

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(Settings::SAVE_PATH + "/Structures/")){

        std::string filepath = FilepathFromDirectoryEntry(dir_entry);

        if(ExtractExtension(filepath) == ".structure"){

            Datafile wd;

            if(!Datafile::Read(wd, filepath, "header")){
                std::cout << "ERROR : Could not read from file: " << filepath << " Serilizer::LoadStructureList\n";
            };

            found_structures.push_back({wd["name"].GetString(), filepath});
        }
    }
    return found_structures;
}