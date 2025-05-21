#include "paciente.h"
#include "bd.h"
#include <stdio.h>

static int paciente_callback(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) printf("%s = %s\t", names[i], values[i] ? values[i] : "NULL");
    printf("\n");
    return 0;
}

int paciente_create(sqlite3 *db,
                    const char *nombre,
                    const char *apellidos,
                    const char *fecha_nac,
                    const char *genero,
                    const char *telefono,
                    const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO paciente(nombre,apellidos,fecha_nac,genero,telefono,email) VALUES('%s','%s','%s','%s','%s','%s');",
             nombre, apellidos, fecha_nac, genero, telefono, email);
    return bd_exec(db, sql);
}

int paciente_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,nombre,apellidos,fecha_nac,genero,telefono,email FROM paciente;", paciente_callback, NULL);
}

int paciente_update(sqlite3 *db, int id,
                    const char *nombre,
                    const char *apellidos,
                    const char *fecha_nac,
                    const char *genero,
                    const char *telefono,
                    const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "UPDATE paciente SET nombre='%s',apellidos='%s',fecha_nac='%s',genero='%s',telefono='%s',email='%s' WHERE id=%d;",
             nombre, apellidos, fecha_nac, genero, telefono, email, id);
    return bd_exec(db, sql);
}

int paciente_delete(sqlite3 *db, int id) {
    char sql[128]; snprintf(sql, sizeof(sql), "DELETE FROM paciente WHERE id=%d;", id);
    return bd_exec(db, sql);
}