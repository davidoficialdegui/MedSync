#ifndef CITAS_H
#define CITAS_H

#include <sqlite3.h>

typedef struct {
    int Id_Cita;
    char Fecha_C[20];
    char Motivo[100];
    char Estado[20];
    int Id_Paciente;
    int Id_Medico;
} Cita;

void agregarCita(int Id_Paciente, int Id_Medico, const char *Fecha, const char *Hora, const char *Motivo);
void modificarCita(int Id_Cita, const char *Nueva_Fecha, const char *Nueva_Hora, const char *Nuevo_Motivo);
void cancelarCita(int Id_Cita);
void listarCitasPaciente(int Id_Paciente);
void listarCitasMedico(int Id_Medico);

#endif 
