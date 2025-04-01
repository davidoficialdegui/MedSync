#include "sqlite3.h"
#ifndef ADMIN_H
#define ADMIN_H

typedef struct
{
    int id;
    char nombre[50];
    char apellido1[50];
    char apellido2[50];
    char dni[10];
    char cargo[50];
    char telefono[15];
    char email[100];
} Empleado;

// metodos para paciente
void gestionarPacientes(sqlite3 *db);
void registrarNuevoPaciente(sqlite3 *db);
void BuscarPaciente(sqlite3 *db);
void EditarPaciente(sqlite3 *db);
void EliminaPaciente(sqlite3 *db);

// metodos para empleado
void gestionarEmpleados(sqlite3 *db);
void registrarNuevoEmpleado(sqlite3 *db);
void BuscarEmpleado(sqlite3 *db);
void EditarEmpleado(sqlite3 *db);
void EliminaEmpleado(sqlite3 *db);

// metodos para reporte
void generarReportes(sqlite3 *db);
void reportePacientes(sqlite3 *db);
void reporteEmpleados(sqlite3 *db);
void reporteMedicos(sqlite3 *db);

#endif