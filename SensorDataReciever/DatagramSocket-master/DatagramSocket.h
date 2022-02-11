#ifndef DATAGRAMSOCKET_H_INCLUDED
#define DATAGRAMSOCKET_H_INCLUDED

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS



#include <winsock2.h>


//Simple socket class for datagrams.  Platform independent between
//unix and Windows.
class DatagramSocket
{
private:
#ifdef WIN32
    WSAData wsaData;
    SOCKET sock;
#else
    int sock;
#endif
    long retval;
    sockaddr_in outaddr;
    char ip[30];
    char received[30];


public:
    DatagramSocket(int port, const char* address, bool Datagram, bool reusesock);
    ~DatagramSocket();

    long receive(char* msg, int msgsize);
    char* received_from();
    long send(const char* msg, int msgsize);
    long sendTo(const char* msg, int msgsize, const char* name);
    int getAddress(const char * name, char * addr);
    const char* getAddress(const char * name);

};



#endif // DATAGRAMSOCKET_H_INCLUDED
