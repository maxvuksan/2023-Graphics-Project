# libs 
ENET = -lenet -lws2_32 -lwinmm
SFML = -lsfml-graphics -lopengl32 -lsfml-network -lsfml-audio -lsfml-window -lsfml-system -DSFML_STATIC
LIBS = -pthread -Lsrc/lib $(ENET) $(SFML)

# source
AMBER = Amber/Core/*.cpp Amber/Networking/*.cpp Amber/Object/*.cpp Amber/Object/Rendering/*.cpp Amber/Object/Physics/*.cpp Amber/Object/UI/*.cpp Amber/Utility/*.cpp 
GAME = Game/Networking/*.cpp Game/Player/*.cpp Game/World/*.cpp Game/Pathfinding/*.cpp Game/*.cpp


# commands
run: compile execute


compile:
# g++ -o server.exe -Isrc/include main_server.cpp  $(AMBER) $(GAME) $(LIBS) 
	g++ -o client.exe -Isrc/include main.cpp  $(AMBER) $(GAME)  $(LIBS) 


