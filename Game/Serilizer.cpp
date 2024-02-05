#include "Serilizer.h"
#include "Player/Inventory.h"
#include "World/World.h"
#include "Networking/GameClient.h"
#include "Utility/UtilityStation.h"

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

    Time::StartRecord();
    // read exisiting 
    Datafile::Read(wd, world_datapair.filepath);
    Time::EndRecord();

    wd["name"].SetString(world_datapair.name);

    // header
    wd["header"]["time of day"].SetInt(TimeManager::GetTimeOfDay());
    wd["header"]["width"].SetInt(world->GetWorldProfile()->width);
    wd["header"]["height"].SetInt(world->GetWorldProfile()->height);
    wd["header"]["spawnpoint"].SetInt(world->GetSpawnCoordinate().x, 0);
    wd["header"]["spawnpoint"].SetInt(world->GetSpawnCoordinate().y, 1);

    Time::StartRecord();
    Datafile::Write(wd, world_datapair.filepath);
    Time::EndRecord();

    std::cout << sizeof(wd) << "\n";

    // loop through each chunk
    for(int chunk_x = 0; chunk_x < world->GetChunks()->size(); chunk_x++){
        for(int chunk_y = 0; chunk_y < world->GetChunks()->at(chunk_x).size(); chunk_y++){
            
            
            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);

            // ignore untouched chunks
            if(!chunk->GetSavingDirty()){
                continue;
            }
            chunk->SetSavingDirty(false);

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
                Tilemap* tilemap;
                std::string tilemap_name;

                switch(i){
                    case 0: {
                        tilemap_name = "m";
                        tilemap = chunk->GetTilemap(SetLocation::MAIN);
                        break;
                    }
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


    Datafile::Write(wd, world_datapair.filepath);
}

void Serilizer::CreateNewWorld(std::string name, World* world){

    std::string filename = GetCollisionFreeFilename(name, ".world", Settings::SAVE_PATH + "/Worlds/");
    TimeManager::SetTimeOfDay(1200);
    SaveWorld({name, Settings::SAVE_PATH + "/Worlds/" + filename + ".world"}, world);

}

void Serilizer::LoadWorldIntoMemory(std::string filepath, World* world){
    
    Datafile wd;
    
    if(!Datafile::Read(wd, filepath)){
        std::cout << "ERROR : Failed to read world from " << filepath << " Serilizer::LoadWorldIntoMemory()\n"; 
    };

    int width = wd["header"]["width"].GetInt();
    int height = wd["header"]["height"].GetInt();
    int spawn_x = wd["header"]["spawnpoint"].GetInt(0);
    int spawn_y = wd["header"]["spawnpoint"].GetInt(1);

    TimeManager::SetTimeOfDay(wd["header"]["time of day"].GetInt());

    world->Create(false, width, height);
    world->SetSpawnCoordinate(spawn_x, spawn_y);

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

            // load utility --------------------------------------------------------------------

/*
            UtilityStation* station;
            count = wd["world"][chunk_string]["utility"]["count"].GetInt();
            for(int i = 0; i < count; i++){

                station = chunk->GetUtilityStations()->at(i);
                ItemCode item_code = (ItemCode)wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["type"].GetInt();

                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].GetInt(0) 
                wd["world"][chunk_string]["utility"]["[" + std::to_string(i) + "]"]["pos"].SetInt(0);
            }
*/


            // for each layer
            for(int i = 0; i < 3; i++){
                
                Tilemap* tilemap;
                std::string tilemap_name;
                switch(i){
                    case 0: {
                        tilemap_name = "m";
                        tilemap = chunk->GetTilemap(SetLocation::MAIN);
                        break;
                    }
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

                        tilemap->SetArea((signed_byte)type, x_step, x_step + length_of_type, y, y + 1);

                        result = wd["world"][chunk_string]["rows"][row_name][tilemap_name].GetString(index);
                        
                        x_step += length_of_type;
                        index++;
                    }
                }
            }

        }
    }

    world->CalculateMinimap();
}

std::vector<Serilizer::DataPair> Serilizer::LoadPlayerList(){

    std::vector<DataPair> found_players;

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(Settings::SAVE_PATH + "/Players/")){

        std::string filepath = FilepathFromDirectoryEntry(dir_entry);

        if(ExtractExtension(filepath) == ".player"){

            Datafile pd;

            if(!Datafile::Read(pd, filepath)){
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

            if(!Datafile::Read(wd, filepath)){
                std::cout << "ERROR : Could not read from file: " << filepath << " Serilizer::LoadWorldList\n";
            };

            found_worlds.push_back({wd["name"].GetString(), filepath});
        }
    }
    return found_worlds;
}