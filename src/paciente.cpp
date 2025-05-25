#include "paciente.hpp"
#include "logs.hpp"  // ← necesario para guardarLog
#include <stdexcept>

namespace MedSyc {

Paciente::Paciente(sqlite3* db)
  : db_(db)
{}

int Paciente::crear(const std::string& nombre, const std::string& dni) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "INSERT INTO pacientes(nombre, dni) VALUES(?,?);";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return sqlite3_errcode(db_);
    }
    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, dni.c_str(), -1, nullptr);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        guardarLog("Creó paciente - Nombre: " + nombre + ", DNI: " + dni);
    }

    return (rc == SQLITE_DONE) ? SQLITE_OK : sqlite3_errcode(db_);
}

bool Paciente::cargar(int id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "SELECT nombre, dni FROM pacientes WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, id);
    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        nombre_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        dni_    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        ok = true;
    }
    sqlite3_finalize(stmt);

    if (ok) {
        guardarLog("Cargó paciente ID " + std::to_string(id));
    }

    return ok;
}

bool Paciente::actualizar(int id, const std::string& nombre, const std::string& dni) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "UPDATE pacientes SET nombre = ?, dni = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, dni.c_str(),   -1, nullptr);
    sqlite3_bind_int (stmt, 3, id);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (ok) {
        guardarLog("Actualizó paciente ID " + std::to_string(id) +
                   " - Nuevo nombre: " + nombre + ", Nuevo DNI: " + dni);
    }

    return ok;
}

bool Paciente::eliminar(int id) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "DELETE FROM pacientes WHERE id = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, id);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (ok) {
        guardarLog("Eliminó paciente ID " + std::to_string(id));
    }

    return ok;
}

} // namespace MedSyc
