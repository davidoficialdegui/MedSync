#include "Socket/Socket.hpp"
#include <iostream>

int main() {
    try {
        SocketCliente cliente("127.0.0.1", 9090);
        std::string datos = "NUEVO_PACIENTE|Ana|López|1995-01-01|F|123456789|ana@example.com";
        cliente.enviar(datos);
        std::cout << "Respuesta del servidor: " << cliente.recibir() << std::endl;
    } catch (const SocketException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
