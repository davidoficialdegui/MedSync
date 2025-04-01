#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "paciente.h"
#include <sqlite3.h>
#include <sqlite3_stmt>

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
// Función para conectar a la base de datos
sqlite3* conectarBD() {
    sqlite3 *db;
    if (sqlite3_open("clinica.db", &db)) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

// Función para obtener un ID de médico aleatorio
char* obtenerIdMedicoAleatorio() {
    sqlite3 *db = conectarBD();
    if (!db) return NULL;
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Id_Medico FROM Medico ORDER BY RANDOM() LIMIT 1";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al obtener ID de médico: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    
    char *id_medico = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id_medico = strdup((const char*)sqlite3_column_text(stmt, 0));
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return id_medico;
}
int idCitaExiste(const char *idCita) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int existe = 0; // 0 significa que no existe

    if (sqlite3_open("hospital.db", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos.\n");
        return 1; // Consideramos que existe en caso de error
    }

    const char *sql = "SELECT COUNT(*) FROM Cita_Medica WHERE Id_Cita = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, idCita, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            existe = sqlite3_column_int(stmt, 0) > 0;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return existe; // Retorna 1 si existe, 0 si no existe
}

char *generarIdCita() {
    static char id[10];
    do {
        sprintf(id, "CITA%04d", rand() % 10000);
    } while (idCitaExiste(id)); // Verificar que no se repita en la BD
    return id;
}

void registrarCita(const char *fecha, const char *motivo, const char *estado, const char *id_paciente) {
    char  idCita[20],*id_medico = obtenerIdMedicoAleatorio();
    if (!id_medico) {
        printf("No se pudo asignar un médico.\n");
        return;
    }
    generarIdCita(idCita);
    sqlite3 *db = conectarBD();
    if (!db) return;

    char sql[512];
    sprintf(sql, "INSERT INTO Cita_Medica (Id_Cita, Fecha_C, Motivo, Estado, Id_Paciente, Id_Medico) VALUES (NULL, '%s', '%s', '%s', '%s', '%s');", fecha, motivo, estado, id_paciente, id_medico);
    
    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al registrar la cita: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Cita registrada con éxito.\n");
    }
    
    sqlite3_close(db);
}

// Función para consultar citas
void consultarCitas() {
    sqlite3 *db = conectarBD();
    if (!db) return;
    
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Id_Cita, Fecha_C, Motivo, Estado FROM Cita_Medica";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al consultar citas: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    
    printf("\nCitas programadas:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %s, Fecha: %s, Motivo: %s, Estado: %s\n", 
            sqlite3_column_text(stmt, 0), 
            sqlite3_column_text(stmt, 1), 
            sqlite3_column_text(stmt, 2), 
            sqlite3_column_text(stmt, 3));
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Función para cancelar una cita
void cancelarCita(const char *id_cita) {
    sqlite3 *db = conectarBD();
    if (!db) return;

    char sql[256];
    sprintf(sql, "DELETE FROM Cita_Medica WHERE Id_Cita = '%s';", id_cita);
    
    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al cancelar la cita: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Cita cancelada con éxito.\n");
    }
    
    sqlite3_close(db);
}


void gestionarCitas(sqlite3 *db, const char *idPaciente) {
    int opcion;

    while (1) {
        printf("\nGESTION DE CITAS MEDICAS\n");
        printf("1. Coger cita\n");
        printf("2. Consultar citas programadas\n");
        printf("3. Cancelar una cita\n");
        printf("4. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                {
                    char idCita[20], idMedico[20], motivo[100], fecha[20], estado[20];
                    // Obtener un Id_Cita único
                    generarIdCita(db, idCita);

                    // Obtener un Id_Medico aleatorio
                    obtenerIdMedicoAleatorio(db, idMedico);
                    printf("\nCoger cita:\n");
                    printf("Motivo: ");
                    scanf("%s", motivo);
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
                    registrarCita(fecha, motivo, estado, idPaciente);
                }
                break;
            case 2:
                printf("\nConsultando citas programadas...\n");
                consultarCitas();
                break;

            case 3:
                printf("\nCancelando una cita...\n");
                printf("\nIngrese el ID de la cita a cancelar: ");
                char id_cita;
                scanf("%s", id_cita);
                cancelarCita(id_cita);
                break;
            case 4:
                return; // Salir de la gestión de citas
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    }
}

void consultarHistorial(const char *id_paciente) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int opcion;

    if (sqlite3_open("clinica.db", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return;
    }

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

        const char *sql = NULL;
        switch(opcion) {
            case 1:
            sql = "SELECT Cita_Medica.Fecha_C FROM Historial_Medico "
            "JOIN Cita_Medica ON Historial_Medico.Id_Cita = Cita_Medica.Id_Cita "
            "WHERE Historial_Medico.Id_Paciente = ?";
                break;
            case 2:
                printf("\nDiagnóstico: \n");
                sql = "SELECT Diagnostico FROM Historial_Medico WHERE Id_Paciente = ?";
                break;
            case 3:
                printf("\nTratamiento: \n");
                sql = "SELECT Tratamiento FROM Historial_Medico WHERE Id_Paciente = ?";
                break;
            case 4:
                printf("\nObservaciones: \n");
                sql = "SELECT Observaciones FROM Historial_Medico WHERE Id_Paciente = ?";
                break;
            case 5:
                printf("\nMedico:\n");
                sql = "SELECT Medico.Nombre FROM Historial_Medico "
                "JOIN Medico ON Historial_Medico.Id_Medico = Medico.Id_Medico "
                "WHERE Historial_Medico.Id_Paciente = ?";
                break;
            case 6:
                sqlite3_close(db);
                return; // Salir de la consulta del historial médico
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                continue;
        }
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id_paciente, -1, SQLITE_STATIC);
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                printf("%s\n", sqlite3_column_text(stmt, 0));
            }
        } else {
            printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
    }
}

// Función para generar un ID único para el reporte
char* generarIdReporte() {
    static char id[10];
    sprintf(id, "REP%04d", rand() % 10000); // Genera un ID con formato "REPxxxx"
    return id;
}

// Función para registrar un reporte en la base de datos
void registrarReporte(const char *descripcion, const char *fecha, const char *id_paciente, const char *id_empleado, const char *id_medico) {
    sqlite3 *db = conectarBD();
    if (!db) return;

    char sql[512];
    char *id_reporte = generarIdReporte();  // Generar un ID único para el reporte

    // Preparar la consulta SQL para insertar el reporte
    sprintf(sql, "INSERT INTO Reporte (Id_Reporte, Descripcion, Fecha_R, Id_Paciente, Id_Empleado, Id_Medico) "
                 "VALUES ('%s', '%s', '%s', '%s', '%s', '%s');",
            id_reporte, descripcion, fecha, id_paciente, id_empleado, id_medico);

    // Ejecutar la consulta SQL
    if (sqlite3_exec(db, sql, 0, 0, NULL) != SQLITE_OK) {
        printf("Error al registrar el reporte: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Reporte registrado con éxito.\n");
    }

    sqlite3_close(db);
}
 // Función para atender al cliente y registrar un reporte
void atencionCliente() {
    char descripcion[200];
    char fecha[20];
    char id_paciente[20];
    char id_empleado[20];
    char id_medico[20];

    // La descripción del problema
    printf("\nDescripción del problema: ");
    scanf("\n]%c", descripcion);  // Leer la descripción completa
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

    // Solicitar ID del paciente, empleado y médico
    printf("Ingrese el ID del paciente: ");
    scanf("%s", id_paciente);
    printf("Ingrese el ID del empleado: ");
    scanf("%s", id_empleado);
    printf("Ingrese el ID del médico: ");
    scanf("%s", id_medico);

    // Registrar el reporte en la base de datos
    registrarReporte(descripcion, fecha, id_paciente, id_empleado, id_medico);

    // Mostrar la información registrada
    printf("\nDescripción registrada: %s\n", descripcion);
    printf("Fecha de consulta registrada: %s\n", fecha);
    printf("ID del paciente: %s\n", id_paciente);
    printf("ID del empleado: %s\n", id_empleado);
    printf("ID del médico: %s\n", id_medico);
}