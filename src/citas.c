#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "citas.h"
#include "sqlite3.h"
#define DB_NAME "MedSync.bd"


void agregarCita(int Id_Paciente, int Id_Medico, const char *Fecha, const char *Hora, const char *Motivo) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[512];
    snprintf(sql, sizeof(sql), "INSERT INTO Cita_Medica (Fecha_C, Motivo, Estado, Id_Paciente, Id_Medico) VALUES ('%s %s', '%s', 'Programada', %d, %d);", Fecha, Hora, Motivo, Id_Paciente, Id_Medico);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al agregar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita programada con éxito.\n");
    }
    sqlite3_close(db);
}


void modificarCita(int Id_Cita, const char *Nueva_Fecha, const char *Nueva_Hora, const char *Nuevo_Motivo) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[512];
    snprintf(sql, sizeof(sql), "UPDATE Cita_Medica SET Fecha_C='%s %s', Motivo='%s' WHERE Id_Cita=%d;", Nueva_Fecha, Nueva_Hora, Nuevo_Motivo, Id_Cita);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al modificar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita modificada exitosamente.\n");
    }
    sqlite3_close(db);
}


void cancelarCita(int Id_Cita) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "UPDATE Cita_Medica SET Estado='Cancelada' WHERE Id_Cita=%d;", Id_Cita);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al cancelar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita cancelada con éxito.\n");
    }
    sqlite3_close(db);
}

void listarCitasPaciente(int Id_Paciente) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT Id_Cita, Fecha_C, Motivo, Estado FROM Cita_Medica WHERE Id_Paciente=%d;", Id_Paciente);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener citas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;

    }

    printf("\n--- Citas del Paciente %d ---\n", Id_Paciente);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID Cita: %d | Fecha: %s | Motivo: %s | Estado: %s\n",
               sqlite3_column_int(stmt, 0),  
               sqlite3_column_text(stmt, 1),  
               sqlite3_column_text(stmt, 2),  
               sqlite3_column_text(stmt, 3)); 
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void listarCitasMedico(int Id_Medico) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT Id_Cita, Id_Paciente, Fecha_C, Estado FROM Cita_Medica WHERE Id_Medico=%d;", Id_Medico);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener citas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n--- Citas del Médico %d ---\n", Id_Medico);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID Cita: %d | Paciente: %d | Fecha: %s | Estado: %s\n",
               sqlite3_column_int(stmt, 0), 
               sqlite3_column_int(stmt, 1),  
               sqlite3_column_text(stmt, 2),  
               sqlite3_column_text(stmt, 3)); 
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


