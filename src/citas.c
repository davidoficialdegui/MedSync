#include "citas.h"
#include "bd.h"
#include <stdio.h>

static int cita_cb(void *data, int cols, char **values, char **names) {
    for (int i=0; i<cols; i++) printf("%s = %s\t", names[i], values[i]?values[i]:"NULL");
    printf("\n"); return 0;
}

int cita_create(sqlite3 *db, int paciente_id, int medico_id, const char *fecha_hora, const char *motivo) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO cita(paciente_id,medico_id,fecha_hora,motivo) VALUES(%d,%d,'%s','%s');",
             paciente_id, medico_id, fecha_hora, motivo);
    return bd_exec(db, sql);
}

int cita_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,paciente_id,medico_id,fecha_hora,motivo FROM cita;", cita_cb, NULL);
}

int cita_update(sqlite3 *db, int id, int paciente_id, int medico_id, const char *fecha_hora, const char *motivo) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "UPDATE cita SET paciente_id=%d,medico_id=%d,fecha_hora='%s',motivo='%s' WHERE id=%d;",
             paciente_id, medico_id, fecha_hora, motivo, id);
    return bd_exec(db, sql);
}

int cita_delete(sqlite3 *db, int id) {
    char sql[128]; snprintf(sql, sizeof(sql), "DELETE FROM cita WHERE id=%d;", id);
    return bd_exec(db, sql);
}