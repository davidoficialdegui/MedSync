#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sqlite3.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
#define PORT 8080
#define BUFFER_SIZE 1024

std::string verificar_usuario(const std::string& username, const std::string& password) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string respuesta = "ACCESO_DENEGADO";

    if (sqlite3_open("bd/MedSync.db", &db) != SQLITE_OK) {
        std::cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return "ERROR_BD";
    }

    const char* sql = "SELECT COUNT(*) FROM usuarios WHERE username=? AND password=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count > 0) {
                respuesta = "ACCESO_CONCEDIDO";
            }
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return respuesta;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET server_fd, client_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Servidor escuchando en el puerto " << PORT << "..." << std::endl;

    while (true) {
        client_socket = accept(server_fd, (sockaddr*)&address, &addrlen);
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);

        std::string received(buffer);
        std::istringstream ss(received);
        std::string username, password;
        ss >> username >> password;

        std::string respuesta = verificar_usuario(username, password);
        send(client_socket, respuesta.c_str(), respuesta.size(), 0);
        closesocket(client_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
