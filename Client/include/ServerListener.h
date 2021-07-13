//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SERVERLISTENER_H
#define BOOST_ECHO_CLIENT_SERVERLISTENER_H
#include <connectionHandler.h>
#include "UsersData.h"

using namespace std;

class ServerListener
{
public:
    ServerListener(ConnectionHandler** connectionHandler1,UsersData& usersData,mutex& socket_mutex);
    void run();
private:
    ConnectionHandler** connectionHandler;
    UsersData& data;
    mutex& socket_mutex;
};


#endif //BOOST_ECHO_CLIENT_SERVERLISTENER_H
