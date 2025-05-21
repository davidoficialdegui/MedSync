#include "autenticacion.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "bd.h"


Role autenticar(sqlite3 *db, const char *username, const char *password) {
    const char *sql = "SELECT role FROM usuarios WHERE username=? AND password=?;";
    sqlite3_stmt *stmt;
    Role role = ROLE_UNKNOWN;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error prepare auth: %s\n", sqlite3_errmsg(db));
        return ROLE_UNKNOWN;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *r = sqlite3_column_text(stmt, 0);
        if (strcmp((const char*)r, "admin")==0) role = ROLE_ADMIN;
        else if (strcmp((const char*)r, "medico")==0) role = ROLE_MEDICO;
        else if (strcmp((const char*)r, "paciente")==0) role = ROLE_PACIENTE;
    }
    sqlite3_finalize(stmt);
    return role;
}
void ensure_default_admin(sqlite3 *db) {
    const char *sql = 
      "INSERT OR IGNORE INTO admin (username, password) "
      "VALUES ('admin', 'admin');";
    int rc = bd_exec(db, sql);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No pude crear el admin por defecto\n");
    }
}
