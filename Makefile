# libs 
ENET = -lenet -lws2_32 -lwinmm
SFML = -lsfml-graphics -lopengl32 -lsfml-network -lsfml-audio -lsfml-window -lsfml-system -DSFML_STATIC
LIBS = -Lsrc/lib $(ENET) $(SFML)

# source
AMBER = Amber/Core/*.cpp Amber/Object/*.cpp Amber/Object/Rendering/*.cpp Amber/Object/Physics/*.cpp Amber/Object/UI/*.cpp Amber/Utility/*.cpp

# commands
run: compile execute

compile:
	g++ -o main.exe  -Isrc/include main.cpp  $(AMBER) Game/*.cpp  $(LIBS) 

compile_network_test:
	g++ -o server.exe -Isrc/include Amber/Networking/server_main.cpp $(LIBS)
	g++ -o client.exe -Isrc/include Amber/Networking/client_main.cpp  $(LIBS)
		
execute: 
	./main.exe