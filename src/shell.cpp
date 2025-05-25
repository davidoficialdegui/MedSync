#include "shell.hpp"
#include <iostream>
#include <limits>
#include <ctime>
#include "logs.hpp"

namespace MedSyc {

Shell::Shell(sqlite3* db,
             Autenticacion& auth,
             Admin& adminSvc,
             Paciente& pacSvc,
             Medico& medSvc,
             Citas& citasSvc,
             HistorialMedico& histSvc,
             Reportes& rptSvc)
  : db_(db)
  , auth_(auth)
  , admin_(adminSvc)
  , pac_(pacSvc)
  , med_(medSvc)
  , citas_(citasSvc)
  , hist_(histSvc)
  , rpt_(rptSvc)
{}

void Shell::setLoginAutomatizado(const std::string& usuario) {
  // Fuerza la autenticación si viene desde fuera (cliente)
  auth_.forzarLogin(usuario);
}

void Shell::run() {
  // Solo pedimos login si no viene autenticado (por ejemplo, desde el cliente)
  if (!auth_.estaAutenticado()) {
    std::cout << "INICIO DE SESION:\n";
    while (!auth_.estaAutenticado()) {
      std::cout << "Usuario: ";
      std::string u, p;
      std::cin >> u;
      if (std::cin.fail()) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << " Entrada inválida. Intenta de nuevo.\n";
          break;
      }

      std::cout << "Clave:   ";
      std::cin >> p;
      if (std::cin.fail()) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cout << " Entrada inválida. Intenta de nuevo.\n";
          break;
      }

      if (!auth_.login(u, p)) {
        std::cout << "Credenciales inválidas.\n";
        setUsuarioLogueado(u);
        guardarLog("Intento de inicio de sesión");
      } else {
        setUsuarioLogueado(u);
        guardarLog("Inicio de sesión");
      }
    }
  }

  const auto role = auth_.role();

  int opc=0;

  // --- shell.cpp (solo el bloque admin dentro de Shell::run()) ---
#include "shell.hpp"
#include <iostream>
#include <limits>

// … dentro de Shell::run(), después de login …

if (role == "admin") {
  do {
    std::cout << "\n[ADMIN]\n"
                 "1) Ver historiales médicos\n"
                 "2) Ver todas las citas\n"
                 "3) Ver todos los médicos\n"
                 "4) Ver todos los reportes\n"
                 "5) Eliminar registro\n"
                 "6) Salir\n> ";
    std::cin >> opc;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
}
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (opc) {
      case 1: {
        // Listar historiales
        const char* sql = "SELECT id, paciente_id, fecha, descripcion FROM historial_medico;";
        sqlite3_stmt* stmt = nullptr;
        guardarLog("Admin - Ver historiales médicos");
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
}
        std::cout << "\nHistoriales Médicos:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
          std::cout
            << "  [" << sqlite3_column_int(stmt,0) << "] "
            << "Pac: " << sqlite3_column_int(stmt,1)
            << " Fecha: " << sqlite3_column_int64(stmt,2)
            << " Desc: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt,3))
            << "\n";
        }
        sqlite3_finalize(stmt);
      } break;

      case 2: {
        // Listar citas
        const char* sql = 
          "SELECT id, paciente_id, medico_id, fecha_hora, motivo FROM cita;";
        sqlite3_stmt* stmt = nullptr;
        guardarLog("Admin - Ver todas las citas");
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
}
        std::cout << "\nCitas:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
          std::cout
            << "  [" << sqlite3_column_int(stmt,0) << "] "
            << "Pac:"   << sqlite3_column_int(stmt,1)
            << " Med:"   << sqlite3_column_int(stmt,2)
            << " Fecha:" << sqlite3_column_int64(stmt,3)
            << " Motivo:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt,4))
            << "\n";
        }
        sqlite3_finalize(stmt);
      } break;

      case 3: {
        // Listar médicos
        const char* sql = 
          "SELECT id, nombre, especialidad, telefono, email FROM medico;";
        sqlite3_stmt* stmt = nullptr;
         guardarLog("Admin - Ver todos los médicos");
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
}
        std::cout << "\nMédicos:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
          std::cout
            << "  [" << sqlite3_column_int(stmt,0) << "] "
            << reinterpret_cast<const char*>(sqlite3_column_text(stmt,1))
            << " (" << reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)) << ") "
            << "Tel: "  << reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)) << " "
            << "Email: "<< reinterpret_cast<const char*>(sqlite3_column_text(stmt,4))
            << "\n";
        }
        sqlite3_finalize(stmt);
      } break;

      case 4: {
        // Listar reportes
        const char* sql = 
          "SELECT id, paciente_id, fecha, problema, estado, respuesta FROM reportes;";
        sqlite3_stmt* stmt = nullptr;
         guardarLog("Admin - Ver todos los reportes");
        if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
}
        std::cout << "\nReportes:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
          std::cout
            << "  [" << sqlite3_column_int(stmt,0) << "] "
            << "Pac:" << sqlite3_column_int(stmt,1)
            << " Fecha:" << sqlite3_column_int64(stmt,2)
            << " Problema:" << reinterpret_cast<const char*>(sqlite3_column_text(stmt,3))
            << " Estado:"   << reinterpret_cast<const char*>(sqlite3_column_text(stmt,4))
            << " Respuesta:"<< reinterpret_cast<const char*>(sqlite3_column_text(stmt,5))
            << "\n";
        }
        sqlite3_finalize(stmt);
      } break;

      case 5: {
        guardarLog("Admin - Eliminar registro");
        // Submenú de eliminación
        std::cout << "\n¿Qué tabla quieres limpiar?\n"
                     "  1) historial_medico\n"
                     "  2) cita\n"
                     "  3) medico\n"
                     "  4) reportes\n"
                     "Opción: ";
        int tbl; std::cin >> tbl;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
}
        std::cout<<"ID a eliminar: ";
        int id; std::cin >> id;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
}
        const char* tables[] = {
          nullptr,
          "historial_medico",
          "cita",
          "medico",
          "reportes"
        };
        if (tbl>=1 && tbl<=4) {
          
          std::string del = 
            "DELETE FROM " + std::string(tables[tbl]) + " WHERE id = ?;";
          sqlite3_stmt* stmt = nullptr;
          if (sqlite3_prepare_v2(db_, del.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
}
          sqlite3_bind_int(stmt, 1, id);
          if (sqlite3_step(stmt)==SQLITE_DONE){
            std::cout<<"Registro eliminado.\n";
             guardarLog("Admin - Eliminó de la tabla " + std::string(tables[tbl]) + " el ID " + std::to_string(id));
          }
          else
            std::cout<<"Error al eliminar.\n";
          sqlite3_finalize(stmt);
        } else {
          std::cout<<"Opción inválida.\n";
        }
      } break;

      case 6:
      guardarLog("Admin - Salió del sistema");
        std::cout<<"Hasta luego, Admin.\n";
      break;

      default:
        std::cout<<"Opción inválida.\n";

    } 
}while(opc!=6);

  } else if (role=="medico") {
    do {
      std::cout << "\n[MEDICO]\n 1) Crear cita\n 2) Ver mis citas\n 3) Ver historial\n 4) Ver reportes\n 5) Salir\n> ";
      std::cin >> opc;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
} std::cin.ignore();
      switch(opc) {
        case 1: {
          Cita c;
          std::cout << "ID Paciente: ";
          std::cin >> c.paciente_id;
          std::cin.ignore();
          sqlite3_stmt* stmt_check = nullptr;
          const char* sql_check = "SELECT 1 FROM paciente WHERE id = ?;";
          bool pacienteValido = false;
           guardarLog("Medico - Crear cita");
if (sqlite3_prepare_v2(db_, sql_check, -1, &stmt_check, nullptr) == SQLITE_OK) {
  if (sqlite3_bind_int(stmt_check, 1, c.paciente_id) != SQLITE_OK) {
    std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db_) << std::endl;
    break;
  }
  if (sqlite3_step(stmt_check) == SQLITE_ROW) {
    pacienteValido = true;
  }
}
          sqlite3_finalize(stmt_check);
          if (!pacienteValido) {
            std::cout << "El paciente no existe.";
            break;
          }
          std::cout<<"Motivo: "; std::getline(std::cin,c.motivo);
          c.medico_id = auth_.userId();
          c.fecha     = std::time(nullptr);
          citas_.crear(c)==SQLITE_OK
            ? std::cout<<"Cita creada.\n"
            : std::cout<<"Error al crear cita.\n";
        } break;
        case 2: {
          auto v=citas_.listarPorMedico(auth_.userId());
          guardarLog("Medico - Ver mis citas");
          for(auto&e:v)
            std::cout<<e.id<<": Pac:"<<e.paciente_id<<" Fecha:"<<e.fecha<<" Motivo:"<<e.motivo<<"\n";
        } break;
        case 3: {
          int pid;
          std::cout<<"ID Paciente: "; std::cin >> pid;
          
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
} std::cin.ignore();
    guardarLog("Medico - Consultó historial del paciente ID " + std::to_string(pid));

          auto h=hist_.listar(pid);
          for(auto&x:h)
            std::cout<<x.id<<": "<<x.descripcion<<" ("<<x.fecha<<")\n";
        } break;
        case 4:
          std::cout<<rpt_.reporteMedicos();
          guardarLog("Medico - Ver reportes");
        break;
        case 5:
          std::cout<<"Adiós.\n";
           guardarLog("Medico - Salió del sistema");
        break;
      }
    } while(opc!=5);

  } else { // paciente
    do {
      std::cout << "\n[PACIENTE]\n 1) Crear cita\n 2) Ver mis citas\n 3) Mi reporte\n 4) Salir\n> ";
      std::cin >> opc;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " Entrada inválida. Intenta de nuevo.\n";
    break;
} std::cin.ignore();
      switch(opc) {
        case 1: {
          Cita c;
          c.paciente_id = auth_.userId();
          guardarLog("Paciente - Crear cita");
          std::cout<<"Motivo: "; std::getline(std::cin,c.motivo);
          c.medico_id = 0;            // o pedirlo
          c.fecha     = std::time(nullptr);
          citas_.crear(c)==SQLITE_OK
            ? std::cout<<"Cita creada.\n"
            : std::cout<<"Error creando cita.\n";
        } break;
        case 2: {
          auto v=citas_.listarPorPaciente(auth_.userId());
          guardarLog("Paciente - Ver mis citas");
          for(auto&e:v)
            std::cout<<e.id<<": Med:"<<e.medico_id<<" Fecha:"<<e.fecha<<" Motivo:"<<e.motivo<<"\n";
        } break;
        case 3:
         guardarLog("Paciente - Ver mi reporte");
          std::cout<<rpt_.reportePaciente(auth_.userId());
        break;
        case 4:
        guardarLog("Paciente - Salió del sistema");
          std::cout<<"Adiós.\n";
        break;
      }
    } while(opc!=4);
  }

  sqlite3_close(db_);
}



}// namespace MedSyc
