#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "empleado.h"

void registrarNuevoEmpleado(sqlite3 *db) {
    empleado nuevo;
    char sql[] = "INSERT INTO Empleados (id_empleado, nombre, dni, telefono, cargo) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    printf("\n--- Registrar Nuevo Empleado ---\n");
    
    printf("ID Empleado: ");
    scanf("%s", nuevo.Id_Empleado);
    printf("Nombre: ");
    scanf(" [^\n]", nuevo.Nombre_E);
    printf("DNI: ");
    scanf("%s", nuevo.DNI_E);
    printf("Teléfono: ");
    scanf("%s", nuevo.Telefono_E);
    printf("Cargo: ");
    scanf(" [^\n]", nuevo.Cargo);

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nuevo.Id_Empleado, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nuevo.Nombre_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nuevo.DNI_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, nuevo.Telefono_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, nuevo.Cargo, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error insertando empleado: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nEmpleado registrado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void buscarEmpleado(sqlite3 *db) {
    char id[50];
    char sql[] = "SELECT * FROM Empleados WHERE id_empleado = ?;";
    sqlite3_stmt *stmt;

    printf("\n--- Buscar Empleado ---\n");
    printf("Ingrese ID del empleado: ");
    scanf("%s", id);

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        printf("\nEmpleado encontrado:\n");
        printf("ID: %s\n", sqlite3_column_text(stmt, 0));
        printf("Nombre: %s\n", sqlite3_column_text(stmt, 1));
        printf("DNI: %s\n", sqlite3_column_text(stmt, 2));
        printf("Teléfono: %s\n", sqlite3_column_text(stmt, 3));
        printf("Cargo: %s\n", sqlite3_column_text(stmt, 4));
    } else {
        printf("\nNo se encontró empleado con ID: %s\n", id);
    }

    sqlite3_finalize(stmt);
}

void editarEmpleado(sqlite3 *db) {
    char id[50];
    char sql_select[] = "SELECT * FROM Empleados WHERE id_empleado = ?;";
    char sql_update[] = "UPDATE Empleados SET nombre = ?, dni = ?, telefono = ?, cargo = ? WHERE id_empleado = ?;";
    sqlite3_stmt *stmt;
    empleado emp;

    printf("\n--- Editar Empleado ---\n");
    printf("Ingrese ID del empleado a editar: ");
    scanf("%s", id);

    // Primero buscamos el empleado
    if(sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_ROW) {
        printf("\nEmpleado no encontrado\n");
        sqlite3_finalize(stmt);
        return;
    }

    // Mostramos datos actuales
    printf("\nDatos actuales:\n");
    printf("Nombre [%s]: ", sqlite3_column_text(stmt, 1));
    scanf(" [^\n]", emp.Nombre_E);
    printf("DNI [%s]: ", sqlite3_column_text(stmt, 2));
    scanf("%s", emp.DNI_E);
    printf("Teléfono [%s]: ", sqlite3_column_text(stmt, 3));
    scanf("%s", emp.Telefono_E);
    printf("Cargo [%s]: ", sqlite3_column_text(stmt, 4));
    scanf(" [^\n]", emp.Cargo);

    sqlite3_finalize(stmt);

    // Actualizamos
    if(sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando actualización: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, emp.Nombre_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, emp.DNI_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, emp.Telefono_E, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, emp.Cargo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, id, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error actualizando empleado: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nEmpleado actualizado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void eliminarEmpleado(sqlite3 *db) {
    char id[50];
    char sql[] = "DELETE FROM Empleados WHERE id_empleado = ?;";
    sqlite3_stmt *stmt;
    char confirmacion;

    printf("\n--- Eliminar Empleado ---\n");
    printf("Ingrese ID del empleado a eliminar: ");
    scanf("%s", id);

    printf("\n¿Está seguro que desea eliminar este empleado? (s/n): ");
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
        fprintf(stderr, "Error eliminando empleado: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nEmpleado eliminado con éxito\n");
    }

    sqlite3_finalize(stmt);
}

void listarEmpleados(sqlite3 *db) {
    char sql[] = "SELECT * FROM Empleados ORDER BY nombre;";
    sqlite3_stmt *stmt;
    int rc;

    printf("\n--- Listado de Empleados ---\n");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n%-10s %-20s %-10s %-12s %-15s\n", 
           "ID", "Nombre", "DNI", "Teléfono", "Cargo");
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