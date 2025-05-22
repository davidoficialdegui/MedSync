/*#ifndef MEDICO_H
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

#endif // MEDICO_H*/

#include "medico.h"
#include <iostream>
#include <string>

using namespace std;

static int medico_callback(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        cout << names[i] << " = " << (values[i] ? values[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

int medico_create(sqlite3 *db,
                 const string &nombre,
                 const string &especialidad,
                 const string &telefono,
                 const string &email) {
    string sql = "INSERT INTO medico(nombre,especialidad,telefono,email) VALUES('" +
                nombre + "','" + especialidad + "','" + telefono + "','" + email + "');";
    return bd_exec(db, sql.c_str());
}

int medico_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,nombre,especialidad,telefono,email FROM medico;", 
                   medico_callback, nullptr);
}

int medico_update(sqlite3 *db, int id,
                 const string &nombre,
                 const string &especialidad,
                 const string &telefono,
                 const string &email) {
    string sql = "UPDATE medico SET nombre='" + nombre + 
                 "',especialidad='" + especialidad + 
                 "',telefono='" + telefono + 
                 "',email='" + email + 
                 "' WHERE id=" + to_string(id) + ";";
    return bd_exec(db, sql.c_str());
}

int medico_delete(sqlite3 *db, int id) {
    string sql = "DELETE FROM medico WHERE id=" + to_string(id) + ";";
    return bd_exec(db, sql.c_str());
}