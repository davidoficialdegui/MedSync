#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock;
    sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error al conectar" << std::endl;
        return -1;
    }

    std::string username, password;
    std::cout << "Usuario: ";
    std::cin >> username;
    std::cout << "Contraseña: ";
    std::cin >> password;

    std::string mensaje = username + " " + password;
    send(sock, mensaje.c_str(), mensaje.length(), 0);

    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    buffer[valread] = '\0';

    std::string respuesta(buffer);
    std::cout << "Respuesta del servidor: " << respuesta << std::endl;

    if (respuesta.rfind("ACCESO_CONCEDIDO", 0) == 0) {
        std::cout << "Iniciando MedSync.exe..." << std::endl;
        std::string comando = "MedSync.exe " + username;  
        system(comando.c_str());
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
