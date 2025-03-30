#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "paciente.h"

// Función para validar si una cadena representa una fecha en formato dd/mm/yyyy
int esFechaValida(const char *fecha) {
    int dia, mes, anio;

    // Validar que la fecha tenga el formato correcto (dd/mm/yyyy)
    if (sscanf(fecha, "%2d/%2d/%4d", &dia, &mes, &anio) != 3) {
        return 0; // Formato incorrecto
    }

    // Verificar que el día, mes y año sean válidos
    if (mes < 1 || mes > 12) {
        return 0; // Mes fuera de rango
    }

    // Días válidos por mes
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Comprobar si es un año bisiesto
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
        diasPorMes[1] = 29; // Febrero en año bisiesto tiene 29 días
    }

    // Verificar que el día esté dentro del rango válido para el mes dado
    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        return 0; // Día fuera de rango para ese mes
    }

    return 1; // Fecha válida
}


// Función para obtener la fecha actual del sistema en formato dd/mm/yyyy
void obtenerFechaActual(int *dia, int *mes, int *anio) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    *dia = tm.tm_mday;
    *mes = tm.tm_mon + 1;  // Los meses en `tm` empiezan desde 0
    *anio = tm.tm_year + 1900;  // Los años en `tm` se cuentan desde 1900
}

int esFechaFutura(int diaCita, int mesCita, int anioCita) {
    int diaHoy, mesHoy, anioHoy;
    obtenerFechaActual(&diaHoy, &mesHoy, &anioHoy);

    // Comparar la fecha de la cita con la fecha actual
    if (anioCita > anioHoy) {
        return 1; // La cita está en el futuro
    } else if (anioCita == anioHoy) {
        if (mesCita > mesHoy) {
            return 1; // La cita está en el futuro
        } else if (mesCita == mesHoy) {
            if (diaCita > diaHoy) {
                return 1; // La cita está en el futuro
            }
        }
    }

    return 0; // La cita es en el pasado o en el presente
}

void gestionarCitas() {
    int opcion;

    while (1) {
        printf("\nGESTION DE CITAS MEDICAS\n");
        printf("1. Coger cita\n");
        printf("2. Consultar citas programadas\n");
        printf("3. Modificar una cita\n");
        printf("4. Cancelar una cita\n");
        printf("5. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                {
                    char motivo[100], fecha[20], estado[20];
                    printf("\nCoger cita:\n");
                    printf("Motivo: ");
                    scanf("%s", motivo);
                        // Validación de la fecha
                     // Validación de la fecha
                    while (1) {
                        printf("Fecha (formato: dd/mm/yyyy): ");
                        scanf("%s", fecha);

                        // Validar si la fecha es correcta y futura
                        int dia, mes, anio;
                        if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) == 3 && esFechaValida(fecha) && esFechaFutura(dia, mes, anio)) {
                            break; // Si la fecha es válida y futura, salimos del ciclo
                        } else {
                            printf("Fecha inválida o pasada. Por favor, ingresa una fecha válida en el futuro (dd/mm/yyyy).\n");
                        }
                    }
                    printf("Estado: ");
                    scanf("%s", estado);
                    printf("\nCita programada con motivo '%s' para la fecha '%s' con estado '%s'.\n", motivo, fecha, estado);
                }
                break;
            case 2:
                printf("\nConsultando citas programadas...\n");
                // Aquí iría la lógica para mostrar citas programadas
                break;
            case 3:
                printf("\nModificando una cita...\n");
                // Aquí iría la lógica para modificar una cita
                break;
            case 4:
                printf("\nCancelando una cita...\n");
                // Aquí iría la lógica para cancelar una cita
                break;
            case 5:
                return; // Salir de la gestión de citas
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    }
}

void consultarHistorial() {
    int opcion;

    while (1) {
        printf("\nCONSULTAR HISTORIAL MEDICO\n");
        printf("1. Fecha cita\n");
        printf("2. Diagnóstico\n");
        printf("3. Tratamiento\n");
        printf("4. Observaciones\n");
        printf("5. Medico\n");
        printf("6. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                printf("\nFecha de la cita: \n");
                break;
            case 2:
                printf("\nDiagnóstico: \n");
                break;
            case 3:
                printf("\nTratamiento: \n");
                break;
            case 4:
                printf("\nObservaciones: \n");
                break;
            case 5:
                printf("\nMedico:\n");
                break;
            case 6:
                return; // Salir de la consulta del historial médico
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    }
}

    void atencionCliente() {
        char descripcion[200];
        char fecha[20];
    
        // la descripción del problema
        printf("\nDescripcion del problema: ");
        getchar();  // Limpiar el buffer de entrada
        fgets(descripcion, sizeof(descripcion), stdin);  // Leer la descripción completa
    
        // Validación de la fecha
         // Validación de la fecha
        while (1) {
            printf("Fecha de la consulta (formato: dd/mm/yyyy): ");
            scanf("%s", fecha);

            // Validar si la fecha es correcta y futura
            int dia, mes, anio;
            if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) == 3 && esFechaValida(fecha) && esFechaFutura(dia, mes, anio)) {
                break; // Si la fecha es válida y futura, salimos del ciclo
            } else {
                printf("Fecha inválida o pasada. Por favor, ingresa una fecha válida en el futuro (dd/mm/yyyy).\n");
            }
        }
    
        // Mostrar la información ingresada
        printf("\nDescripcion registrada: %s", descripcion);
        printf("Fecha de consulta registrada: %s\n", fecha);
    }