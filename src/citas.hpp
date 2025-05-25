
#ifndef MEDSYC_CITAS_HPP
#define MEDSYC_CITAS_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <sqlite3.h>

namespace MedSyc {

struct Cita {
    int          id;
    int          paciente_id;
    int          medico_id;
    std::int64_t fecha;   
    std::string  motivo;
};

class Citas {
public:
    explicit Citas(sqlite3* db);
    int crear(const Cita& c);
    std::vector<Cita> listarPorPaciente(int paciente_id);
    std::vector<Cita> listarPorMedico(int medico_id);

private:
    sqlite3* db_;  
};

}

#endif 
