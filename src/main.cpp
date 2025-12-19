#include <iostream>

// Windows socket API
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // Step 1: Initialize Winsock
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (wsaResult != 0)
    {
        std::cerr << "WSAStartup failed: " << wsaResult << std::endl;
        return 1;
    }

    std::cout << "Winsock initialized" << std::endl;

    // Step 2: Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: "
                  << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    std::cout << "Socket created" << std::endl;



    // Step 3: Bind to port
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    int bindResult = bind(
        sock,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );

    if (bindResult != 0) {
        std::cerr << "Bind failed: " << WSAGetLastError() << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;

    }
    std::cout << "Socket Bound to port 8080" << '\n';




    // Step 4: Listen for connections

    // Step 5: Accept client

    // Step 6: Read request

    // Step 7: Send response



    // Step 8: Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}
