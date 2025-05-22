#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string nombrePaciente;

    std::cout << "Introduce el nombre del paciente: ";
    std::getline(std::cin, nombrePaciente);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // Cambia IP si es otra máquina

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, nombrePaciente.c_str(), nombrePaciente.length(), 0);
    std::cout << "Nombre enviado al servidor.\n";

    close(sock);
    return 0;
}
