#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DB_NAME "MedSync.bd"

void agregarCita(int id_paciente, int id_medico, const char *fecha, const char *hora, const char *motivo) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[512];
    snprintf(sql, sizeof(sql), "INSERT INTO Cita_Medica (Fecha_C, Motivo, Estado, Id_Paciente, Id_Medico) VALUES ('%s %s', '%s', 'Programada', %d, %d);", fecha, hora, motivo, id_paciente, id_medico);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al agregar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita programada con éxito.\n");
    }
    sqlite3_close(db);
}

void modificarCita(int id_cita, const char *nueva_fecha, const char *nueva_hora, const char *nuevo_motivo) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[512];
    snprintf(sql, sizeof(sql), "UPDATE Cita_Medica SET Fecha_C='%s %s', Motivo='%s' WHERE Id_Cita=%d;", nueva_fecha, nueva_hora, nuevo_motivo, id_cita);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al modificar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita modificada exitosamente.\n");
    }
    sqlite3_close(db);
}

void cancelarCita(int id_cita) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "UPDATE Cita_Medica SET Estado='Cancelada' WHERE Id_Cita=%d;", id_cita);

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al cancelar la cita: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Cita cancelada con éxito.\n");
    }
    sqlite3_close(db);
}

void listarCitasPaciente(int id_paciente) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT Id_Cita, Fecha_C, Motivo, Estado FROM Cita_Medica WHERE Id_Paciente=%d;", id_paciente);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener citas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n--- Citas del Paciente %d ---\n", id_paciente);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d | Fecha: %s | Motivo: %s | Estado: %s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void listarCitasMedico(int id_medico) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT Id_Cita, Id_Paciente, Fecha_C, Estado FROM Cita_Medica WHERE Id_Medico=%d;", id_medico);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener citas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n--- Citas del Médico %d ---\n", id_medico);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d | Paciente: %d | Fecha: %s | Estado: %s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_int(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
