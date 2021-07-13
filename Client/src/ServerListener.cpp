//

#include "ServerListener.h"
#include "MessagesFromServerProtocol.h"

ServerListener::ServerListener( ConnectionHandler** connectionHandler1,UsersData& userdata,mutex& socket_mutex) :connectionHandler(connectionHandler1) , data(userdata),socket_mutex(socket_mutex)
{
}

void ServerListener::run()
{
    MessagesFromServerProtocol messagesFromServerProtocol(data);
    while (!messagesFromServerProtocol.shouldTerminate)
    {
        string frameRecieved;
        if (!(*connectionHandler)->getFrameAscii(frameRecieved,'\0'))
        {
            cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        string FrameToSend=messagesFromServerProtocol.process(frameRecieved,connectionHandler);
        if (!FrameToSend.empty())
        {
            lock_guard<mutex> lock(socket_mutex);
            (*connectionHandler)->sendFrameAscii(FrameToSend,'\0');
        }
    }
}