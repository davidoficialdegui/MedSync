#ifndef CITAS_H
#define CITAS_H

#include <sqlite3.h>

int cita_create(sqlite3 *db, int paciente_id, int medico_id, const char *fecha_hora, const char *motivo);
int cita_list(sqlite3 *db);
int cita_update(sqlite3 *db, int id, int paciente_id, int medico_id, const char *fecha_hora, const char *motivo);
int cita_delete(sqlite3 *db, int id);

#endif 