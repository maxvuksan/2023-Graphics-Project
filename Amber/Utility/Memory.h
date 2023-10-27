#pragma once
#include <iostream>
#include "../Core/Globals.h"

/*
    A wrapper for new and delete heap interactions to track any possible memory problems.
*/
class Memory {

    public:

        template <typename T>
        static T* New(const char* source){

            T* memory = new T;

            if(Globals::PROFILE){

                std::cout << "New   : " << sizeof(*memory) << " b : " << source << "\n";
                total_size += sizeof(*memory);
                std::cout << "Total : " << total_size << " mb\n";
            }

            return memory;
        }

        template <typename T>
        static void Delete(T* memory, const char* source){

            if(Globals::PROFILE){

                total_size -= sizeof(*memory);
                std::cout << "Delete : " << sizeof(*memory) << " b : " << source << "\n";
                std::cout << "Total  : " << total_size << " mb\n";
            }

            delete memory;
        }

    private:
        static int allocations;
        static float total_size; // in mb
};