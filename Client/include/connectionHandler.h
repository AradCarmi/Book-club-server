#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>

using namespace std;


using boost::asio::ip::tcp;

class ConnectionHandler {
private:
    std::string host_;
    short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
    mutex* socket_mutex;
public:
    ConnectionHandler(const ConnectionHandler& con);

    ConnectionHandler& operator=(const ConnectionHandler& other);

    ConnectionHandler(std::string host, short port);
    virtual ~ConnectionHandler();
 
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);
 
	// Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);

 
    // Get Ascii data from the server until the delimiter character
    // Returns false in case connection closed before null can be read.
    bool getFrameAscii(std::string& frame, char delimiter);
 
    // Send a message to the remote host.
    // Returns false in case connection is closed before all the data is sent.
    bool sendFrameAscii(const std::string& frame, char delimiter);
	
    // Close down the connection properly.
    void close();



}; //class ConnectionHandler
 
#endif