#ifndef REPORTES_H
#define REPORTES_H

#include <sqlite3.h>
#include <time.h>

#define MAX_REPORTES 100

// Estructura Reporte
typedef struct {
    int Id_Reporte;         // ID único del reporte
    char Descripcion[255];  // Descripción del reporte
    char Fecha_R[11];       // Fecha de generación del reporte (formato dd/mm/yyyy)
    int Id_Paciente;        // ID del paciente (opcional, dependiendo del tipo de reporte)
    int Id_Empleado;        // ID del empleado que genera el reporte
    int Id_Medico;          // ID del médico (opcional, dependiendo del tipo de reporte)
} Reporte;

// Funciones para los reportes
int abrirBD(const char *nombre_bd);
void cerrarBD();
void generarReporte(const char *tipo_reporte, const char *descripcion, int Id_Paciente, int Id_Empleado, int Id_Medico);
void consultarReportes();
void generarReporteCitasProgramadas();
void generarReporteHistorialesMedicos();
void mostrarReporteCitas();
void mostrarReporteHistoriales();

#endif // REPORTES_H
