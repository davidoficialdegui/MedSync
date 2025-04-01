#ifndef EMPLEADO_H
#define EMPLEADO_H
#include "sqlite3.h"

typedef struct 
{
    char Id_Empleado[50];
    char Nombre_E[50];
    char DNI_E[50];
    char Telefono_E[50];
    char Cargo[50];
} empleado;

// Function declarations (actualizadas para recibir conexión a BD)
void registrarNuevoEmpleado(sqlite3 *db);
void buscarEmpleado(sqlite3 *db);
void editarEmpleado(sqlite3 *db);
void eliminarEmpleado(sqlite3 *db);
void listarEmpleados(sqlite3 *db);

#endif