
/*#include "reportes.h"
#include <stdio.h>


static int rpt_cb(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        printf("%s = %s\t", names[i], values[i] ? values[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int reporte_citas_por_medico(sqlite3 *db) {
    const char *sql =
        "SELECT m.nombre || ' ('||m.especialidad||')' AS medico, "
        "       COUNT(c.id) AS total_citas "
        "FROM medico m "
        "LEFT JOIN cita c ON m.id=c.medico_id "
        "GROUP BY m.id;";
    return bd_query(db, sql, rpt_cb, NULL);
}

int reporte_historial_paciente(sqlite3 *db, int paciente_id) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT fecha, descripcion "
        "FROM historial_medico "
        "WHERE paciente_id=%d;", paciente_id);
    return bd_query(db, sql, rpt_cb, NULL);
}

int reporte_usuario_create(sqlite3 *db, int paciente_id, const char *problema) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO reportes(paciente_id,problema) VALUES(%d,'%s');",
        paciente_id, problema);
    return bd_exec(db, sql);
}

int reporte_usuario_list(sqlite3 *db, int paciente_id) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT id, fecha, problema, estado, respuesta "
        "FROM reportes WHERE paciente_id=%d;", paciente_id);
    return bd_query(db, sql, rpt_cb, NULL);
}

int reporte_usuario_list_all(sqlite3 *db) {
    const char *sql =
      "SELECT r.id, r.paciente_id, p.nombre||' '||p.apellidos AS paciente, "
      "       r.fecha, r.problema, r.estado, r.respuesta "
      "FROM reportes r "
      "JOIN paciente p ON p.id=r.paciente_id "
      "ORDER BY r.fecha DESC;";
    return bd_query(db, sql, rpt_cb, NULL);
}*/


// ------------------------FIN DE CODIGO EN C----------------------------



/*
#include "reportes.h"
#include <iostream>
#include <string>

using namespace std;

static int rpt_cb(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        cout << names[i] << " = " << (values[i] ? values[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

int reporte_citas_por_medico(sqlite3 *db) {
    const char *sql =
        "SELECT m.nombre || ' ('||m.especialidad||')' AS medico, "
        "       COUNT(c.id) AS total_citas "
        "FROM medico m "
        "LEFT JOIN cita c ON m.id=c.medico_id "
        "GROUP BY m.id;";
    return bd_query(db, sql, rpt_cb, nullptr);
}

int reporte_historial_paciente(sqlite3 *db, int paciente_id) {
    string sql = "SELECT fecha, descripcion "
                "FROM historial_medico "
                "WHERE paciente_id=" + to_string(paciente_id) + ";";
    return bd_query(db, sql.c_str(), rpt_cb, nullptr);
}

int reporte_usuario_create(sqlite3 *db, int paciente_id, const string &problema) {
    string sql = "INSERT INTO reportes(paciente_id,problema) VALUES(" +
                to_string(paciente_id) + ",'" + problema + "');";
    return bd_exec(db, sql.c_str());
}

int reporte_usuario_list(sqlite3 *db, int paciente_id) {
    string sql = "SELECT id, fecha, problema, estado, respuesta "
                "FROM reportes WHERE paciente_id=" + to_string(paciente_id) + ";";
    return bd_query(db, sql.c_str(), rpt_cb, nullptr);
}

int reporte_usuario_list_all(sqlite3 *db) {
    const char *sql =
      "SELECT r.id, r.paciente_id, p.nombre||' '||p.apellidos AS paciente, "
      "       r.fecha, r.problema, r.estado, r.respuesta "
      "FROM reportes r "
      "JOIN paciente p ON p.id=r.paciente_id "
      "ORDER BY r.fecha DESC;";
    return bd_query(db, sql, rpt_cb, nullptr);
}*/


#include "reportes.h"
#include <iostream>
#include <string>

using namespace std;

static int rpt_cb(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        cout << names[i] << " = " << (values[i] ? values[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

int reporte_citas_por_medico(sqlite3 *db) {
    char *errMsg = nullptr;
    const char *sql =
        "SELECT m.nombre || ' ('||m.especialidad||')' AS medico, "
        "       COUNT(c.id) AS total_citas "
        "FROM medico m "
        "LEFT JOIN cita c ON m.id=c.medico_id "
        "GROUP BY m.id;";
    
    int rc = sqlite3_exec(db, sql, rpt_cb, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error en reporte citas por médico: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

int reporte_historial_paciente(sqlite3 *db, int paciente_id) {
    char *errMsg = nullptr;
    string sql = "SELECT fecha, descripcion "
                "FROM historial_medico "
                "WHERE paciente_id=" + to_string(paciente_id) + ";";
    
    int rc = sqlite3_exec(db, sql.c_str(), rpt_cb, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error en reporte historial paciente: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

int reporte_usuario_create(sqlite3 *db, int paciente_id, const string &problema) {
    char *errMsg = nullptr;
    string sql = "INSERT INTO reportes(paciente_id,problema) VALUES(" +
                to_string(paciente_id) + ",'" + problema + "');";
    
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al crear reporte: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

int reporte_usuario_list(sqlite3 *db, int paciente_id) {
    char *errMsg = nullptr;
    string sql = "SELECT id, fecha, problema, estado, respuesta "
                "FROM reportes WHERE paciente_id=" + to_string(paciente_id) + ";";
    
    int rc = sqlite3_exec(db, sql.c_str(), rpt_cb, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al listar reportes: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}

int reporte_usuario_list_all(sqlite3 *db) {
    char *errMsg = nullptr;
    const char *sql =
      "SELECT r.id, r.paciente_id, p.nombre||' '||p.apellidos AS paciente, "
      "       r.fecha, r.problema, r.estado, r.respuesta "
      "FROM reportes r "
      "JOIN paciente p ON p.id=r.paciente_id "
      "ORDER BY r.fecha DESC;";
    
    int rc = sqlite3_exec(db, sql, rpt_cb, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al listar todos los reportes: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 1;
    }
    return 0;
}
