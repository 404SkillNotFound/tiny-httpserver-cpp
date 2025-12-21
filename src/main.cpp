#include <iostream>
#include <string>

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
        std::cerr << "WSAStartup failed: " << wsaResult << '\n';
        return 1;
    }

    std::cout << "Winsock initialized" << std::endl;

    // Step 2: Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: "
                  << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    std::cout << "Socket created" << '\n';

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
    listen(sock, 5);

    // Step 5: Accept client
    sockaddr_in clientAddress;
    int clientAddressLen = sizeof(clientAddress);

    SOCKET clientSocket = accept(
        sock,
        reinterpret_cast<sockaddr*>(&clientAddress),
        &clientAddressLen
    );

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << '\n';
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Accepted connection from "
              << inet_ntoa(clientAddress.sin_addr)
              << '\n';

    // Step 6: Read request
    char buffer[4096];
    std::string request;

    while (true)
    {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0)
        {
            request.append(buffer, bytesReceived);

            if (request.find("\r\n\r\n") != std::string::npos)
                break;
        }
        else if (bytesReceived == 0)
        {
            break;
        }
        else
        {
            std::cerr << "recv failed\n";
            closesocket(clientSocket);
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    // Step 7: Send response
    std::string response;

    if (request.rfind("GET / ", 0) == 0)
    {
        std::string body =
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>Index</title>"
            "<style>"
            "html, body { height: 100%; margin: 0; }"
            "body {"
            "  font-family: system-ui, -apple-system, BlinkMacSystemFont, sans-serif;"
            "  background: #f6f7f9;"
            "  display: flex;"
            "  align-items: center;"
            "  justify-content: center;"
            "}"
            ".card {"
            "  background: #ffffff;"
            "  padding: 32px 40px;"
            "  border-radius: 8px;"
            "  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.08);"
            "  max-width: 420px;"
            "}"
            "h1 {"
            "  margin-top: 0;"
            "  font-weight: 600;"
            "}"
            "p {"
            "  color: #444;"
            "  line-height: 1.6;"
            "}"
            "</style>"
            "</head>"
            "<body>"
            "<div class='card'>"
            "<h1>Hello</h1>"
            "<p>If this loaded, something went right.</p>"
            "<p>Let us not investigate further.</p>"
            "</div>"
            "</body>"
            "</html>";

        response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;
    }
    else
    {
        response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 9\r\n"
            "\r\n"
            "Not Found";
    }

    send(clientSocket,
         response.c_str(),
         static_cast<int>(response.size()),
         0);

    shutdown(clientSocket, SD_SEND);
    closesocket(clientSocket);


    // Step 8: Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}
