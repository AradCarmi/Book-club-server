//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USERSDATA_H
#define BOOST_ECHO_CLIENT_USERSDATA_H
#include <string>
#include <list>
#include <map>
#include <Frames.h>
#include <vector>
#include <mutex>

using namespace std;

class UsersData
{
public:
    UsersData();
    ~UsersData();
    int getLastReceiptId() const;
    int getLastSubscriptionId() const;
    void incLastReceiptId();
    void incLastSubscriptionId();
    void setName(const string &name);
    const string &getName() const;
    void addBookToInventory(string genre,string bookname);
    void AddToWishList(string bookname);
    bool isInWishList(string bookname);
    string WhoIBorrowedTheBookFrom(string bookname);
    void returnBook(string genre,string bookname);
    void addReceipt(int receiptid,Frame* frame);
    void Subscribe(int id,string genre);
    int UnSubscribe(string genre);
    string getGenreBySubscriptionId(int id);
    bool hasBook(string bookName,string genre);
    void giveBook(string bookname,string genre);
    void borrowFrom(string bookname,string lender);
    string getBooksByGenre(string genre);
    Frame* getFrameForReceipt(int id);
    void removeFromWishList(string bookname);
    void clean();
private:
    int lastReceiptId;
    int lastSubscriptionId;
    string name;
    map<string,vector<string>> bookInventory;//key=genre,value=books of this type in the clients inventory
    list<string> WishList;
    map<string,string> whoIBorrowedTheBookFrom;//key=bookname, value= the user i borrowed the client from
    map<string,int> GenereToSubscriptionID;//key=genre,value=the id i used to register this genre,
    map<int,string> SubscriptionIdToGenre;//key=subscriptionId , value=genre
    map<int,Frame*> RecieptToFrame;//key=receipt_id, value=frame.StompName
    mutex bookInventoryMutex;
    mutex wishListMutex;
    mutex whoIBorrowedTheBookFromMutex;
    mutex subscriptionIdToGenreMutex;
    mutex receiptToFrameMutex;
};


#endif //BOOST_ECHO_CLIENT_USERSDATA_H
