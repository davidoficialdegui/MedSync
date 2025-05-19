// src/autentificacion.h
#ifndef AUTENTIFICACION_H
#define AUTENTIFICACION_H

#include <sqlite3.h>

#define MAX_LINE   256
#define MAX_USER    50
#define MAX_PASS    50

typedef enum {
    TIPO_DESCONOCIDO = 0,
    TIPO_MEDICO,
    TIPO_PACIENTE,
    TIPO_ADMIN
} TipoUsuario;

// Lee ficheros/usuarios.txt, busca usuario+contrasena y devuelve su tipo.
TipoUsuario autentificarUsuario(const char *usuario, const char *contrasena);

// Tras autenticar, muestra el menú correspondiente (usa menu.h).
void autenticarYMostrarMensaje(const char *usuario, const char *contrasena, sqlite3 *db);

#endif // AUTENTIFICACION_H
