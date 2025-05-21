#ifndef PACIENTE_H
#define PACIENTE_H

#include <sqlite3.h>

int paciente_create(sqlite3 *db,  const char *nombre, const char *apellidos, const char *fecha_nac, const char *genero,  const char *telefono, const char *email);

int paciente_list(sqlite3 *db);
int paciente_update(sqlite3 *db,  int id,  const char *nombre,  const char *apellidos , const char *fecha_nac,   const char *genero,  const char *telefono, const char *email);
int paciente_delete(sqlite3 *db, int id);

#endif // PACIENTE_H