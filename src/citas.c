#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "citas.h"

#define MAX_CITAS 100

typedef struct {
    int id_cita;
    int id_paciente;
    int id_medico;
    char fecha[11];
    char hora[6];
    char motivo[100];
    char estado[20];
} Cita;

Cita citas[MAX_CITAS];
int totalCitas = 0;

void agregarCita(int id_paciente, int id_medico, const char *fecha, const char *hora, const char *motivo) {
    if (totalCitas < MAX_CITAS) {
        citas[totalCitas].id_cita = totalCitas + 1;
        citas[totalCitas].id_paciente = id_paciente;
        citas[totalCitas].id_medico = id_medico;
        strcpy(citas[totalCitas].fecha, fecha);
        strcpy(citas[totalCitas].hora, hora);
        strcpy(citas[totalCitas].motivo, motivo);
        strcpy(citas[totalCitas].estado, "Programada");
        totalCitas++;
        printf("Cita programada con éxito. ID de cita: %d\n", totalCitas);
    } else {
        printf("No se pueden agendar más citas.\n");
    }
}

void modificarCita(int id_cita, const char *nueva_fecha, const char *nueva_hora, const char *nuevo_motivo) {
    for (int i = 0; i < totalCitas; i++) {
        if (citas[i].id_cita == id_cita) {
            strcpy(citas[i].fecha, nueva_fecha);
            strcpy(citas[i].hora, nueva_hora);
            strcpy(citas[i].motivo, nuevo_motivo);
            printf("Cita modificada exitosamente.\n");
            return;
        }
    }
    printf("Cita no encontrada.\n");
}

void cancelarCita(int id_cita) {
    for (int i = 0; i < totalCitas; i++) {
        if (citas[i].id_cita == id_cita) {
            strcpy(citas[i].estado, "Cancelada");
            printf("Cita cancelada con éxito.\n");
            return;
        }
    }
    printf("Cita no encontrada.\n");
}

void listarCitasPaciente(int id_paciente) {
    printf("\n--- Citas del Paciente %d ---\n", id_paciente);
    int found = 0;
    for (int i = 0; i < totalCitas; i++) {
        if (citas[i].id_paciente == id_paciente) {
            printf("ID: %d | Fecha: %s | Hora: %s | Motivo: %s | Estado: %s\n",
                   citas[i].id_cita, citas[i].fecha, citas[i].hora, citas[i].motivo, citas[i].estado);
            found = 1;
        }
    }
    if (!found) {
        printf("No hay citas programadas para este paciente.\n");
    }
}

void listarCitasMedico(int id_medico) {
    printf("\n--- Citas del Médico %d ---\n", id_medico);
    int found = 0;
    for (int i = 0; i < totalCitas; i++) {
        if (citas[i].id_medico == id_medico) {
            printf("ID: %d | Paciente: %d | Fecha: %s | Hora: %s | Estado: %s\n",
                   citas[i].id_cita, citas[i].id_paciente, citas[i].fecha, citas[i].hora, citas[i].estado);
            found = 1;
        }
    }
    if (!found) {
        printf("No hay citas programadas para este médico.\n");
    }
}
