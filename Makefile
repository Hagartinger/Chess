SDL_Stuff = -Ilib\SDL\include\SDL2 -Llib\SDL\lib -w  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

Static_Stuff = -static-libgcc -static-libstdc++


all: 
	g++  src\main.cpp src\figures.cpp src\game.cpp src\TextureHandler.cpp src\Board.cpp -Iheaders $(SDL_Stuff) $(Static_Stuff) -o bin\chess
	
debug: 
	g++ -g src\main.cpp src\figures.cpp src\game.cpp src\TextureHandler.cpp src\Board.cpp -Iheaders $(SDL_Stuff) -o bin\chess