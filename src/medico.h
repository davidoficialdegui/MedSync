#ifndef MEDICO_H
#define MEDICO_H

#include <sqlite3.h>

int medico_create(sqlite3 *db,
                  const char *nombre,
                  const char *especialidad,
                  const char *telefono,
                  const char *email);

int medico_list(sqlite3 *db);
int medico_update(sqlite3 *db,
                  int id,
                  const char *nombre,
                  const char *especialidad,
                  const char *telefono,
                  const char *email);
int medico_delete(sqlite3 *db, int id);

#endif // MEDICO_H