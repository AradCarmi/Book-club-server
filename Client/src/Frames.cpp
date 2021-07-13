//
// Created by amarmic@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <Frames.h>

Frame::Frame(string name) :commandName(name),headers{},body(){
}

string Frame::getBody()
{
    return body;
}

string Frame::getValue(string key)
{
    return headers[key];
}

void Frame::setBody(string newBody)
{
    this->body=newBody;
}

void Frame::addheader(string key, string value)
{
    headers[key]=value;
}

string Frame::getCommandName() {
    return this->commandName;
}

string Frame::toString()
{
    string returnString="";
    returnString=returnString+this->commandName+"\n";
    for (auto it:headers)
    {
        returnString=returnString+it.first+":"+it.second+"\n";
    }
    returnString=returnString+"\n"+body;
    return returnString;
}

CONNECT::CONNECT() :Frame("CONNECT"){}

SEND::SEND(): Frame("SEND"){}

SUBSCRIBE::SUBSCRIBE() :Frame("SUBSCRIBE"){}

UNSUBSCRIBE::UNSUBSCRIBE() :Frame("UNSUBSCRIBE"){}

DISCONNECT::DISCONNECT() :Frame("DISCONNECT"){}

MESSAGE::MESSAGE() :Frame("MESSAGE") {}

RECEIPT::RECEIPT() :Frame("RECEIPT"){}

ERROR::ERROR() :Frame("ERROR"){}

CONNECTED::CONNECTED() :Frame("CONNECTED"){}