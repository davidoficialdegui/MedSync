#ifndef MEDSYC_PACIENTE_HPP
#define MEDSYC_PACIENTE_HPP

#include <string>
#include "sqlite3.h"

namespace MedSyc {

class Paciente {
public:
    explicit Paciente(sqlite3* db);
    int crear(const std::string& nombre, const std::string& dni);
    bool cargar(int id);
    bool actualizar(int id, const std::string& nombre, const std::string& dni);
    bool eliminar(int id);

private:
    sqlite3* db_;
    std::string nombre_;
    std::string dni_;
};

}

#endif 
