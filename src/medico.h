#ifndef MEDICO_H
#define MEDICO_H

#include "sqlite3.h"

typedef struct {
    char Id_Medico[50];
    char Nombre_M[50];
    char DNI_M[50];
    char Telefono_M[50];
    char Especialidad[50];
} medico;

// Gestión de médicos
void registrarMedico(sqlite3 *db);
void buscarMedico(sqlite3 *db);
void editarMedico(sqlite3 *db);
void eliminarMedico(sqlite3 *db);
void listarMedicos(sqlite3 *db);

// Funcionalidades médicas
void gestionarCitasMedico(sqlite3 *db, const char* id_medico);
void consultarHistorialPaciente(sqlite3 *db);
void atenderCita(sqlite3 *db, const char* id_medico);

#endif