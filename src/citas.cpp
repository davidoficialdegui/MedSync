
#include "citas.hpp"
#include <stdexcept>

namespace MedSyc {

Citas::Citas(sqlite3* db)
  : db_(db)
{ }

int Citas::crear(const Cita& c) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "INSERT INTO cita(paciente_id, medico_id, fecha_hora, motivo) VALUES(?,?,?,?);";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        int err = sqlite3_errcode(db_);
        sqlite3_finalize(stmt);
        return err;
    }

    sqlite3_bind_int64(stmt, 1, c.paciente_id);
    sqlite3_bind_int64(stmt, 2, c.medico_id);
    sqlite3_bind_int64(stmt, 3, c.fecha);
    sqlite3_bind_text( stmt, 4, c.motivo.c_str(), -1, nullptr);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : sqlite3_errcode(db_);
}

std::vector<Cita> Citas::listarPorPaciente(int pid) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "SELECT id, medico_id, fecha_hora, motivo "
      "FROM cita WHERE paciente_id = ? ORDER BY fecha_hora;";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, pid);

    std::vector<Cita> out;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cita c;
        c.id          = sqlite3_column_int   (stmt, 0);
        c.paciente_id = pid;
        c.medico_id   = sqlite3_column_int   (stmt, 1);
        c.fecha       = sqlite3_column_int64 (stmt, 2);
        c.motivo      = reinterpret_cast<const char*>(
                           sqlite3_column_text(stmt, 3)
                        );
        out.push_back(std::move(c));
    }
    sqlite3_finalize(stmt);
    return out;
}

std::vector<Cita> Citas::listarPorMedico(int mid) {
    sqlite3_stmt* stmt = nullptr;
    constexpr char SQL[] =
      "SELECT id, paciente_id, fecha_hora, motivo "
      "FROM cita WHERE medico_id = ? ORDER BY fecha_hora;";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_int(stmt, 1, mid);

    std::vector<Cita> out;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cita c;
        c.id          = sqlite3_column_int   (stmt, 0);
        c.paciente_id = sqlite3_column_int   (stmt, 1);
        c.medico_id   = mid;
        c.fecha       = sqlite3_column_int64 (stmt, 2);
        c.motivo      = reinterpret_cast<const char*>(
                           sqlite3_column_text(stmt, 3)
                        );
        out.push_back(std::move(c));
    }
    sqlite3_finalize(stmt);
    return out;
}

} 
