#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reportes.h"
#include "citas.h" 
#include "historialmedico.h" 


#define MAX_REPORTES 100
Reporte reportes[MAX_REPORTES];
int totalReportes = 0;

void generarReporte(const char *tipo_reporte, const char *descripcion) {
    if (totalReportes < MAX_REPORTES) {
        Reporte nuevo_reporte;
        nuevo_reporte.id_reporte = totalReportes + 1;
        strcpy(nuevo_reporte.tipo_reporte, tipo_reporte);
        strcpy(nuevo_reporte.descripcion, descripcion);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(nuevo_reporte.fecha_generacion, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

        reportes[totalReportes] = nuevo_reporte;
        totalReportes++;

        printf("Reporte generado con éxito. ID de reporte: %d\n", nuevo_reporte.id_reporte);
    } else {
        printf("No se pueden generar más reportes. Se ha alcanzado el límite máximo.\n");
    }
}

void consultarReportes() {
    if (totalReportes == 0) {
        printf("No hay reportes generados.\n");
        return;
    }

    printf("\n--- Reportes Generados ---\n");
    for (int i = 0; i < totalReportes; i++) {
        printf("ID: %d | Tipo: %s | Descripción: %s | Fecha de Generación: %s\n",
               reportes[i].id_reporte, reportes[i].tipo_reporte, reportes[i].descripcion, reportes[i].fecha_generacion);
    }
}

void generarReporteCitasProgramadas() {
    char descripcion[200];
    strcpy(descripcion, "Reporte de todas las citas programadas.");
    generarReporte("Citas Programadas", descripcion);
}

void generarReporteHistorialesMedicos() {
    char descripcion[200];
    strcpy(descripcion, "Reporte de todos los historiales médicos.");
    generarReporte("Historiales Médicos", descripcion);
}

void mostrarReporteCitas() {
    printf("\n--- Reportes de Citas Programadas ---\n");
    for (int i = 0; i < totalReportes; i++) {
        if (strcmp(reportes[i].tipo_reporte, "Citas Programadas") == 0) {
            printf("ID: %d | Descripción: %s | Fecha de Generación: %s\n",
                   reportes[i].id_reporte, reportes[i].descripcion, reportes[i].fecha_generacion);
        }
    }
}

void mostrarReporteHistoriales() {
    printf("\n--- Reportes de Historiales Médicos ---\n");
    for (int i = 0; i < totalReportes; i++) {
        if (strcmp(reportes[i].tipo_reporte, "Historiales Médicos") == 0) {
            printf("ID: %d | Descripción: %s | Fecha de Generación: %s\n",
                   reportes[i].id_reporte, reportes[i].descripcion, reportes[i].fecha_generacion);
        }
    }
}
