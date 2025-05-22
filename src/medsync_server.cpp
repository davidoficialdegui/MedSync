#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    char buffer[1024] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Escucha en todas las interfaces
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Esperando conexión de cliente en el puerto " << PORT << "...\n";

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(new_socket, buffer, 1024);
    std::cout << "Nuevo paciente recibido: " << buffer << std::endl;

    close(new_socket);
    close(server_fd);
    return 0;
}
