// src/main.c
#include <stdio.h>
#include <sqlite3.h>
#include "autentificacion.h"

int main(void) {
    sqlite3 *db;
    if (sqlite3_open_v2("MedSyncc.db", &db,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "No se pudo abrir MedSync.db: %s\n",
                sqlite3_errmsg(db));
        return 1;
    }
    printf("¡Base de datos abierta correctamente!\n\n");

    char usuario[MAX_USER], contrasena[MAX_PASS];

    // Intentar hasta que la autenticación sea exitosa
    while (1) {
        printf("Ingrese usuario: ");
        scanf("%49s", usuario);
        printf("Ingrese contrasena: ");
        scanf("%49s", contrasena);

        TipoUsuario tipo = autentificarUsuario(usuario, contrasena);
        if (tipo != TIPO_DESCONOCIDO) {
            autenticarYMostrarMensaje(usuario, contrasena, db);
            break;
        }
        printf("Intente de nuevo.\n\n");
    }

    sqlite3_close(db);
    return 0;
}
