// src/autentificacion.c
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "autentificacion.h"
#include "menu.h"

// Lee ficheros/usuarios.txt y devuelve el tipo si encuentra credenciales válidas.
TipoUsuario autentificarUsuario(const char *usuario, const char *contrasena) {
    FILE *fp = fopen("ficheros/usuarios.txt", "r");
    if (!fp) {
        fprintf(stderr, "No se pudo abrir ficheros/usuarios.txt: %s\n", strerror(errno));
        return TIPO_DESCONOCIDO;
    }

    char linea[MAX_LINE];
    char file_user[MAX_USER], file_pass[MAX_PASS];
    TipoUsuario current = TIPO_DESCONOCIDO;

    while (fgets(linea, sizeof(linea), fp)) {
        // Detectamos la sección
        if (strstr(linea, "Medicos:"))       current = TIPO_MEDICO;
        else if (strstr(linea, "Pacientes:")) current = TIPO_PACIENTE;
        else if (strstr(linea, "Administracion:")) current = TIPO_ADMIN;

        // Si es línea de "usuario:", leemos también la siguiente de "contrasena:"
        else if (sscanf(linea, "usuario: %49s", file_user) == 1) {
            if (fgets(linea, sizeof(linea), fp) &&
                sscanf(linea, "contrasena: %49s", file_pass) == 1)
            {
                if (strcmp(usuario, file_user) == 0 &&
                    strcmp(contrasena, file_pass) == 0)
                {
                    fclose(fp);
                    return current;
                }
            }
        }
    }

    fclose(fp);
    return TIPO_DESCONOCIDO;
}

void autenticarYMostrarMensaje(const char *usuario,
                               const char *contrasena,
                               sqlite3 *db)
{
    TipoUsuario tipo = autentificarUsuario(usuario, contrasena);
    if (tipo == TIPO_DESCONOCIDO) {
        printf("Usuario o contraseña incorrectos.\n");
        return;
    }

    // Mostrar mensaje y registrar en log si quieres...
    switch (tipo) {
        case TIPO_MEDICO:
            printf("¡Bienvenido Dr. %s!\n", usuario);
            menuMedico(db);
            break;
        case TIPO_PACIENTE:
            printf("¡Bienvenido paciente %s!\n", usuario);
            menuPaciente(db);
            break;
        case TIPO_ADMIN:
            printf("¡Bienvenido administrador %s!\n", usuario);
            menuAdministracion(db);
            break;
        default:
            break;
    }
}
