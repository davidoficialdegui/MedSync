#define _WIN32_WINNT 0x0600
#include "Socket/Socket.hpp"
#include "paciente.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <sstream>
#include <iostream>
#include <sqlite3.h>

void atender_cliente(SOCKET cfd, sqlite3* db) {
    char buf[1024];
    int n = recv(cfd, buf, sizeof(buf) - 1, 0);
    if (n <= 0) {
        closesocket(cfd);
        return;
    }
    buf[n] = 0;

    std::istringstream ss(buf);
    std::string cmd;
    std::getline(ss, cmd, '|');

    if (cmd == "NUEVO_PACIENTE") {
        std::string nombre, apellidos, fecha_nac, genero, telefono, email;

        std::getline(ss, nombre, '|');
        std::getline(ss, apellidos, '|');
        std::getline(ss, fecha_nac, '|');
        std::getline(ss, genero, '|');
        std::getline(ss, telefono, '|');
        std::getline(ss, email, '|');

        int res = paciente_create(db, nombre, apellidos, fecha_nac, genero, telefono, email);
        if (res == 0)
            send(cfd, "OK", 2, 0);
        else
            send(cfd, "ERROR", 5, 0);
    } else {
        send(cfd, "COMANDO_INVALIDO", 16, 0);
    }

    closesocket(cfd);
}

int main() {
    sqlite3* db;
    if (sqlite3_open("MedSyncc.db", &db) != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos\n";
        return 1;
    }

    try {
        SocketServidor servidor(9090);
        std::cout << "Servidor de MedSync activo en puerto 9090\n";

        while (true) {
            SOCKET cfd = servidor.aceptar();
            //std::thread([cfd, db]() { atender_cliente(cfd, db); }).detach();
            // lo de arriba es lo que esta bien, pero no va con la verion de MinGW que tenemos.
            atender_cliente(cfd, db);  
        }
    } catch (const SocketException& e) {
        std::cerr << "Error de socket: " << e.what() << std::endl;
    }

    sqlite3_close(db);
    return 0;
}
