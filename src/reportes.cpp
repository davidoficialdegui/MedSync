
#include "reportes.hpp"
#include <sqlite3.h>
#include <sstream>
#include <stdexcept>
#include <ctime>

namespace MedSyc {

Reportes::Reportes(sqlite3 *db)
  : db_(db)
{ }

std::string Reportes::reportePaciente(int paciente_id) {
    std::stringstream ss;
    ss << "Reporte Paciente ID: " << paciente_id << "\n";

    sqlite3_stmt *stmt = nullptr;
    const char *sqlPac =
        "SELECT nombre, dni FROM pacientes WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, sqlPac, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, paciente_id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        ss << "Nombre: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) << "\n";
        ss << "DNI:    " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) << "\n";
    } else {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Paciente no encontrado");
    }
    sqlite3_finalize(stmt);

    ss << "\nCitas:\n";
    const char *sqlCitas =
        "SELECT fecha, motivo FROM citas WHERE paciente_id = ? ORDER BY fecha;";
    if (sqlite3_prepare_v2(db_, sqlCitas, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, paciente_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::time_t fecha = static_cast<std::time_t>(sqlite3_column_int64(stmt, 0));
        ss << " - " << std::ctime(&fecha)
           << "   Motivo: " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
           << "\n";
    }
    sqlite3_finalize(stmt);

    ss << "\nHistorial Médico:\n";
    const char *sqlHist =
        "SELECT fecha, descripcion FROM historial_medico WHERE paciente_id = ? ORDER BY fecha;";
    if (sqlite3_prepare_v2(db_, sqlHist, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, paciente_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::time_t fecha = static_cast<std::time_t>(sqlite3_column_int64(stmt, 0));
        ss << " - " << std::ctime(&fecha)
           << "   " << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
           << "\n";
    }
    sqlite3_finalize(stmt);

    return ss.str();
}

std::string Reportes::reporteMedicos() {
    std::stringstream ss;
    ss << "Reporte de Médicos:\n";

    sqlite3_stmt *stmt = nullptr;
    const char *sql =
        "SELECT m.id, m.nombre, m.especialidad, COUNT(c.id) AS total_citas "
        "FROM medicos m "
        "LEFT JOIN citas c ON m.id = c.medico_id "
        "GROUP BY m.id "
        "ORDER BY total_citas DESC;";
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int         id    = sqlite3_column_int(stmt, 0);
        const char *nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char *esp    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int         cnt    = sqlite3_column_int(stmt, 3);
        ss << "ID " << id
           << " - " << nombre
           << " (" << esp << ")"
           << " => " << cnt << " citas\n";
    }
    sqlite3_finalize(stmt);

    return ss.str();
}

}
