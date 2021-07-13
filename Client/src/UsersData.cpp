//
// Created by amarmic@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <algorithm>
#include "UsersData.h"
 UsersData::UsersData() : lastReceiptId(0), lastSubscriptionId(0), name(""), bookInventory{},
                            WishList{}, whoIBorrowedTheBookFrom{}, GenereToSubscriptionID{},SubscriptionIdToGenre{},RecieptToFrame{},
                            bookInventoryMutex(),wishListMutex(),whoIBorrowedTheBookFromMutex(),subscriptionIdToGenreMutex(),receiptToFrameMutex()
 {

 }

int UsersData::getLastReceiptId() const {
    return lastReceiptId;
}

int UsersData::getLastSubscriptionId() const {
    return lastSubscriptionId;
}

void UsersData::incLastReceiptId() {
    this->lastReceiptId++;
}

void UsersData::incLastSubscriptionId() {
    this->lastSubscriptionId++;
}

void UsersData::setName(const string &name) {
    UsersData::name = name;
}

const string &UsersData::getName() const {
    return name;
}

void UsersData::addBookToInventory(string genre,string bookname)
{
    lock_guard<mutex> lock(bookInventoryMutex);
    if (bookInventory.find(genre)==bookInventory.end())
        bookInventory[genre]=vector<string>();
    bookInventory[genre].push_back(bookname);
}

void UsersData::AddToWishList(string bookname)
{
    lock_guard<mutex> lock(wishListMutex);
    WishList.push_back(bookname);
}

void UsersData::removeFromWishList(string bookname)
{
    lock_guard<mutex> lock(wishListMutex);
    WishList.remove(bookname);
}

bool UsersData::isInWishList(string bookname)
{
    lock_guard<mutex> lock(wishListMutex);
    list<string>::iterator findIter = std::find(WishList.begin(), WishList.end(), bookname);
    if (findIter==WishList.end())
        return false;
    return true;
}

string UsersData::WhoIBorrowedTheBookFrom(string bookname)
{
    lock_guard<mutex> lock(whoIBorrowedTheBookFromMutex);
    return whoIBorrowedTheBookFrom[bookname];
}

void UsersData::borrowFrom(string bookname, string lender)
{
    lock_guard<mutex> lock(whoIBorrowedTheBookFromMutex);
    whoIBorrowedTheBookFrom[bookname]=lender;
}

void UsersData::returnBook(string genre,string bookname)
{
    lock_guard<mutex> lock(bookInventoryMutex);
    lock_guard<mutex> lock2(whoIBorrowedTheBookFromMutex);
    //delete the book from the inventory
    bookInventory[genre].erase(std::find(bookInventory[genre].begin(), bookInventory[genre].end(),bookname));
    whoIBorrowedTheBookFrom.erase(bookname);
}

void UsersData::giveBook(string bookname, string genre)
{
    lock_guard<mutex> lock(bookInventoryMutex);
    vector<string>::iterator it = std::find(bookInventory[genre].begin(), bookInventory[genre].end(), bookname);
    bookInventory[genre].erase(it);
}

bool UsersData::hasBook(string bookName,string genre)
{
    lock_guard<mutex> lock(bookInventoryMutex);
    vector<string>::iterator it = std::find(bookInventory[genre].begin(), bookInventory[genre].end(), bookName);
    if (it==bookInventory[genre].end())
        return false;
    return true;
}

void UsersData::Subscribe(int id, string genre)
{
    ///GenereToSubscriptionID is a map that only the thread listening to the key board is using so we dont block it
    lock_guard<mutex> lock(subscriptionIdToGenreMutex);
    GenereToSubscriptionID[genre]=id;
    SubscriptionIdToGenre[id]=genre;
}

//returns the id used to register to this genre
//also removes the the pair from the map
int UsersData::UnSubscribe(string genre)
{

    int id=GenereToSubscriptionID[genre];
    //GenereToSubscriptionID.erase(genre);
    return id;
}

string UsersData::getGenreBySubscriptionId(int id)
{
    lock_guard<mutex> lock(subscriptionIdToGenreMutex);
    return SubscriptionIdToGenre[id];
}

string UsersData::getBooksByGenre(string genre)
{
    lock_guard<mutex> lock(bookInventoryMutex);
    string returnString="";
    for (string book : this->bookInventory[genre])
    {
        returnString=returnString+book+',';
    }
    if (returnString[returnString.length()-1]==',')//if the last char is comma remove it
    {
        string temp=returnString.substr(0,returnString.length()-1);
        returnString=temp;
    }
    return returnString;
}

//gets receipt id and returns the stomp command name that the receipt is for-(DISCONNECT,etc..)
Frame* UsersData::getFrameForReceipt(int recieptId)
{
    lock_guard<mutex> lock(receiptToFrameMutex);
    return this->RecieptToFrame[recieptId];
}

void UsersData::addReceipt(int receiptid, Frame* frame)
{
    lock_guard<mutex> lock(receiptToFrameMutex);
    this->RecieptToFrame[receiptid]=frame;
}

void UsersData::clean()
{
    lastSubscriptionId=0;
    lastReceiptId=0;
    name="";
    bookInventory.clear();
    WishList.clear();
    whoIBorrowedTheBookFrom.clear();
    GenereToSubscriptionID.clear();
    SubscriptionIdToGenre.clear();
}

UsersData::~UsersData()
{

}