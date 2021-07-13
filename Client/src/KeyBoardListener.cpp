//
// Created by amarmic@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <Frames.h>
#include <iostream>
#include <vector>
#include <connectionHandler.h>
#include <MessagesFromClientProtocol.h>
#include "UsersData.h"



using namespace std;



int main (int argc, char *argv[])
{

    UsersData data;
    ConnectionHandler* connectionHandler= nullptr;
    mutex socket_mutex;
    MessagesFromClientProtocol msgFromClientProtocol(data,socket_mutex);
    while(true)
    {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        cout.flush();
        if (line=="bye")
            break;
        string respose = msgFromClientProtocol.process(line,&connectionHandler);
        if (!respose.empty() )
        {
            if (data.getName()=="")
            {
                cout<<"login befor sending messages"<<endl;
            } else {
                lock_guard<mutex> lock(socket_mutex);
                connectionHandler->sendFrameAscii(respose, '\0');
            }
        }
    }
    if (connectionHandler!= nullptr)
        delete connectionHandler;
}



