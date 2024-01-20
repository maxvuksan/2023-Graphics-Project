#include "LightSource.h"
#include "LightingManager.h"

void LightSource::Start(){
    LightingManager::AddLightSource(this);    
}

LightSource::~LightSource(){
    LightingManager::DeleteLightSource(this);
}