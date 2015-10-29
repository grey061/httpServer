CC=g++

CFLAGS=-std=c++11 -pthread

all:
	$(CC) $(CFLAGS) src/main.cpp src/ServerSocket.cpp src/Server.cpp src/WebServer.cpp src/ClientHandler.cpp src/WebClientHandler.cpp -o main
