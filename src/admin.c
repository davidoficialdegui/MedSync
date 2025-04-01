#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "admin.h"
#include "bd.h"

// metodos para paciente ----------------------------------
void gestionarPacientes()
{
    printf("\n--- Gestion de Pacientes ---\n");
    printf("1. Registrar nuevo paciente\n");
    printf("2. Buscar paciente\n");
    printf("3. Editar paciente\n");
    printf("4. Eliminar paciente\n");
    printf("5. Volver\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);
}

void registrarNuevoPaciente(sqlite3 *db)
{
    char sql[] = "INSERT INTO Paciente (Id_Paciente, DNI_P, Nombre_P, Email, Fecha_Ncto, Genero, Telefono_P, Direccion_P, Fecha_Reg) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Error preparando la consulta\n");
        return;
    }

    char id[20], dni[15], nombre[50], email[50], fecha_ncto[15], genero[10], direccion[100], fecha_reg[15];
    int telefono;

    printf("Ingrese ID del paciente: ");
    scanf("%s", id);
    printf("Ingrese DNI: ");
    scanf("%s", dni);
    printf("Ingrese Nombre: ");
    scanf("%s", nombre);
    printf("Ingrese Email: ");
    scanf("%s", email);
    printf("Ingrese Fecha de Nacimiento (YYYY-MM-DD): ");
    scanf("%s", fecha_ncto);
    printf("Ingrese Genero: ");
    scanf("%s", genero);
    printf("Ingrese Telefono: ");
    scanf("%d", &telefono);
    printf("Ingrese Direccion: ");
    scanf("%s", direccion);
    printf("Ingrese Fecha de Registro (YYYY-MM-DD): ");
    scanf("%s", fecha_reg);

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, fecha_ncto, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, genero, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, telefono);
    sqlite3_bind_text(stmt, 8, direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, fecha_reg, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error insertando paciente\n");
    }
    else
    {
        printf("Paciente registrado exitosamente\n");
    }

    sqlite3_finalize(stmt);
}

void BuscarPaciente(sqlite3 *db)
{
    char sql[] = "SELECT * FROM Paciente WHERE Id_Paciente = ?;";
    sqlite3_stmt *stmt;
    char id[20];

    printf("Ingrese ID del paciente a buscar: ");
    scanf("%s", id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Error preparando la consulta\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("Paciente encontrado:\n");
        printf("ID: %s, Nombre: %s, DNI: %s, Email: %s, Telefono: %d\n",
               sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 3),
               sqlite3_column_int(stmt, 6));
    }
    else
    {
        printf("Paciente no encontrado\n");
    }

    sqlite3_finalize(stmt);
}

void EditarPaciente(sqlite3 *db)
{
    char sql[] = "UPDATE Paciente SET Nombre_P = ?, Telefono_P = ? WHERE Id_Paciente = ?;";
    sqlite3_stmt *stmt;
    char id[20], nombre[50];
    int telefono;

    printf("Ingrese ID del paciente a editar: ");
    scanf("%s", id);
    printf("Ingrese nuevo Nombre: ");
    scanf("%s", nombre);
    printf("Ingrese nuevo Telefono: ");
    scanf("%d", &telefono);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Error preparando la consulta\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, telefono);
    sqlite3_bind_text(stmt, 3, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error actualizando paciente\n");
    }
    else
    {
        printf("Paciente actualizado exitosamente\n");
    }

    sqlite3_finalize(stmt);
}

void EliminaPaciente(sqlite3 *db)
{
    char sql[] = "DELETE FROM Paciente WHERE Id_Paciente = ?;";
    sqlite3_stmt *stmt;
    char id[20];

    printf("Ingrese ID del paciente a eliminar: ");
    scanf("%s", id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Error preparando la consulta\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error eliminando paciente\n");
    }
    else
    {
        printf("Paciente eliminado exitosamente\n");
    }

    sqlite3_finalize(stmt);
}

// metodos para empleado ------------------------------------------
void gestionarEmpleados()
{
    printf("\n--- Gestion de Empleados ---\n");
    printf("1. Registrar nuevo empleado\n");
    printf("2. Buscar empleado\n");
    printf("3. Editar empleado\n");
    printf("4. Eliminar empleado\n");
    printf("5. Volver\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);
}

void registrarNuevoEmpleado(sqlite3 *db)
{
    char id[20], nombre[50], dni[20], telefono[20], cargo[30];
    char sql[256];
    int result;

    printf("Ingrese ID del empleado: ");
    scanf("%s", id);
    printf("Ingrese nombre: ");
    scanf("%s", nombre);
    printf("Ingrese DNI: ");
    scanf("%s", dni);
    printf("Ingrese telefono: ");
    scanf("%s", telefono);
    printf("Ingrese cargo: ");
    scanf("%s", cargo);

    sprintf(sql, "INSERT INTO Empleado (Id_Empleado, Nombre_E, DNI_E, Telefono_E, Cargo) VALUES ('%s', '%s', '%s', '%s', '%s');", id, nombre, dni, telefono, cargo);

    result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK)
    {
        printf("Error al registrar empleado.\n");
    }
    else
    {
        printf("Empleado registrado exitosamente.\n");
    }
}

void BuscarEmpleado(sqlite3 *db)
{
    char id[20];
    char sql[256];
    sqlite3_stmt *stmt;
    int result;

    printf("Ingrese ID del empleado a buscar: ");
    scanf("%s", id);

    sprintf(sql, "SELECT * FROM Empleado WHERE Id_Empleado = '%s';", id);

    result = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (result == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            printf("ID: %s\n", sqlite3_column_text(stmt, 0));
            printf("Nombre: %s\n", sqlite3_column_text(stmt, 1));
            printf("DNI: %s\n", sqlite3_column_text(stmt, 2));
            printf("Telefono: %s\n", sqlite3_column_text(stmt, 3));
            printf("Cargo: %s\n", sqlite3_column_text(stmt, 4));
        }
        else
        {
            printf("Empleado no encontrado.\n");
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        printf("Error al buscar empleado.\n");
    }
}

void EditarEmpleado(sqlite3 *db)
{
    char id[20], nombre[50], dni[20], telefono[20], cargo[30];
    char sql[256];
    int result;

    printf("Ingrese ID del empleado a editar: ");
    scanf("%s", id);
    printf("Ingrese nuevo nombre: ");
    scanf("%s", nombre);
    printf("Ingrese nuevo DNI: ");
    scanf("%s", dni);
    printf("Ingrese nuevo telefono: ");
    scanf("%s", telefono);
    printf("Ingrese nuevo cargo: ");
    scanf("%s", cargo);

    sprintf(sql, "UPDATE Empleado SET Nombre_E='%s', DNI_E='%s', Telefono_E='%s', Cargo='%s' WHERE Id_Empleado='%s';", nombre, dni, telefono, cargo, id);

    result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK)
    {
        printf("Error al actualizar empleado.\n");
    }
    else
    {
        printf("Empleado actualizado exitosamente.\n");
    }
}

void EliminaEmpleado(sqlite3 *db)
{
    char id[20];
    char sql[256];
    int result;

    printf("Ingrese ID del empleado a eliminar: ");
    scanf("%s", id);

    sprintf(sql, "DELETE FROM Empleado WHERE Id_Empleado='%s';", id);

    result = sqlite3_exec(db, sql, 0, 0, 0);
    if (result != SQLITE_OK)
    {
        printf("Error al eliminar empleado.\n");
    }
    else
    {
        printf("Empleado eliminado exitosamente.\n");
    }
}

// metodos para reporte -------------------------------
void generarReportes()
{
    printf("\n--- Generar Reportes ---\n");
    printf("1. Reporte de pacientes registrados\n");
    printf("2. Reporte de empleados activos\n");
    printf("3. Reporte de citas programadas\n");
    printf("4. Volver\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);
}