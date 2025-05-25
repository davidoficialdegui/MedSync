
#include "medico.hpp"
#include <stdexcept>

namespace MedSyc {

Medico::Medico(sqlite3* db)
  : db_(db)
{}

int Medico::crear(const std::string& nombre, const std::string& especialidad) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "INSERT INTO medicos(nombre, especialidad) VALUES(?,?);";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return sqlite3_errcode(db_);
    }
    sqlite3_bind_text(stmt, 1, nombre.c_str(),       -1, nullptr);
    sqlite3_bind_text(stmt, 2, especialidad.c_str(), -1, nullptr);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : sqlite3_errcode(db_);
}

bool Medico::cargar(int id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "SELECT nombre, especialidad FROM medicos WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, id);
    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        nombre_       = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        especialidad_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        ok = true;
    }
    sqlite3_finalize(stmt);
    return ok;
}

bool Medico::actualizar(int id, const std::string& nombre, const std::string& especialidad) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "UPDATE medicos SET nombre = ?, especialidad = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_text(stmt, 1, nombre.c_str(),       -1, nullptr);
    sqlite3_bind_text(stmt, 2, especialidad.c_str(), -1, nullptr);
    sqlite3_bind_int (stmt, 3, id);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

bool Medico::eliminar(int id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "DELETE FROM medicos WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, id);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

} 
