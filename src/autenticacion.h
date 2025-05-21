#ifndef AUTENTICACION_H
#define AUTENTICACION_H

#include <sqlite3.h>

typedef enum { ROLE_UNKNOWN=0, ROLE_ADMIN, ROLE_MEDICO, ROLE_PACIENTE } Role;

Role autenticar(sqlite3 *db, const char *username, const char *password);
void ensure_default_admin(sqlite3 *db);

#endif 