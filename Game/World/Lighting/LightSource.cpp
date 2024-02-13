#include "LightSource.h"
#include "LightingManager.h"

void LightSource::Start(){
    std::cout << "light source added\n";
    LightingManager::AddLightSource(this);    
}

LightSource::~LightSource(){
    LightingManager::DeleteLightSource(this);
}