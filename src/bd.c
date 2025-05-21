#include "bd.h"
#include <stdio.h>
#include <sqlite3.h>

int bd_open(const char *filename, sqlite3 **db_out) {
    sqlite3 *db;
    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        fprintf(stderr, "No se pudo abrir BD %s: %s\n", filename, sqlite3_errmsg(db));
        return -1;
    }
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "No se pudo habilitar claves foráneas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    *db_out = db;
    return 0;
}

void bd_close(sqlite3 *db) {
    sqlite3_close(db);
}

int bd_exec(sqlite3 *db, const char *sql) {
    char *err = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL: %s\n", err);
        sqlite3_free(err);
    }
    return rc;
}

int bd_query(sqlite3 *db, const char *sql,
             int (*callback)(void*,int,char**,char**),
             void *param) {
    char *err = NULL;
    int rc = sqlite3_exec(db, sql, callback, param, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en bd_query: %s\nSQL: %s\n", err, sql);
        sqlite3_free(err);
    }
    return rc;
}
