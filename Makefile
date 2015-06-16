CC=g++
CFLAGS=-Iinclude
LIBS=-lSDL -lSDL_ttf -lSDL_image

invaders: src/* include/*
	 $(CC) -o invaders src/main.cpp src/Shield.cpp src/Alien.cpp src/Ship.cpp src/Missile.cpp src/AlienMissile.cpp $(CFLAGS) $(LIBS)
