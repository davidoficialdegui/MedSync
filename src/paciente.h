#ifndef PACIENTE_H
#define PACIENTE_H

#include "sqlite3.h"

typedef struct 
{  
    char Id_Paciente[50];
	char DNI_P[50];
	char Nombre_P [50];
	char Email[50];
	char Fecha_Ncto[50];
	char Genero[50];
	char Telefono_P[50];
	char Direccion_P[50];
	char Fecha_Reg[50];
}paciente;



// Función para validar si una cadena representa una fecha en formato dd/mm/yyyy
int esFechaValida(const char *fecha);

// Función para obtener la fecha actual del sistema en formato dd/mm/yyyy
void obtenerFechaActual(int *dia, int *mes, int *anio);

// Función para verificar si una fecha es futura
int esFechaFutura(int diaCita, int mesCita, int anioCita);

// Función para obtener un ID de médico aleatorio
char* obtenerIdMedicoAleatorio(sqlite3 *db);

// Función para verificar si un ID de cita ya existe
int idCitaExiste(sqlite3 *db, const char *idCita);

// Función para generar un ID de cita único
char *generarIdCita(sqlite3 *db);

// Función para registrar una nueva cita
void registrarCita(sqlite3 *db, const char *fecha, const char *motivo, const char *estado, const char *id_paciente);

// Función para consultar todas las citas
void consultarCitas(sqlite3 *db);

// Función para cancelar una cita por su ID
void cancelarCita(sqlite3 *db, const char *id_cita);

// Función para gestionar las citas médicas de un paciente
void gestionarCitas(sqlite3 *db, const char *idPaciente);

// Función para consultar el historial médico de un paciente
void consultarHistorial(sqlite3 *db, const char *id_paciente);

// Función para generar un ID único para el reporte
char* generarIdReporte();

// Función para registrar un reporte en la base de datos
void registrarReporte(sqlite3 *db,const char *descripcion, const char *fecha, const char *id_paciente, const char *id_empleado, const char *id_medico);

// Función para la atención al cliente y registro de reporte
void atencionCliente(sqlite3 *db);

#endif
