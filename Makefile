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


# commands ----------------------------------------------------------------

# -static flag to embed (statically link) dependencies into the .exe
build:
	g++ -static -o client.exe -Isrc/include main.cpp  $(AMBER) $(GAME) $(LIBS) $(LDFLAGS)

compile:
	g++ -o client.exe -Isrc/include main.cpp $(AMBER) $(GAME) $(LIBS) 

	


# compiles with debug symbols (allow gbd to see line numbers in its backtrace (bt) command)
debug:
	g++ -g -o client.exe -Isrc/include main.cpp  $(AMBER) $(GAME) $(LIBS) 

# run with : gdb ./client.exe

