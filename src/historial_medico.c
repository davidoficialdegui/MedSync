#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historialmedico.h"
#include "paciente.h" 
#include "medico.h" 
#include "sqlite3.h"

sqlite3 *db;

int abrirBD(const char *nombre_bd) {
    int rc = sqlite3_open(nombre_bd, &db);
    if (rc) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return 0;
}

void cerrarBD() {
    sqlite3_close(db);
}

int ejecutarConsulta(const char *sql) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

void registrarHistorial(int Id_Paciente, int Id_Medico, const char *Diagnostico, const char *Tratamiento, const char *Observaciones) {
    char sql[512];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char Fecha[11];
    sprintf(Fecha, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    snprintf(sql, sizeof(sql),
        "INSERT INTO Historial_Medico (Diagnostico, Tratamiento, Observaciones, Id_Paciente, Id_Medico, Id_Cita, Fecha) "
        "VALUES ('%s', '%s', '%s', %d, %d, NULL, '%s');",
        Diagnostico, Tratamiento, Observaciones, Id_Paciente, Id_Medico, Fecha);

    if (ejecutarConsulta(sql) == SQLITE_OK) {
        printf("Historial médico registrado con éxito.\n");
    } else {
        printf("Error al registrar el historial médico.\n");
    }
}

void consultarHistorialesPaciente(int Id_Paciente) {
    const char *sql = "SELECT * FROM Historial_Medico WHERE Id_Paciente = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, Id_Paciente);

    int found = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("ID Historial: %d | Fecha: %s | Diagnóstico: %s | Tratamiento: %s | Observaciones: %s\n",
               sqlite3_column_int(stmt, 0),  
               sqlite3_column_text(stmt, 6),  
               sqlite3_column_text(stmt, 1),  
               sqlite3_column_text(stmt, 2),  
               sqlite3_column_text(stmt, 3)); 
        found = 1;
    }

    sqlite3_finalize(stmt);
    if (!found) {
        printf("No hay historiales médicos para este paciente.\n");
    }
}

void mostrarHistoriales() {
    const char *sql = "SELECT * FROM Historial_Medico";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n--- Todos los Historiales Médicos ---\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("ID Historial: %d | Paciente ID: %d | Médico ID: %d | Fecha: %s\n",
               sqlite3_column_int(stmt, 0),  
               sqlite3_column_int(stmt, 4), 
               sqlite3_column_int(stmt, 5),  
               sqlite3_column_text(stmt, 6)); 
    }

    sqlite3_finalize(stmt);
}

int main() {
    if (abrirBD("MedSync.db") != 0) {
        return 1;
    }

    registrarHistorial(1, 2, "Diagnóstico Ejemplo", "Tratamiento Ejemplo", "Observaciones Ejemplo");
    consultarHistorialesPaciente(1);
    mostrarHistoriales();

    cerrarBD();
    return 0;
}
