#ifndef HISTORIAL_MEDICO_H
#define HISTORIAL_MEDICO_H

#include "sqlite3.h"

typedef struct {
    int Id_Historial;       // Corrección del nombre (antes Id_Hospital)
    char Diagnostico[255];  
    char Tratamiento[255];  
    char Observaciones[255];  
    int Id_Paciente;        
    int Id_Medico;          
    int Id_Cita;            
    char Fecha[11];         
} HistorialMedico;

// Funciones de conexión a la base de datos
int abrirBD(const char *nombre_bd);
void cerrarBD();
int ejecutarConsultaHistorial(const char *sql);

// Funciones de gestión del historial médico
void registrarHistorial(int Id_Paciente, int Id_Medico, const char *Diagnostico, const char *Tratamiento, const char *Observaciones);
void consultarHistorialesPaciente(int Id_Paciente);
void mostrarHistoriales();

#endif // HISTORIAL_MEDICO_H
