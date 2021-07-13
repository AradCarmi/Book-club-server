//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MESSAGESFROMCLIENTPROTOCOL_H
#define BOOST_ECHO_CLIENT_MESSAGESFROMCLIENTPROTOCOL_H

#include "connectionHandler.h"
#include "Frames.h"
#include "UsersData.h"
#include <string>
#include <ServerListener.h>
#include <thread>

using namespace std;
class MessagesFromClientProtocol
{
public:
    MessagesFromClientProtocol &operator=(const MessagesFromClientProtocol &other);
    MessagesFromClientProtocol(const MessagesFromClientProtocol& clientProtocol) ;
    MessagesFromClientProtocol(UsersData& usersData,mutex& socket_mutex);
    Frame* StringToFrame(string line,ConnectionHandler** connectionHandler);
    string process(string msg,ConnectionHandler** connectionHandler);
    ~MessagesFromClientProtocol();
private:
    UsersData& data;
    vector<string> splitByWhiteSpace(string line);
    mutex& socket_mutex;
    std::thread* serverListenerThread;
};









#endif //BOOST_ECHO_CLIENT_MESSAGESFROMCLIENTPROTOCOL_H
