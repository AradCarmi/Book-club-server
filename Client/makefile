all: StompBookClubClient

StompBookClubClient:bin/connectionHandler.o bin/Frames.o bin/KeyBoardListener.o bin/MessagesFromClientProtocol.o bin/MessagesFromServerProtocol.o bin/ServerListener.o bin/UsersData.o
	@echo 'building target client'
	g++ -g -Wall -Weffc++ -std=c++11 -o  bin/StompBookClubClient bin/connectionHandler.o bin/Frames.o bin/KeyBoardListener.o bin/MessagesFromClientProtocol.o bin/MessagesFromServerProtocol.o bin/ServerListener.o bin/UsersData.o -lboost_system -lpthread
	@echo 'finished building target'

bin/connectionHandler.o:src/connectionHandler.cpp
	@echo 'compiling main'

	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Frames.o:src/Frames.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Frames.o src/Frames.cpp

bin/KeyBoardListener.o:src/KeyBoardListener.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/KeyBoardListener.o src/KeyBoardListener.cpp

bin/MessagesFromClientProtocol.o:src/MessagesFromClientProtocol.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/MessagesFromClientProtocol.o src/MessagesFromClientProtocol.cpp

bin/MessagesFromServerProtocol.o:src/MessagesFromServerProtocol.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/MessagesFromServerProtocol.o src/MessagesFromServerProtocol.cpp

bin/ServerListener.o:src/ServerListener.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/ServerListener.o src/ServerListener.cpp

bin/UsersData.o:src/UsersData.cpp
	@echo 'compiling main'
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/UsersData.o src/UsersData.cpp

#clean previos compiles
clean:
	@echo 'cleaning build path'
	rm -f bin/*