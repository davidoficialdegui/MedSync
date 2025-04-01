#include <stdio.h>
#include <stdlib.h>
#include "autentificacion.h"
#include "sqlite3.h"


// main_menu.c
int main(int argc, char *argv[])
{

    sqlite3 *db;
    int rc;

    // Intentar abrir la base de datos (se crea si no existe)
    rc = sqlite3_open("hospital.db", &db);

    if (rc)
    {
        printf("No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1; // Salir con error
    }
    else
    {
        printf("Conexión a SQLite exitosa.\n");

        // Cerrar la base de datos al terminar
        sqlite3_close(db);

        char usuario[MAX_USER], contrasena[MAX_PASS];

        printf("Ingrese usuario: ");
        scanf("%s", usuario);

        printf("Ingrese contrasena: ");
        scanf("%s", contrasena);

        autenticarYMostrarMensaje(usuario, contrasena, db);
    }
}