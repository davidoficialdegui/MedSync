#include "autenticacion.hpp"
#include <stdexcept>

namespace MedSyc {

Autenticacion::Autenticacion(sqlite3* db)
  : db_(db)
  , autenticado_(false)
  , userId_(-1)
{}

bool Autenticacion::login(const std::string& user, const std::string& pass) {
    sqlite3_stmt* stmt = nullptr;
    
    constexpr char SQL[] =
      "SELECT id, role FROM usuarios WHERE username = ? AND password = ?;";
    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_text(stmt, 1, user.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, nullptr);

    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId_      = sqlite3_column_int   (stmt, 0);
        role_        = reinterpret_cast<const char*>(
                         sqlite3_column_text(stmt, 1)
                       );
        ok = true;
    }
    sqlite3_finalize(stmt);
    autenticado_ = ok;
    return ok;
}

void Autenticacion::logout() {
    autenticado_ = false;
    userId_      = -1;
    role_.clear();
}

bool Autenticacion::estaAutenticado() const {
    return autenticado_;
}

} 
