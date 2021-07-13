//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <connectionHandler.h>
#include <MessagesFromClientProtocol.h>
#include <thread>



/* deals with user inputs and acts accordingly*/
MessagesFromClientProtocol::MessagesFromClientProtocol(UsersData& usersData,mutex& socket_mutex) : data(usersData),socket_mutex(socket_mutex),serverListenerThread(nullptr)
{
}

MessagesFromClientProtocol::MessagesFromClientProtocol(const MessagesFromClientProtocol & clientProtocol) : data(clientProtocol.data),socket_mutex(clientProtocol.socket_mutex),serverListenerThread(nullptr)
{}

Frame* MessagesFromClientProtocol::StringToFrame(string line, ConnectionHandler** connectionHandler)
{
        Frame* currentFrame;
        vector<string> tokens=splitByWhiteSpace(line);
        string commandName=tokens[0];
        if (commandName.compare("login")==0)
        {
            currentFrame = new CONNECT();
            string host=tokens[1].substr(0,tokens[1].find(":"));
            string port=tokens[1].substr(tokens[1].find(":")+1);
            string username=tokens[2];
            string password=tokens[3];
            currentFrame->addheader("login",username);
            currentFrame->addheader("passcode",password);
            currentFrame->addheader("host",host);
            currentFrame->addheader("accept-version","1.2");
            currentFrame->setBody("");
            short port2=stoi(port);
            if ((*connectionHandler)== nullptr)
            {
                (*connectionHandler) = new ConnectionHandler(host, port2);
                if (!(*connectionHandler)->connect())
                {
                    cout << "Could not connect to server" << endl;
                    delete currentFrame;
                    return nullptr;
                }
                data.setName(username);
                if (serverListenerThread!=nullptr)
                {
                    serverListenerThread->join();
                    delete (serverListenerThread);
                }
                ServerListener serverListener(connectionHandler,data,socket_mutex);
                serverListenerThread=new thread(&ServerListener::run, serverListener);
            }
        }
        else if (commandName.compare("join")==0)
        {
            currentFrame = new SUBSCRIBE();
            string destination=tokens[1];
            int subscriptionId=data.getLastSubscriptionId();
            data.incLastSubscriptionId();
            int receiptId=data.getLastReceiptId();
            data.incLastReceiptId();
            data.Subscribe(subscriptionId,destination);
            currentFrame->addheader("destination",destination);
            currentFrame->addheader("id",to_string(subscriptionId));
            currentFrame->addheader("receipt",to_string(receiptId));
            currentFrame->setBody("");
            data.addReceipt(receiptId,currentFrame);
        }
        else if (commandName.compare("add")==0)
        {
            currentFrame = new SEND();
            string destination = tokens[1];
            string bookName = tokens[2];
            for (unsigned int i=3;i<tokens.size();i++)
                bookName=bookName +" "+tokens[i];
            currentFrame->addheader("destination",destination);
            if (data.hasBook(bookName,destination))
            {
                cout<<bookName + " already exists in your inventory"<<endl;
                delete (currentFrame);
                return nullptr;
            }
            data.addBookToInventory(destination,bookName);
            currentFrame->setBody(data.getName()+ " has added the book "+ bookName);
        }
        else if (commandName.compare("borrow")==0)
        {
            currentFrame = new SEND();
            string destination = tokens[1];
            string bookName =tokens[2];
            for (unsigned int i=3;i<tokens.size();i++)
                bookName=bookName+" "+tokens[i];
            if (data.hasBook(bookName,destination))
            {
                cout<<bookName + " already exists in your inventory"<<endl;
                delete (currentFrame);
                return nullptr;
            }
            currentFrame->addheader("destination",destination);
            currentFrame->setBody(data.getName()+" wish to borrow "+ bookName);
            data.AddToWishList(bookName);
        }
        else if (commandName.compare("return")==0)
        {
            currentFrame = new SEND();
            string destination=tokens[1];
            string bookname=tokens[2];
            for (unsigned int i=3;i<tokens.size();i++)
                bookname=bookname+" " +tokens[i];
            currentFrame->addheader("destination",destination);
            currentFrame->setBody("Returning "+ bookname + " to "+ data.WhoIBorrowedTheBookFrom(bookname));
            data.returnBook(destination,bookname); //remove the book from the client inventory

        }
        else if (commandName.compare("status")==0)
        {
            currentFrame = new SEND();
            string destination=tokens[1];
            currentFrame->addheader("destination",destination);
            currentFrame->setBody("book status");
        }
        else if (commandName.compare("logout")==0)
        {
            currentFrame = new DISCONNECT();
            int receiptId=data.getLastReceiptId();
            currentFrame->addheader("receipt",to_string(receiptId));
            currentFrame->setBody("");
            data.incLastReceiptId();
            data.addReceipt(receiptId,currentFrame);
        }
        else if (commandName.compare("exit")==0)
        {
            currentFrame = new UNSUBSCRIBE();
            string genre=tokens[1];
            int receiptId=data.getLastReceiptId();
            currentFrame->addheader("id",to_string(data.UnSubscribe(genre)));
            currentFrame->addheader("receipt",to_string(receiptId));
            data.addReceipt(receiptId,currentFrame);
        }
        return currentFrame;
}

vector<string> MessagesFromClientProtocol::splitByWhiteSpace(string line)
{
    vector<string> tokens;
    stringstream check1(line);
    string intermediate;
    while (getline(check1, intermediate, ' ')) //splitting the string to tokens by whitespace
    {
        tokens.push_back(intermediate);
    }
    return tokens;
}

string MessagesFromClientProtocol::process(string msg,ConnectionHandler** connectionHandler)
{

    string stringToSend= "";
    Frame* frame=StringToFrame(msg,connectionHandler);
    if (frame!= nullptr)
    {
        stringToSend = frame->toString();
        if (frame->getValue("receipt") =="") /// if there is no receipt in this frame we can delete it otherwise we might need the frame later
            delete frame;
    }
    return stringToSend;
}

MessagesFromClientProtocol::~MessagesFromClientProtocol()
{
    if (this->serverListenerThread!=nullptr)
    {
        serverListenerThread->join();
        delete (serverListenerThread);
    }
}
