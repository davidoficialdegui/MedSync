#ifndef MEDSYC_MEDICO_HPP
#define MEDSYC_MEDICO_HPP

#include <string>
#include <sqlite3.h>

namespace MedSyc {

class Medico {
public:
    explicit Medico(sqlite3* db);
    int crear(const std::string& nombre, const std::string& especialidad);
    bool cargar(int id);
    bool actualizar(int id, const std::string& nombre, const std::string& especialidad);
    bool eliminar(int id);

private:
    sqlite3* db_;
    std::string nombre_;
    std::string especialidad_;
};

}

#endif 
