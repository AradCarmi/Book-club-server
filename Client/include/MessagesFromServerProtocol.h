//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MESSAGESFROMSERVERPROTOCOL_H
#define BOOST_ECHO_CLIENT_MESSAGESFROMSERVERPROTOCOL_H
#include<condition_variable>
#include <connectionHandler.h>
#include <Frames.h>
#include "UsersData.h"

using namespace std;

class MessagesFromServerProtocol {
private:
    UsersData& data;
public:
    MessagesFromServerProtocol(UsersData& userData);
    string process(string msg,ConnectionHandler** connectionHandler);
    Frame* ExecuteFrame(string msg, ConnectionHandler** handler);
    Frame* StringToFrame(string msg);
    vector<string> StringToLines(string msg);
    vector<string> splitByWhiteSpace(string line);
    bool shouldTerminate;

};


#endif //BOOST_ECHO_CLIENT_MESSAGESFROMSERVERPROTOCOL_H
