#ifndef ADMIN_H
#define ADMIN_H

typedef struct {
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
void gestionarPacientes();
void registrarNuevoPaciente();
void BuscarPaciente();
void EditarPaciente();
void EliminaPaciente();

// metodos para empleado
void gestionarEmpleados();
void registrarNuevoEmpleado();
void BuscarEmpleado();
void EditarEmpleado();
void EliminaEmpleado();

// metodos para reporte
void generarReportes();
void reportePacientes();
void reporteEmpleados();
void reporteMedicos();

#endif