#include "Settings.h"

std::string Settings::SAVE_PATH = "Data"; //where things are saved

float Settings::LIGHT_BLUR_FACTOR = 2.0f;
int Settings::LIGHT_REFRESH_RATE = 10; // values could range from say 5 - 100 

float Settings::_ACTIVE_LIGHT_BLUR_FACTOR = Settings::LIGHT_BLUR_FACTOR;

bool Settings::AMBIENT_OCCLUSION = true;
float Settings::AMBIENT_OCCLUSION_BLUR_FACTOR = 2.0f;
int Settings::AMBIENT_OCCLUSION_ALPHA = 130;

int Settings::CAMERA_CHUNK_BLENDING = 3;