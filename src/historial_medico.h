#ifndef HISTORIAL_MEDICO_H
#define HISTORIAL_MEDICO_H

#include "paciente.h"  

typedef struct {
    int id_historial;
    int id_paciente;
    int id_medico;
    char fecha[20];
    char diagnostico[200];
    char tratamiento[200];
    char observaciones[200];
} HistorialMedico;

void registrarHistorial(int id_paciente, int id_medico, const char *diagnostico, const char *tratamiento, const char *observaciones);

void consultarHistorialesPaciente(int id_paciente);

void mostrarHistoriales();

#endif 
