#include "historial_medico.h"
#include "bd.h"
#include <stdio.h>

static int hist_cb(void *data, int cols, char **values, char **names) {
    for (int i=0; i<cols; i++) printf("%s = %s\t", names[i], values[i]?values[i]:"NULL");
    printf("\n"); return 0;
}

int historial_create(sqlite3 *db, int paciente_id, const char *fecha, const char *descripcion) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO historial_medico(paciente_id,fecha,descripcion) VALUES(%d,'%s','%s');",
             paciente_id, fecha, descripcion);
    return bd_exec(db, sql);
}

int historial_list(sqlite3 *db, int paciente_id) {
    char sql[256]; snprintf(sql, sizeof(sql),
             "SELECT id,fecha,descripcion FROM historial_medico WHERE paciente_id=%d;", paciente_id);
    return bd_query(db, sql, hist_cb, NULL);
}

int historial_delete(sqlite3 *db, int id) {
    char sql[128]; snprintf(sql, sizeof(sql), "DELETE FROM historial_medico WHERE id=%d;", id);
    return bd_exec(db, sql);
}