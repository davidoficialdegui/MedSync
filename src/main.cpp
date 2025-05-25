// --- main.cpp ---
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>

#include "autenticacion.hpp"
#include "admin.hpp"
#include "paciente.hpp"
#include "medico.hpp"
#include "citas.hpp"
#include "historial_medico.hpp"
#include "reportes.hpp"
#include "shell.hpp"

int main() {
    
    sqlite3* db = nullptr;
    if (sqlite3_open("bd/MedSync.db", &db) != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos: "
                  << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    try {
       
        MedSyc::Autenticacion   authSvc(db);
        MedSyc::Admin           adminSvc(db);
        MedSyc::Paciente        pacSvc(db);
        MedSyc::Medico          medSvc(db);
        MedSyc::Citas           citasSvc(db);
        MedSyc::HistorialMedico histSvc(db);
        MedSyc::Reportes        rptSvc(db);

        
        MedSyc::Shell shell(
            db,
            authSvc,
            adminSvc,
            pacSvc,
            medSvc,
            citasSvc,
            histSvc,
            rptSvc
        );
        shell.run();

    } catch (const std::exception& ex) {
        std::cerr << "Excepción: " << ex.what() << std::endl;
        sqlite3_close(db);
        return 1;
    }

    
    sqlite3_close(db);
    return 0;
}
