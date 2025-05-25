
#include "historial_medico.hpp"
#include <stdexcept>

namespace MedSyc {

HistorialMedico::HistorialMedico(sqlite3* db)
  : db_(db)
{}

int HistorialMedico::agregar(const Historial& h) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "INSERT INTO historial_medico(paciente_id, fecha, descripcion) VALUES(?,?,?);";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return sqlite3_errcode(db_);
    }
    sqlite3_bind_int64(stmt, 1, h.paciente_id);
    sqlite3_bind_int64(stmt, 2, h.fecha);
    sqlite3_bind_text  (stmt, 3, h.descripcion.c_str(), -1, nullptr);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : sqlite3_errcode(db_);
}

std::vector<Historial> HistorialMedico::listar(int paciente_id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "SELECT id, fecha, descripcion FROM historial_medico WHERE paciente_id = ? ORDER BY fecha;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, paciente_id);
    std::vector<Historial> out;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Historial h;
        h.id           = sqlite3_column_int(stmt, 0);
        h.paciente_id  = paciente_id;
        h.fecha        = sqlite3_column_int64(stmt, 1);
        h.descripcion  = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 2));
        out.push_back(std::move(h));
    }
    sqlite3_finalize(stmt);
    return out;
}

bool HistorialMedico::eliminar(int id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "DELETE FROM historial_medico WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, id);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

} 
