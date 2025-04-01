#ifndef CITAS_H
#define CITAS_H

#define MAX_CITAS 100

typedef struct {
    int id_cita;
    int id_paciente;
    int id_medico;
    char fecha[20];
    char hora[6];
    char motivo[100];
    char estado[20];  
} Cita;

extern Cita citas[MAX_CITAS];
extern int totalCitas;

void agregarCita(int id_paciente, int id_medico, const char *fecha, const char *hora, const char *motivo);
void modificarCita(int id_cita, const char *nueva_fecha, const char *nueva_hora, const char *nuevo_motivo);
void cancelarCita(int id_cita);
void listarCitasPaciente(int id_paciente);
void listarCitasMedico(int id_medico);

#endif 