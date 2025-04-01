#ifndef HISTORIALMEDICO_H
#define HISTORIALMEDICO_H

#include <sqlite3.h>

typedef struct {
    int Id_Hospital;       
    char Diagnostico[255]; 
    char Tratamiento[255]; 
    char Observaciones[255]; 
    int Id_Paciente;      
    int Id_Medico;         
    int Id_Cita;           
    char Fecha[11];        
} HistorialMedico;

int abrirBD(const char *nombre_bd);
void cerrarBD();
int ejecutarConsulta(const char *sql);

void registrarHistorial(int Id_Paciente, int Id_Medico, const char *Diagnostico, const char *Tratamiento, const char *Observaciones);
void consultarHistorialesPaciente(int Id_Paciente);
void mostrarHistoriales();

#endif 
