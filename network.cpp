/* network.cpp */
#include "network.h"
#include "game.h"

/* networking code */
void SetupNetwork()
{
    // Declare variables
    SOCKET RequestSocket;
    struct sockaddr_in saServer;
    hostent* remoteHost;
    char* IP;
    WSADATA wsaData;
    
    // Initialize winsock
	int res = WSAStartup(MAKEWORD(1,1), &wsaData);
	if (res != 0)
	{
		MessageBox(NULL, "Error while initializing winsock.", NULL, 0);
	}

    // Create a socket
    RequestSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Get the host information
    remoteHost = gethostbyname("g.team-duck.com");
    if (remoteHost == NULL)
    {
        MessageBox(NULL, "Error resolving host name.", NULL, 0);
        return;
    }
    IP = inet_ntoa (*(struct in_addr *)*remoteHost->h_addr_list);

    // Set up the sockaddr structure
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr(IP);
    saServer.sin_port = htons(80);

    // Connect to the client list server
    if (connect(RequestSocket,(SOCKADDR*) &saServer, sizeof(saServer)) != 0)
    {
        // bind()
        MessageBox(NULL, "Could not connect.", NULL, 0);
        return;
    }

    // Send the HTTP GET request
    const char* request = "GET /server.php HTTP/1.1\r\nHOST: g.team-duck.com\r\n\r\n";
    if (send(RequestSocket, request, strlen(request), 0) == SOCKET_ERROR)
    {
        MessageBox(NULL, "Error sending.", NULL, 0);
        return;
    }

    // Check for a response
    FD_SET sockets;
    FD_ZERO(&sockets);
    FD_SET(RequestSocket, &sockets);
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 100;
    if (select(0, &sockets, NULL, NULL, &timeout) == 0)
    {
        MessageBox(NULL, "No response.", NULL, 0);
        return;
    }

    // recv() data from the server
    char data[1024]; char alert[1024];
    int size = recv(RequestSocket, data, 1024, 0);
    textbuffer::putline(data);

    // close the socket
	closesocket(RequestSocket);
    WSACleanup();
}
