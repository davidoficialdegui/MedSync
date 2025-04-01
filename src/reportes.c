#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include "reportes.h"
#include "citas.h"
#include "historialmedico.h"

#define DB_NAME "MedSync.bd"

sqlite3 *db;

// Función para abrir la base de datos
int abrirBD(const char *nombre_bd) {
    int rc = sqlite3_open(nombre_bd, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return 0;
}

// Función para cerrar la base de datos
void cerrarBD() {
    sqlite3_close(db);
}

// Función para ejecutar las consultas SQL
int ejecutarConsulta(const char *sql) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al ejecutar la consulta: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

// Función para generar un reporte
void generarReporte(const char *tipo_reporte, const char *descripcion, int Id_Paciente, int Id_Empleado, int Id_Medico) {
    // Obtener la fecha actual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char fecha_actual[11];
    sprintf(fecha_actual, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Crear el SQL para insertar el reporte en la base de datos
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO Reporte (Descripcion, Fecha_R, Id_Paciente, Id_Empleado, Id_Medico) "
        "VALUES ('%s', '%s', %d, %d, %d);",
        descripcion, fecha_actual, Id_Paciente, Id_Empleado, Id_Medico);

    // Ejecutar la consulta SQL
    if (ejecutarConsulta(sql) == SQLITE_OK) {
        printf("Reporte generado con éxito.\n");
    } else {
        printf("Error al generar el reporte.\n");
    }
}

// Función para consultar todos los reportes
void consultarReportes() {
    sqlite3_stmt *stmt;
    char sql[] = "SELECT * FROM Reporte;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener reportes: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n--- Reportes Generados ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d | Descripción: %s | Fecha de Generación: %s\n",
               sqlite3_column_int(stmt, 0),  // Id_Reporte
               sqlite3_column_text(stmt, 1),  // Descripcion
               sqlite3_column_text(stmt, 2)); // Fecha_R
    }

    sqlite3_finalize(stmt);
}

// Función para generar un reporte de citas programadas
void generarReporteCitasProgramadas() {
    char descripcion[200];
    strcpy(descripcion, "Reporte de todas las citas programadas.");
    generarReporte("Citas Programadas", descripcion, 0, 1, 0);  // Aquí 0 es para pacientes o médicos si no se requiere especificar
}

// Función para generar un reporte de historiales médicos
void generarReporteHistorialesMedicos() {
    char descripcion[200];
    strcpy(descripcion, "Reporte de todos los historiales médicos.");
    generarReporte("Historiales Médicos", descripcion, 0, 1, 0);  // Aquí 0 es para pacientes o médicos si no se requiere especificar
}

// Función para mostrar los reportes de citas programadas
void mostrarReporteCitas() {
    printf("\n--- Reportes de Citas Programadas ---\n");
    for (int i = 0; i < totalReportes; i++) {
        if (strcmp(reportes[i].Descripcion, "Citas Programadas") == 0) {
            printf("ID: %d | Descripción: %s | Fecha de Generación: %s\n",
                   reportes[i].Id_Reporte, reportes[i].Descripcion, reportes[i].Fecha_R);
        }
    }
}

// Función para mostrar los reportes de historiales médicos
void mostrarReporteHistoriales() {
    printf("\n--- Reportes de Historiales Médicos ---\n");
    for (int i = 0; i < totalReportes; i++) {
        if (strcmp(reportes[i].Descripcion, "Historiales Médicos") == 0) {
            printf("ID: %d | Descripción: %s | Fecha de Generación: %s\n",
                   reportes[i].Id_Reporte, reportes[i].Descripcion, reportes[i].Fecha_R);
        }
    }
}
