#include "Amber/Utility/Datafile.h"
#include "Amber/Utility/Time.h"
#include <iostream>


// testing datafile speed

int main(){


    Time::StartRecord();

    Datafile df;
    Datafile::Read(df, "Data/Worlds/my_world.txt");

    Datafile::Write(df, "Data/Worlds/my_world.txt");

    Time::EndRecord();

    return 0;
}