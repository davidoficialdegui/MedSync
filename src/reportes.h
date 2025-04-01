#ifndef REPORTES_H
#define REPORTES_H

typedef struct {
    int id_reporte;
    char tipo_reporte[50];
    char descripcion[200];
    char fecha_generacion[20];
} Reporte;

void generarReporte(const char *tipo_reporte, const char *descripcion);

void consultarReportes();

void generarReporteCitasProgramadas();

void generarReporteHistorialesMedicos();

void mostrarReporteCitas();

void mostrarReporteHistoriales();

#endif 
