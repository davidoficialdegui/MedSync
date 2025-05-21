#ifndef BD_H
#define BD_H

#include <sqlite3.h>

/**
 * Abre o crea una base de datos SQLite y habilita claves foráneas.
 * @param filename Ruta al archivo de la BD.
 * @param db_out Puntero donde se devuelve la conexión.
 * @return 0 si OK, !=0 si error.
 */
int bd_open(const char *filename, sqlite3 **db_out);

/**
 * Cierra la conexión a la base de datos.
 */
void bd_close(sqlite3 *db);

/**
 * Ejecuta una sentencia SQL sin resultados (CREATE, INSERT, UPDATE, DELETE).
 * @return SQLITE_OK si OK, otro código SQLite si error.
 */
int bd_exec(sqlite3 *db, const char *sql);

/**
 * Ejecuta una consulta SELECT con callback.
 * @param callback Función que se llama por fila.
 * @param param Parámetro que se pasa al callback.
 * @return SQLITE_OK si OK, otro código SQLite si error.
 */
int bd_query(sqlite3 *db, const char *sql,
             int (*callback)(void*,int,char**,char**),
             void *param);

#endif // BD_H