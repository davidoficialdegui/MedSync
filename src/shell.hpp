
#ifndef MEDSYC_SHELL_HPP
#define MEDSYC_SHELL_HPP

#include <sqlite3.h>
#include "autenticacion.hpp"
#include "admin.hpp"
#include "paciente.hpp"
#include "medico.hpp"
#include "citas.hpp"
#include "historial_medico.hpp"
#include "reportes.hpp"

namespace MedSyc {

class Shell {
public:
    Shell(sqlite3* db,
          Autenticacion& auth,
          Admin& adminSvc,
          Paciente& pacSvc,
          Medico& medSvc,
          Citas& citasSvc,
          HistorialMedico& histSvc,
          Reportes& rptSvc);

    
    void run();

private:
    sqlite3* db_;
    Autenticacion& auth_;
    Admin& admin_;
    Paciente& pac_;
    Medico& med_;
    Citas& citas_;
    HistorialMedico& hist_;
    Reportes& rpt_;

    
    void mostrarMenu();
    void manejarOpcion(int opc);
};

} 

#endif 
