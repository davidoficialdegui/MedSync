#ifndef REPORTES_H
#define REPORTES_H

#include <sqlite3.h>

int reporte_citas_por_medico(sqlite3 *db);
int reporte_historial_paciente(sqlite3 *db, int paciente_id);
int reporte_usuario_create(sqlite3 *db, int paciente_id, const char *problema);
int reporte_usuario_list(sqlite3 *db, int paciente_id);
int reporte_usuario_list_all(sqlite3 *db);

#endif 
