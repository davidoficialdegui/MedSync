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

void agregarCita(int id_paciente, int id_medico, const char *fecha, const char *hora, const char *motivo);
void modificarCita(int id_cita, const char *nueva_fecha, const char *nueva_hora, const char *nuevo_motivo);
void cancelarCita(int id_cita);
void listarCitasPaciente(int id_paciente);
void listarCitasMedico(int id_medico);

#endif // CITAS_H