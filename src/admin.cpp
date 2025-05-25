#include "admin.hpp"
#include <stdexcept>

namespace MedSyc {

Admin::Admin(sqlite3* db)
  : db_(db)
{}

int Admin::crear(const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    constexpr auto SQL =
      "INSERT INTO admin(username,password) VALUES(?,?);";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        int err = sqlite3_errcode(db_);
        sqlite3_finalize(stmt);
        return err;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, nullptr);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : sqlite3_errcode(db_);
}

std::vector<AdminInfo> Admin::listar() {
    sqlite3_stmt* stmt = nullptr;
    constexpr auto SQL = "SELECT id, username FROM admin;";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }

    std::vector<AdminInfo> out;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        AdminInfo ai;
        ai.id       = sqlite3_column_int(stmt, 0);
        ai.username = reinterpret_cast<const char*>(
                        sqlite3_column_text(stmt, 1)
                      );
        out.push_back(std::move(ai));
    }
    sqlite3_finalize(stmt);
    return out;
}

bool Admin::autenticar(const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    constexpr auto SQL =
      "SELECT COUNT(*) FROM admin WHERE username=? AND password=?;";

    if (sqlite3_prepare_v2(db_, SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, nullptr);

    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        ok = (sqlite3_column_int(stmt, 0) == 1);
    }
    sqlite3_finalize(stmt);
    return ok;
}

} 
