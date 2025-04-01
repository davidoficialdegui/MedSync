#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "medico.h"
#include "paciente.h"
#include "admin.h"
#include "sqlite3.h"

void menuMedico(sqlite3 *db) {
    int opcion;
    const char *id_medico;
    do {
        printf("\n--- Menu Medico ---\n");
        printf("1. Gestion de citas\n");
        printf("2. Historiales medicos\n");
        printf("3. Atender cita\n");
        printf("4. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
             printf("inserte el id del medico: ");
             scanf("%c", &id_medico);
                gestionarCitasMedico(db, id_medico);
                break;
            case 2:
                consultarHistorialPaciente(db);
                break;
            case 3:
                atenderCita(db, id_medico);
                break;
            case 4:
                printf("Cerrando sesión de médico...\n");
                break;
        }
    } while(opcion != 4);
}

void menuPaciente(sqlite3 *db) {
    int opcion;
    const char *id;

    while (1) {
        printf("\nMENU DE PACIENTE\n");
        printf("1. Gestion de Citas Medicas\n");
        printf("2. Consultar Historial Medico\n");
        printf("3. Atencion al Cliente\n");
        printf("4. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                gestionarCitas(db, id);
                break;
            case 2:
                printf("ingrese el id del paciente:");
                scanf("%c", &id);
                consultarHistorial(db, id);
                break;
            case 3:
                atencionCliente(db);
                break;
            case 4:
                printf("Saliendo...\n");
                exit(0);
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } 
}
    

void menuAdministracion(sqlite3 *db) {
    int opcion;
    do {
        printf("\n--- Menu Administrador ---\n");
        printf("1. Gestion de pacientes\n");
        printf("2. Gestion de empleados\n");
        printf("3. Generar reportes\n");
        printf("4. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
                gestionarPacientes(db);
                break;
            case 2:
                gestionarEmpleados(db);
                break;
            case 3:
                generarReportes(db);
                break;
            case 4:
                printf("Cerrando sesión de administrador...\n");
                break;
        }
    } while(opcion != 4);
}