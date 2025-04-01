#ifndef REPORTES_H
#define REPORTES_H

#include "sqlite3.h"
#include <time.h>

#define MAX_REPORTES 100

typedef struct {
    int Id_Reporte;         
    char Descripcion[255];  
    char Fecha_R[11];       
    int Id_Paciente;       
    int Id_Empleado;        
    int Id_Medico;         
} Reporte;

int abrirBD(const char *nombre_bd);
void cerrarBD();
void generarReporte(const char *tipo_reporte, const char *descripcion, int Id_Paciente, int Id_Empleado, int Id_Medico);
void consultarReportes();
void generarReporteCitasProgramadas();
void generarReporteHistorialesMedicos();
void mostrarReporteCitas();
void mostrarReporteHistoriales();

#endif 
