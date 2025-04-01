#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historialmedico.h"
#include "paciente.h" 
#include "medico.h" 
#include "sqlite3.h"

#define MAX_HISTORIALES 100
HistorialMedico historialMedico[MAX_HISTORIALES];
int totalHistoriares = 0;

void registrarHistorial(int id_paciente, int id_medico, const char *diagnostico, const char *tratamiento, const char *observaciones, sqlite3 *db) {
    if (totalHistoriares < MAX_HISTORIALES) {
        HistorialMedico nuevoHistorial;
        nuevoHistorial.id_historial = totalHistoriares + 1;
        nuevoHistorial.id_paciente = id_paciente;
        nuevoHistorial.id_medico = id_medico;
        strcpy(nuevoHistorial.diagnostico, diagnostico);
        strcpy(nuevoHistorial.tratamiento, tratamiento);
        strcpy(nuevoHistorial.observaciones, observaciones);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(nuevoHistorial.fecha, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

        historialMedico[totalHistoriares] = nuevoHistorial;
        totalHistoriares++;

        printf("Historial médico registrado con éxito. ID del historial: %d\n", nuevoHistorial.id_historial);
    } else {
        printf("No se pueden registrar más historiales médicos.\n");
    }
}

void consultarHistorialesPaciente(int id_paciente) {
    int found = 0;
    printf("\n--- Historial Médico del Paciente %d ---\n", id_paciente);
    for (int i = 0; i < totalHistoriares; i++) {
        if (historialMedico[i].id_paciente == id_paciente) {
            printf("ID Historial: %d | Fecha: %s | Diagnóstico: %s | Tratamiento: %s | Observaciones: %s\n",
                   historialMedico[i].id_historial, historialMedico[i].fecha, historialMedico[i].diagnostico,
                   historialMedico[i].tratamiento, historialMedico[i].observaciones);
            found = 1;
        }
    }
    if (!found) {
        printf("No hay historiales médicos para este paciente.\n");
    }
}

void mostrarHistoriales() {
    if (totalHistoriares == 0) {
        printf("No hay historiales médicos registrados.\n");
        return;
    }

    printf("\n--- Todos los Historiales Médicos ---\n");
    for (int i = 0; i < totalHistoriares; i++) {
        printf("ID Historial: %d | Paciente ID: %d | Médico ID: %d | Fecha: %s\n",
               historialMedico[i].id_historial, historialMedico[i].id_paciente, historialMedico[i].id_medico, historialMedico[i].fecha);
    }
}
