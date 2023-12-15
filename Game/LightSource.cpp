#include "LightSource.h"
#include "World/LightingManager.h"

void LightSource::Start(){
    LightingManager::AddLightSource(this);    
}

LightSource::~LightSource(){
    LightingManager::DeleteLightSource(this);
}