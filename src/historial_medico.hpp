#ifndef MEDSYC_HISTORIAL_MEDICO_HPP
#define MEDSYC_HISTORIAL_MEDICO_HPP

#include <string>
#include <ctime>
#include <vector>
#include "sqlite3.h"

namespace MedSyc {

struct Historial {
    int id;
    int paciente_id;
    std::string descripcion;
    std::time_t fecha;
};

class HistorialMedico {
public:
    explicit HistorialMedico(sqlite3* db);
    int agregar(const Historial& h);
    std::vector<Historial> listar(int paciente_id);
    bool eliminar(int id);

private:
    sqlite3* db_;
};

} 

#endif 
