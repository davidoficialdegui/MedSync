#include <stdio.h>
#include <string.h>
#include "autentificacion.h"
#include "menu.h"
#include <time.h>
#include "sqlite3.h"

// Función para obtener la fecha y hora actual
void obtenerFechaHora(char *fechaHora) {
    time_t t;
    struct tm *tm_info;

    // Obtener la hora actual
    time(&t);
    tm_info = localtime(&t);

    // Formatear la fecha y hora
    strftime(fechaHora, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Función para registrar el log con fecha, hora y nombre de usuario
void registrarLog(const char *usuario) {
    FILE *logfile = fopen("../ficheros/logs.txt", "a");  // Abrir el archivo en modo append
    if (logfile == NULL) {
        printf("Error al abrir el archivo de logs.\n");
        return;
    }

    char fechaHora[20];
    obtenerFechaHora(fechaHora);  // Obtener la fecha y hora actual

    // Escribir la información de inicio de sesión en el archivo
    fprintf(logfile, "Usuario: %s, Inicio de sesión: %s\n", usuario, fechaHora);

    fclose(logfile);  // Cerrar el archivo
}


// Función que realiza todo el proceso de autenticación y retorna el tipo de usuario
TipoUsuario autentificarUsuario(const char *usuario, const char *contrasena) {
    FILE *archivo = fopen("../ficheros/usuarios.txt", "r");
    
    if (!archivo) {
        printf("Error al abrir el archivo de usuarios.\n");
        return TIPO_DESCONOCIDO;
    }

    char linea[MAX_LINE];
    char credencialUsuario[MAX_USER];
    char credencialContrasena[MAX_PASS];
    
    TipoUsuario tipo = TIPO_DESCONOCIDO;

    while (fgets(linea, MAX_LINE, archivo)) {
        if (strstr(linea, "Medicos:")) tipo = TIPO_MEDICO;
        if (strstr(linea, "Pacientes:")) tipo = TIPO_PACIENTE;
        if (strstr(linea, "Administracion:")) tipo = TIPO_ADMIN;

        if (sscanf(linea, "usuario: %s", credencialUsuario) == 1) {
            fgets(linea, MAX_LINE, archivo);
            sscanf(linea, "contrasena: %s", credencialContrasena);
            
            if (strcmp(usuario, credencialUsuario) == 0 && strcmp(contrasena, credencialContrasena) == 0) {
                fclose(archivo);
                return tipo;
            }
        }
    }

    fclose(archivo);
    return TIPO_DESCONOCIDO;
}

void autenticarYMostrarMensaje(const char *usuario, const char *contrasena, sqlite3 *db) {
    TipoUsuario tipo = autentificarUsuario(usuario, contrasena);
    
    // Si la autenticación es exitosa, registramos el log
    if (tipo != TIPO_DESCONOCIDO) {
        registrarLog(usuario);  // Registrar el log con la fecha, hora y usuario
    }

    switch (tipo) {
        case TIPO_MEDICO:
            printf("Autenticacion exitosa. Bienvenido, Dr. %s!\n", usuario);
            menuMedico(sqlite3 *db);
            break;
        case TIPO_PACIENTE:
            printf("Autenticacion exitosa. Bienvenido, paciente %s!\n", usuario);
            menuPaciente(sqlite3 *db);
            break;
        case TIPO_ADMIN:
            printf("Autenticacion exitosa. Bienvenido, administrador %s!\n", usuario);
            menuAdministracion(sqlite3 *db);
            break;
        default:
            printf("Usuario o contrasena incorrectos.\n");
            break;
    }
}



