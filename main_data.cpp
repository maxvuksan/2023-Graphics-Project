#include "Datafile.h"
#include <iostream>


// testing datafile class

int main(){

    Datafile df;
    df["node"]["name"].SetString("max");
    df["node"]["age"].SetInt(20);
    
    df["node"]["chunk"]["width"].SetInt(50);
    df["node"]["chunk"]["height"].SetInt(50); 
    
    Datafile::Write(df, "save_test.txt");

    Datafile df2;
    Datafile::Read(df2, "save_test.txt");
    
    df2["test"].SetString("1");
    
    Datafile::Write(df2, "save_2.txt");

    return 0;
}