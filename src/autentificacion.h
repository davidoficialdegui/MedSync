#include "sqlite3.h"

#ifndef AUTENTIFICACION_H
#define AUTENTIFICACION_H


// Definimos constantes para los tamaños máximos de la línea, usuario y contraseña
#define MAX_LINE 100
#define MAX_USER 50
#define MAX_PASS 50

typedef enum {
    TIPO_DESCONOCIDO,
    TIPO_MEDICO,
    TIPO_PACIENTE,
    TIPO_ADMIN
} TipoUsuario;

TipoUsuario autentificarUsuario(const char *usuario, const char *contrasena);

void autenticarYMostrarMensaje(const char *usuario, const char *contrasena, sqlite3 *db);

// Nueva función para registrar el inicio de sesión
void registrarInicioSesion(const char *usuario);
void registrarLog(const char *usuario);  // Función para registrar el log

#endif