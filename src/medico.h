/*include "medico.h"
#include <stdio.h>

static int medico_callback(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) printf("%s = %s\t", names[i], values[i] ? values[i] : "NULL");
    printf("\n");
    return 0;
}

int medico_create(sqlite3 *db,
                  const char *nombre,
                  const char *especialidad,
                  const char *telefono,
                  const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO medico(nombre,especialidad,telefono,email) VALUES('%s','%s','%s','%s');",
             nombre, especialidad, telefono, email);
    return bd_exec(db, sql);
}

int medico_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,nombre,especialidad,telefono,email FROM medico;", medico_callback, NULL);
}

int medico_update(sqlite3 *db, int id,
                  const char *nombre,
                  const char *especialidad,
                  const char *telefono,
                  const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "UPDATE medico SET nombre='%s',especialidad='%s',telefono='%s',email='%s' WHERE id=%d;",
             nombre, especialidad, telefono, email, id);
    return bd_exec(db, sql);
}

int medico_delete(sqlite3 *db, int id) {
    char sql[128]; snprintf(sql, sizeof(sql), "DELETE FROM medico WHERE id=%d;", id);
    return bd_exec(db, sql);
}*/



// ------------------------FIN DE CODIGO EN C----------------------------





#ifndef MEDICO_H
#define MEDICO_H

#include <sqlite3.h>
#include <string>

int medico_create(sqlite3 *db,
                 const std::string &nombre,
                 const std::string &especialidad,
                 const std::string &telefono = "",
                 const std::string &email = "");

int medico_list(sqlite3 *db);
int medico_update(sqlite3 *db,
                 int id,
                 const std::string &nombre,
                 const std::string &especialidad,
                 const std::string &telefono = "",
                 const std::string &email = "");
int medico_delete(sqlite3 *db, int id);

#endif