CC=g++

CFLAGS=-std=c++11 -pthread

all:
	$(CC) $(CFLAGS) src/main.cpp src/ServerSocket.cpp src/ClientSocket.cpp src/Server.cpp src/ClientHandler.cpp src/ClientHandlerPool.cpp src/ServerController.cpp -o main
