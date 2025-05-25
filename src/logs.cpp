
#include "logs.hpp"
#include <fstream>
#include <ctime>
#include <cstring>

static std::string usuario_logueado = "desconocido";

void setUsuarioLogueado(const std::string& usuario) {
    usuario_logueado = usuario;
}

void guardarLog(const std::string& accion) {
    std::ofstream archivo("logs.txt", std::ios::app);

    if (archivo.is_open()) {
        time_t ahora = time(0);
        char* dt = ctime(&ahora);
        dt[strlen(dt)-1] = '\0';

        archivo << "[" << dt << "] Usuario: " << usuario_logueado
                << " - Acción: " << accion << std::endl;
        archivo.close();
    }
}