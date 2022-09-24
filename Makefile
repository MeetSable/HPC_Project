all: compile

compile:
	g++ -std=c++20 -Ofast -I./include ./src/*.cpp -o ./builds/main.exe
