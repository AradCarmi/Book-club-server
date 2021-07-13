//
// Created by amarmic@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_FRAMES_H
#define BOOST_ECHO_CLIENT_FRAMES_H
#include <string>
#include <unordered_map>
using namespace std;

class Frame
{
protected:
    Frame(string name);
    string commandName;
    unordered_map<string,string> headers;
    string body;
public:
    string toString();
    void addheader(string key,string value);
    string getCommandName();
    void setBody(string newBody);
    string getBody();
    string getValue(string key);
};

class CONNECT : public Frame
{
public:
    CONNECT();
};

class SEND : public Frame
{
    public:
        SEND();
};

class SUBSCRIBE: public Frame
{
    public:
        SUBSCRIBE();
};

class UNSUBSCRIBE :public Frame
{
    public:
        UNSUBSCRIBE();
};

class DISCONNECT :public Frame
{
    public:
        DISCONNECT();
};

class CONNECTED: public Frame
{
    public:
        CONNECTED();
};

class RECEIPT: public Frame
{
    public:
       RECEIPT();
};

class ERROR: public Frame
{
public:
    ERROR();
};

class MESSAGE: public Frame
{
public:
    MESSAGE();
};


#endif //BOOST_ECHO_CLIENT_FRAMES_H
