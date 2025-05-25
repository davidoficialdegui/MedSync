#ifndef MEDSYC_REPORTES_HPP
#define MEDSYC_REPORTES_HPP

#include <string>
#include "sqlite3.h"

namespace MedSyc {

class Reportes {
public:
    explicit Reportes(sqlite3 *db);

    std::string reportePaciente(int paciente_id);
    std::string reporteMedicos();

private:
    sqlite3 *db_;   
};

} 

#endif 
