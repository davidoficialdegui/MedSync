#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "medico.h"

void registrarMedico(sqlite3 *db) {
    medico nuevo;
    char sql[] = "INSERT INTO Medicos (id_medico, nombre, dni, telefono, especialidad) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    printf("\n--- Registrar Nuevo Médico ---\n");
    
    printf("ID Médico: ");
    scanf("%s", nuevo.Id_Medico);
    printf("Nombre: ");
    scanf(" [^\n]", nuevo.Nombre_M);
    printf("DNI: ");
    scanf("%s", nuevo.DNI_M);
    printf("Teléfono: ");
    scanf("%s", nuevo.Telefono_M);
    printf("Especialidad: ");
    scanf(" [^\n]", nuevo.Especialidad);

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nuevo.Id_Medico, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nuevo.Nombre_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nuevo.DNI_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, nuevo.Telefono_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, nuevo.Especialidad, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error insertando médico: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nMédico registrado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void buscarMedico(sqlite3 *db) {
    char id[50];
    char sql[] = "SELECT * FROM Medicos WHERE id_medico = ?;";
    sqlite3_stmt *stmt;

    printf("\n--- Buscar Médico ---\n");
    printf("Ingrese ID del médico: ");
    scanf("%s", id);

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        printf("\nMédico encontrado:\n");
        printf("ID: %s\n", sqlite3_column_text(stmt, 0));
        printf("Nombre: %s\n", sqlite3_column_text(stmt, 1));
        printf("DNI: %s\n", sqlite3_column_text(stmt, 2));
        printf("Teléfono: %s\n", sqlite3_column_text(stmt, 3));
        printf("Especialidad: %s\n", sqlite3_column_text(stmt, 4));
    } else {
        printf("\nNo se encontró médico con ID: %s\n", id);
    }

    sqlite3_finalize(stmt);
}

void editarMedico(sqlite3 *db) {
    char id[50];
    char sql_select[] = "SELECT * FROM Medicos WHERE id_medico = ?;";
    char sql_update[] = "UPDATE Medicos SET nombre = ?, dni = ?, telefono = ?, especialidad = ? WHERE id_medico = ?;";
    sqlite3_stmt *stmt;
    medico med;

    printf("\n--- Editar Médico ---\n");
    printf("Ingrese ID del médico a editar: ");
    scanf("%s", id);

    // Buscar médico existente
    if(sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW) {
        printf("\nMédico no encontrado\n");
        sqlite3_finalize(stmt);
        return;
    }

    // Mostrar y editar datos
    printf("\nDatos actuales:\n");
    printf("Nombre [%s]: ", sqlite3_column_text(stmt, 1));
    scanf(" [^\n]", med.Nombre_M);
    printf("DNI [%s]: ", sqlite3_column_text(stmt, 2));
    scanf("%s", med.DNI_M);
    printf("Teléfono [%s]: ", sqlite3_column_text(stmt, 3));
    scanf("%s", med.Telefono_M);
    printf("Especialidad [%s]: ", sqlite3_column_text(stmt, 4));
    scanf(" [^\n]", med.Especialidad);

    sqlite3_finalize(stmt);

    // Actualizar
    if(sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando actualización: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, med.Nombre_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, med.DNI_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, med.Telefono_M, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, med.Especialidad, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error actualizando médico: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nMédico actualizado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void eliminarMedico(sqlite3 *db) {
    char id[50];
    char sql[] = "DELETE FROM Medicos WHERE id_medico = ?;";
    sqlite3_stmt *stmt;
    char confirmacion;

    printf("\n--- Eliminar Médico ---\n");
    printf("Ingrese ID del médico a eliminar: ");
    scanf("%s", id);

    printf("\n¿Está seguro que desea eliminar este médico? (s/n): ");
    scanf(" %c", &confirmacion);

    if(confirmacion != 's' && confirmacion != 'S') {
        printf("Operación cancelada\n");
        return;
    }

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error eliminando médico: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nMédico eliminado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void listarMedicos(sqlite3 *db) {
    char sql[] = "SELECT * FROM Medicos ORDER BY nombre;";
    sqlite3_stmt *stmt;
    int rc;

    printf("\n--- Listado de Médicos ---\n");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n%-10s %-20s %-10s %-12s %-15s\n", 
           "ID", "Nombre", "DNI", "Teléfono", "Especialidad");
    printf("------------------------------------------------------------\n");

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%-10s %-20s %-10s %-12s %-15s\n",
               sqlite3_column_text(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 4));
    }

    if(rc != SQLITE_DONE) {
        fprintf(stderr, "Error leyendo datos: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void gestionarCitasMedico(sqlite3 *db, const char* id_medico) {
    int opcion;
    do {
        printf("\n--- Gestión de Citas ---\n");
        printf("1. Consultar mis citas programadas\n");
        printf("2. Cancelar cita\n");
        printf("3. Programar nueva cita\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1: {
                char sql[] = "SELECT c.id, p.nombre, c.fecha, c.hora, c.motivo FROM Citas c "
                             "JOIN Pacientes p ON c.id_paciente = p.id_paciente "
                             "WHERE c.id_medico = ? AND c.estado = 'programada' "
                             "ORDER BY c.fecha, c.hora;";
                sqlite3_stmt *stmt;
                
                if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
                    fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
                    break;
                }
                
                sqlite3_bind_text(stmt, 1, id_medico, -1, SQLITE_STATIC);
                
                printf("\n%-5s %-20s %-12s %-8s %s\n", "ID", "Paciente", "Fecha", "Hora", "Motivo");
                printf("--------------------------------------------------\n");
                
                while(sqlite3_step(stmt) == SQLITE_ROW) {
                    printf("%-5d %-20s %-12s %-8s %s\n",
                           sqlite3_column_int(stmt, 0),
                           sqlite3_column_text(stmt, 1),
                           sqlite3_column_text(stmt, 2),
                           sqlite3_column_text(stmt, 3),
                           sqlite3_column_text(stmt, 4));
                }
                
                sqlite3_finalize(stmt);
                break;
            }
            case 2: {
                int id_cita;
                char sql[] = "UPDATE Citas SET estado = 'cancelada' WHERE id = ? AND id_medico = ?;";
                sqlite3_stmt *stmt;
                
                printf("Ingrese ID de la cita a cancelar: ");
                scanf("%d", &id_cita);
                
                if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
                    fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
                    break;
                }
                
                sqlite3_bind_int(stmt, 1, id_cita);
                sqlite3_bind_text(stmt, 2, id_medico, -1, SQLITE_STATIC);
                
                if(sqlite3_step(stmt) != SQLITE_DONE) {
                    fprintf(stderr, "Error cancelando cita: %s\n", sqlite3_errmsg(db));
                } else {
                    printf("Cita cancelada con éxito\n");
                }
                
                sqlite3_finalize(stmt);
                break;
            }
            case 3: {
                char id_paciente[50], fecha[11], hora[6], motivo[100];
                char sql[] = "INSERT INTO Citas (id_paciente, id_medico, fecha, hora, motivo, estado) "
                             "VALUES (?, ?, ?, ?, ?, 'programada');";
                sqlite3_stmt *stmt;
                
                printf("ID del paciente: ");
                scanf("%s", id_paciente);
                printf("Fecha (YYYY-MM-DD): ");
                scanf("%s", fecha);
                printf("Hora (HH:MM): ");
                scanf("%s", hora);
                printf("Motivo: ");
                scanf(" [^\n]", motivo);
                
                if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
                    fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
                    break;
                }
                
                sqlite3_bind_text(stmt, 1, id_paciente, -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 2, id_medico, -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 3, fecha, -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 4, hora, -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 5, motivo, -1, SQLITE_STATIC);
                
                if(sqlite3_step(stmt) != SQLITE_DONE) {
                    fprintf(stderr, "Error programando cita: %s\n", sqlite3_errmsg(db));
                } else {
                    printf("Cita programada con éxito\n");
                }
                
                sqlite3_finalize(stmt);
                break;
            }
        }
    } while(opcion != 4);
}

void consultarHistorialPaciente(sqlite3 *db) {
    char dni[10];
    char sql[] = "SELECT p.nombre, h.diagnostico, h.tratamiento, h.fecha "
                 "FROM HistorialMedico h "
                 "JOIN Pacientes p ON h.id_paciente = p.id_paciente "
                 "WHERE p.dni = ? "
                 "ORDER BY h.fecha DESC;";
    sqlite3_stmt *stmt;

    printf("\n--- Consultar Historial Médico ---\n");
    printf("Ingrese DNI del paciente: ");
    scanf("%s", dni);

    if(strlen(dni) != 8) {
        printf("DNI debe tener 8 caracteres\n");
        return;
    }

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);

    printf("\n%-20s %-12s %-30s %s\n", "Paciente", "Fecha", "Diagnóstico", "Tratamiento");
    printf("--------------------------------------------------------------------\n");

    int tiene_registros = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        tiene_registros = 1;
        printf("%-20s %-12s %-30s %s\n",
               sqlite3_column_text(stmt, 0),
               sqlite3_column_text(stmt, 3),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2));
    }

    if(!tiene_registros) {
        printf("No se encontró historial para este paciente\n");
    }

    sqlite3_finalize(stmt);
}

void atenderCita(sqlite3 *db, const char* id_medico) {
    int id_cita;
    char sql_select[] = "SELECT c.id, p.nombre, c.fecha, c.hora, c.motivo "
                        "FROM Citas c "
                        "JOIN Pacientes p ON c.id_paciente = p.id_paciente "
                        "WHERE c.id = ? AND c.id_medico = ? AND c.estado = 'programada';";
    char sql_update[] = "UPDATE Citas SET estado = 'completada' WHERE id = ?;";
    char sql_insert[] = "INSERT INTO HistorialMedico (id_paciente, id_medico, fecha, diagnostico, tratamiento) "
                        "VALUES (?, ?, date('now'), ?, ?);";
    sqlite3_stmt *stmt;
    char diagnostico[256], tratamiento[256];
    char id_paciente[50];

    printf("\n--- Atender Cita ---\n");
    printf("Ingrese ID de la cita a atender: ");
    scanf("%d", &id_cita);

    
    if(sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id_cita);
    sqlite3_bind_text(stmt, 2, id_medico, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW) {
        printf("Cita no encontrada o ya atendida\n");
        sqlite3_finalize(stmt);
        return;
    }

    // Mostrar info
    printf("\nAtendiendo cita:\n");
    printf("Paciente: %s\n", sqlite3_column_text(stmt, 1));
    printf("Fecha: %s\n", sqlite3_column_text(stmt, 2));
    printf("Hora: %s\n", sqlite3_column_text(stmt, 3));
    printf("Motivo: %s\n", sqlite3_column_text(stmt, 4));

    strcpy(id_paciente, (const char*)sqlite3_column_text(stmt, 1));
    sqlite3_finalize(stmt);

    // Registrar diagnóstico y tratamiento
    printf("\nDiagnóstico: ");
    scanf(" [^\n]", diagnostico);
    printf("Tratamiento prescrito: ");
    scanf(" [^\n]", tratamiento);

    // Actualizar estado 
    if(sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando actualización: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id_cita);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error actualizando cita: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    // Registrar en historial médico
    if(sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando inserción: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id_paciente, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id_medico, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, diagnostico, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, tratamiento, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error registrando historial: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nAtención registrada con éxito en el historial médico\n");
    }

    sqlite3_finalize(stmt);
}