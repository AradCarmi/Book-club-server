//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include "MessagesFromServerProtocol.h"
#include <sstream>

MessagesFromServerProtocol::MessagesFromServerProtocol(UsersData& usersData ) : data(usersData),shouldTerminate(false)
{

}

//executes the Frame and returns another Frame to send to server -can be null
Frame* MessagesFromServerProtocol::ExecuteFrame(string msg, ConnectionHandler** handler)
{
    Frame* currentFrame=StringToFrame(msg);
    Frame* Returnframe= nullptr;
    if (currentFrame->getCommandName()=="CONNECTED")
    {
        cout<<"Login successful"<<endl;
    }
    else if (currentFrame->getCommandName()=="ERROR")
    {
        cout<<currentFrame->getBody()<<endl;
        (*handler)->close();
        shouldTerminate=true;
        data.clean();
        delete (*handler);
        (*handler)=nullptr;
    }
    else if (currentFrame->getCommandName()=="MESSAGE")
    {
        cout<<currentFrame->getValue("destination")+":"+currentFrame->getBody()<<endl;
        string body=currentFrame->getBody();
        string destination=currentFrame->getValue("destination");
        vector<string> tokens=splitByWhiteSpace(body);
        if (msg.find("borrow")!= string::npos) ///if the message says some one wish to borrow a book
        {
            string bookName=tokens[4];
            for (unsigned int i=5;i<tokens.size();i++)
                bookName=bookName+" "+tokens[i];
            if (data.hasBook(bookName,destination))
            {
                Returnframe=new SEND();
                Returnframe->addheader("destination",destination);
                string ReturnframeBody=data.getName() + " has " + bookName;
                Returnframe->setBody(ReturnframeBody);
            }
        }
        else if ((msg.find("Taking")!=string::npos) & (msg.find(data.getName())!=string::npos))///if the message says the book is being taken from me
        {
            string bookName=tokens[1];

            for (unsigned int i=2;i<tokens.size();i++)
            {
                if (tokens[i]=="from")
                {
                    break;
                }
                bookName=bookName+" "+tokens[i];
            }

            if (data.hasBook(bookName,destination))
            {
                data.giveBook(bookName,destination);
            }
        }
        else if ((msg.find("Returning")!=string::npos) & (msg.find(data.getName())!=string::npos))/// if the message says the book is being returned to me
        {
            string bookName=tokens[1];
            for (unsigned int i=1;i<tokens.size();i++)
            {
                if (tokens[i]=="to")
                    break;
                bookName=bookName+" "+tokens[i];
            }
            if (!data.hasBook(bookName,destination))
            {
                data.addBookToInventory(destination,bookName);
            }
        }
        else if (msg.find("book status")!=string::npos)
        {
            Returnframe=new SEND();
            string newbody=data.getName()+":";
            Returnframe->addheader("destination",destination);
            newbody=newbody+data.getBooksByGenre(destination);
            Returnframe->setBody(newbody);
        }
        else if (msg.find("has added")!=string::npos)
        {
            //someone has added a book nothing to do
        }
        else if (msg.find("has")!=string::npos) ///if someone published he has a certain book
        {
            string bookName=tokens[2];
            for (unsigned int i=3;i<tokens.size();i++)
            {
                bookName=bookName+" "+tokens[i];
            }
            if (data.isInWishList(bookName))/// if i wanted to borrow this book
            {
                string lender=tokens[0];
                data.borrowFrom(bookName,lender);
                Returnframe=new SEND();
                data.removeFromWishList(bookName);
                data.addBookToInventory(destination,bookName);
                string newbody="Taking "+bookName+" from "+lender;
                Returnframe->addheader("destination",destination);
                Returnframe->setBody(newbody);
            }
        }
    }
    else if(currentFrame->getCommandName()=="RECEIPT")
    {
        int recieptid=stoi(currentFrame->getValue("receipt-id"));
        if (data.getFrameForReceipt(recieptid)->getCommandName() == "DISCONNECT")
        {
            data.clean();
            shouldTerminate=true;
            (*handler)->close();
            delete (*handler);
            *handler= nullptr;
        }
        else if (data.getFrameForReceipt(recieptid)->getCommandName()=="SUBSCRIBE")
        {
            cout<<"Joined club "+data.getFrameForReceipt(recieptid)->getValue("destination")<<endl;
        }
        else if (data.getFrameForReceipt(recieptid)->getCommandName()=="UNSUBSCRIBE")
        {
            int subscriptionId=stoi(data.getFrameForReceipt(recieptid)->getValue("id"));
            cout<<"Exited club "+data.getGenreBySubscriptionId(subscriptionId)<<endl;
        }
        delete data.getFrameForReceipt(recieptid);
    }
    delete currentFrame;
    return Returnframe;
}

string MessagesFromServerProtocol::process(string msg,ConnectionHandler** connectionHandler) {
    Frame *frame = ExecuteFrame(msg, connectionHandler);
    string returnString = "";
    if (frame != nullptr)
    {
        returnString=frame->toString();
        delete frame;
    }
    return returnString;
}

vector<string> MessagesFromServerProtocol::StringToLines(string msg)
{
    istringstream f(msg);
    string line;
    vector<string> msgByLines;
    while (getline(f, line))
    {
        msgByLines.push_back(line);
    }
    return msgByLines;
}

Frame* MessagesFromServerProtocol::StringToFrame(string msg)
{
    Frame* frame=nullptr;
    vector<string> msgByLines=StringToLines(msg);
    string StompCommand=msgByLines[0];
    if (StompCommand=="CONNECTED")
    {
        frame=new CONNECTED();
    }
    else if (StompCommand=="RECEIPT")
    {
        frame=new RECEIPT();
    }
    else if(StompCommand=="ERROR")
    {
        frame=new ERROR();
    }
    else if (StompCommand=="MESSAGE")
    {
        frame=new MESSAGE();
    }
    if (frame!= nullptr) {
        for (unsigned int i = 1; i < msgByLines.size(); i++) {
            if (msgByLines[i] != "") {
                string key = msgByLines[i].substr(0, msgByLines[i].find(':'));
                string value = msgByLines[i].substr(msgByLines[i].find(':') + 1);
                frame->addheader(key, value);
            } else {
                string body = "";
                for (unsigned int j = i + 1; j < msgByLines.size(); j++) {
                    if (body!="") body=body+'\n';
                    body = body + msgByLines[j];
                }
                frame->setBody(body);
                break;
            }
        }
    }
    return frame;

}

vector<string> MessagesFromServerProtocol::splitByWhiteSpace(string line)
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
