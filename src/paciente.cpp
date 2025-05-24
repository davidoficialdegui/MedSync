/*#include "paciente.h"
#include <stdio.h>

static int paciente_callback(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) printf("%s = %s\t", names[i], values[i] ? values[i] : "NULL");
    printf("\n");
    return 0;
}

int paciente_create(sqlite3 *db,
                    const char *nombre,
                    const char *apellidos,
                    const char *fecha_nac,
                    const char *genero,
                    const char *telefono,
                    const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO paciente(nombre,apellidos,fecha_nac,genero,telefono,email) VALUES('%s','%s','%s','%s','%s','%s');",
             nombre, apellidos, fecha_nac, genero, telefono, email);
    return bd_exec(db, sql);
}

int paciente_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,nombre,apellidos,fecha_nac,genero,telefono,email FROM paciente;", paciente_callback, NULL);
}

int paciente_update(sqlite3 *db, int id,
                    const char *nombre,
                    const char *apellidos,
                    const char *fecha_nac,
                    const char *genero,
                    const char *telefono,
                    const char *email) {
    char sql[512];
    snprintf(sql, sizeof(sql),
             "UPDATE paciente SET nombre='%s',apellidos='%s',fecha_nac='%s',genero='%s',telefono='%s',email='%s' WHERE id=%d;",
             nombre, apellidos, fecha_nac, genero, telefono, email, id);
    return bd_exec(db, sql);
}

int paciente_delete(sqlite3 *db, int id) {
    char sql[128]; snprintf(sql, sizeof(sql), "DELETE FROM paciente WHERE id=%d;", id);
    return bd_exec(db, sql);
}*/




// ------------------------FIN DE CODIGO EN C----------------------------

#include "paciente.h"
#include <iostream>
#include <string>

using namespace std;

static int paciente_callback(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        cout << names[i] << " = " << (values[i] ? values[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

int paciente_create(sqlite3 *db,
                   const string &nombre,
                   const string &apellidos,
                   const string &fecha_nac,
                   const string &genero,
                   const string &telefono,
                   const string &email) {
    string sql = "INSERT INTO paciente(nombre,apellidos,fecha_nac,genero,telefono,email) VALUES('" +
                nombre + "','" + apellidos + "','" + fecha_nac + "','" + genero + "','" + 
                telefono + "','" + email + "');";
    return bd_exec(db, sql.c_str());
}

int paciente_list(sqlite3 *db) {
    return bd_query(db, "SELECT id,nombre,apellidos,fecha_nac,genero,telefono,email FROM paciente;",
                   paciente_callback, nullptr);
}

int paciente_update(sqlite3 *db, int id,
                   const string &nombre,
                   const string &apellidos,
                   const string &fecha_nac,
                   const string &genero,
                   const string &telefono,
                   const string &email) {
    string sql = "UPDATE paciente SET nombre='" + nombre + 
                "',apellidos='" + apellidos + 
                "',fecha_nac='" + fecha_nac + 
                "',genero='" + genero + 
                "',telefono='" + telefono + 
                "',email='" + email + 
                "' WHERE id=" + to_string(id) + ";";
    return bd_exec(db, sql.c_str());
}

int paciente_delete(sqlite3 *db, int id) {
    string sql = "DELETE FROM paciente WHERE id=" + to_string(id) + ";";
    return bd_exec(db, sql.c_str());
}