/*#ifndef PACIENTE_H
#define PACIENTE_H

#include <sqlite3.h>

int paciente_create(sqlite3 *db,  const char *nombre, const char *apellidos, const char *fecha_nac, const char *genero,  const char *telefono, const char *email);

int paciente_list(sqlite3 *db);
int paciente_update(sqlite3 *db,  int id,  const char *nombre,  const char *apellidos , const char *fecha_nac,   const char *genero,  const char *telefono, const char *email);
int paciente_delete(sqlite3 *db, int id);

#endif // PACIENTE_H */




// ------------------------FIN DE CODIGO EN C----------------------------




#ifndef PACIENTE_H
#define PACIENTE_H

#include <sqlite3.h>
#include <string>

int paciente_create(sqlite3 *db,
                   const std::string &nombre,
                   const std::string &apellidos,
                   const std::string &fecha_nac,
                   const std::string &genero = "O",
                   const std::string &telefono = "",
                   const std::string &email = "");

int paciente_list(sqlite3 *db);
int paciente_update(sqlite3 *db,
                   int id,
                   const std::string &nombre,
                   const std::string &apellidos,
                   const std::string &fecha_nac,
                   const std::string &genero = "O",
                   const std::string &telefono = "",
                   const std::string &email = "");
int paciente_delete(sqlite3 *db, int id);

#endif // PACIENTE_H