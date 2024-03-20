# libs 
ENET = -lenet -lws2_32 -lwinmm
SFML = -lsfml-graphics -lopengl32 -lsfml-network -lsfml-audio -lsfml-window -lsfml-system -DSFML_STATIC
STEAM = -lsteam_api
LIBS = -pthread -Lsrc/lib $(ENET) $(SFML) $(STEAM)

# static link libstdc++ and libgcc
LDFLAGS = -static-libgcc -static-libstdc++

# source
AMBER = Amber/Core/*.cpp Amber/Networking/*.cpp Amber/Object/*.cpp Amber/Object/Rendering/*.cpp Amber/Object/Physics/*.cpp Amber/Object/UI/*.cpp Amber/Utility/*.cpp 
GAME = Game/Items/*.cpp Game/Networking/*.cpp Game/Player/*.cpp Game/World/*.cpp Game/World/Lighting/*.cpp Game/Pathfinding/*.cpp Game/*.cpp Game/GameUI/*.cpp Game/Utility/*.cpp

AMBER_DIRS = Amber Amber/Networking Amber/Object Amber/Object Amber/Object/Rendering Amber/Object/Physics Amber/Object/UI Amber/Utility
GAME_DIRS = Game Game/Items Game/Networking Game/Player Game/World Game/World/Lighting Game/Pathfinding Game/GameUI Game/Utility
DIRS = $(AMBER_DIRS) $(GAME_DIRS)

# finding all directories ^ same as above
SRC = $(wildcard *.cpp) $(foreach DIR, $(DIRS), $(wildcard $(DIR)/*.cpp))

# stick the .object_files/ directory before the .cpp file and change the extension
OBJ = $(addprefix object_files/,$(SRC:.cpp=.o))

test:
	echo $(SRC)


object_files/%.o : %.cpp
        @mkdir -p $(@D)
        $(COMPILE.cpp) -o $@ $<

# commands ----------------------------------------------------------------

# -static flag to embed (statically link) dependencies into the .exe
build:
	g++ -static -o client.exe -Isrc/include main.cpp  $(AMBER) $(GAME) $(LIBS) $(LDFLAGS)

compile:
	g++ -o client.exe -Isrc/include main.cpp $(AMBER) $(GAME) $(LIBS) 

	

# compiles with debug symbols (allow gbd to see line numbers in its backtrace (bt) command)
debug:
	g++ -g -o client.exe -Isrc/include main.cpp  $(AMBER) $(GAME) $(LIBS) 


clean:
	echo "Removing .o files"
	rm *.o

# run with : gdb ./client.exe

