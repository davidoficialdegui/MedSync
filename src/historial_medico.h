#ifndef HISTORIAL_MEDICO_H
#define HISTORIAL_MEDICO_H

#include <sqlite3.h>

int historial_create(sqlite3 *db, int paciente_id, const char *fecha, const char *descripcion);
int historial_list(sqlite3 *db, int paciente_id);
int historial_delete(sqlite3 *db, int id);

#endif 