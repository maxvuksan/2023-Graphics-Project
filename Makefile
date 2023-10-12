run: compile execute

compile:
	g++ -o main.exe  -Isrc/include main.cpp  Amber/Core/*.cpp Amber/Object/*.cpp Amber/Object/Rendering/*.cpp Amber/Object/Physics/*.cpp Amber/Object/UI/*.cpp Amber/Utility/*.cpp Game/*.cpp -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -DSFML_STATIC 

execute: 
	./main.exe