#include "autenticacion.hpp"
#include "logs.hpp"
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
        userId_ = sqlite3_column_int(stmt, 0);
        role_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        username_ = user;
        setUsuarioLogueado(user);
        ok = true;
    }
    sqlite3_finalize(stmt);
    autenticado_ = ok;
    return ok;
}

void Autenticacion::logout() {
    autenticado_ = false;
    userId_ = -1;
    role_.clear();
    username_.clear();
}

bool Autenticacion::estaAutenticado() const {
    return autenticado_;
}

std::string Autenticacion::getUsername() const {
    return username_;
}

void Autenticacion::forzarLogin(const std::string& usuario) {
    this->username_ = usuario;
    this->autenticado_ = true;
    this->userId_ = obtenerIdDesdeUsuario(usuario);
    this->role_ = obtenerRolDesdeUsuario(usuario);
    setUsuarioLogueado(usuario);  
}

int Autenticacion::obtenerIdDesdeUsuario(const std::string& username) {
    sqlite3_stmt* stmt = nullptr;
    int id = -1;
    const char* sql = "SELECT id FROM usuarios WHERE username = ?";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return id;
}
std::string Autenticacion::obtenerRolDesdeUsuario(const std::string& username) {
    sqlite3_stmt* stmt = nullptr;
    std::string rol;
    const char* sql = "SELECT role FROM usuarios WHERE username = ?";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            rol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }

    return rol;
}



} // namespace MedSyc
