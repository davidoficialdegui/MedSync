#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h" 
#include "admin.h"
#include "empleado.h"

// metodos para paciente ----------------------------------
void registrarNuevoPaciente(sqlite3 *db)
{
   char sql[] = "INSERT INTO Paciente (Id_Paciente, DNI_P, Nombre_P, Email, Fecha_Ncto, Genero, Telefono_P, Direccion_P, Fecha_Reg, Usuario, Contrasena) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        printf("Error preparando la consulta\n");
        return;
    }

    char id[20], dni[15], nombre[50], email[50], fecha_ncto[15], genero[10], direccion[100], fecha_reg[15], usuario[50], contrasena[50];;
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
    printf("Ingrese nombre de usuario: ");
    scanf("%s", usuario);
    printf("Ingrese contraseña: ");
    scanf("%s", contrasena);

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, fecha_ncto, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, genero, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, telefono);
    sqlite3_bind_text(stmt, 8, direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, fecha_reg, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, contrasena, -1, SQLITE_STATIC);

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
        printf("ID: %s, Nombre: %s, DNI: %s, Email: %s, Telefono: %d""Usuario: %s, Contraseña: %s\n",
               sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 3),
               sqlite3_column_int(stmt, 6), sqlite3_column_text(stmt, 9),
               sqlite3_column_text(stmt, 10));
        
    }
    else
    {
        printf("Paciente no encontrado\n");
    }

    sqlite3_finalize(stmt);
}

void EditarPaciente(sqlite3 *db)
{
    char sql[] = "UPDATE Paciente SET DNI_P = ?, Nombre_P = ?, Email = ?, Fecha_Ncto = ?, Genero = ?, Telefono_P = ?, Direccion_P = ?, Fecha_Reg = ?, Usuario = ?, Contrasena = ? WHERE Id_Paciente = ?;";
    sqlite3_stmt *stmt;

    char id[20], dni[15], nombre[50], email[50], fecha_ncto[15], genero[10], direccion[100], fecha_reg[15], usuario[50], contrasena[50];
    int telefono;

    printf("Ingrese ID del paciente a editar: ");
    scanf("%s", id);
    printf("Nuevo DNI: ");
    scanf("%s", dni);
    printf("Nuevo Nombre: ");
    scanf("%s", nombre);
    printf("Nuevo Email: ");
    scanf("%s", email);
    printf("Nueva Fecha de Nacimiento (YYYY-MM-DD): ");
    scanf("%s", fecha_ncto);
    printf("Nuevo Género: ");
    scanf("%s", genero);
    printf("Nuevo Teléfono: ");
    scanf("%d", &telefono);
    printf("Nueva Dirección: ");
    scanf("%s", direccion);
    printf("Nueva Fecha de Registro (YYYY-MM-DD): ");
    scanf("%s", fecha_reg);
    printf("Nuevo Usuario: ");
    scanf("%s", usuario);
    printf("Nueva Contraseña: ");
    scanf("%s", contrasena);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, fecha_ncto, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, genero, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, telefono);
    sqlite3_bind_text(stmt, 7, direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, fecha_reg, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, contrasena, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error actualizando paciente: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Paciente actualizado exitosamente.\n");
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

void gestionarPacientes(sqlite3 *db)
{
    int opcion;

    do
    {
        printf("\n--- Gestion de Pacientes ---\n");
        printf("1. Registrar nuevo paciente\n");
        printf("2. Buscar paciente\n");
        printf("3. Editar paciente\n");
        printf("4. Eliminar paciente\n");
        printf("5. Volver\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            registrarNuevoPaciente(db);
            break;
        case 2:
            BuscarPaciente(db);
            break;
        case 3:
            EditarPaciente(db);
            break;
        case 4:
            EliminaPaciente(db);
            break;
        case 5:
            return;
        }
    } while (opcion != 5);
}

// metodos para empleado ------------------------------------------

/* void registrarNuevoEmpleado(sqlite3 *db)
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
} */

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
            printf("Usuario: %s\n", sqlite3_column_text(stmt, 5));
        printf("Contraseña: %s\n", sqlite3_column_text(stmt, 6));
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
    char id[20], nombre[50], dni[20], telefono[20], cargo[30], usuario[50], contrasena[50];
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
    printf("Ingrese nuevo usuario: ");
    scanf("%s", usuario);
    printf("Ingrese nueva contrasena: ");
    scanf("%s", contrasena);

    sprintf(sql, "UPDATE Empleado SET Nombre_E='%s', DNI_E='%s', Telefono_E='%s', Cargo='%s', Usuario='%s', Contrasena='%s' WHERE Id_Empleado='%s';",
        nombre, dni, telefono, cargo, usuario, contrasena, id);

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

void gestionarEmpleados(sqlite3 *db)
{
    int opcion;

    do
    {

        printf("\n--- Gestion de Empleados ---\n");
        printf("1. Registrar nuevo empleado\n");
        printf("2. Buscar empleado\n");
        printf("3. Editar empleado\n");
        printf("4. Eliminar empleado\n");
        printf("5. Volver\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            registrarNuevoEmpleado(db);
            break;
        case 2:
            BuscarEmpleado(db);
            break;
        case 3:
            EditarEmpleado(db);
            break;
        case 4:
            EliminaEmpleado(db);
            break;
        case 5:
            return;
        }

    } while (opcion != 5);
}

// metodos para reporte -------------------------------

void ReportePacientes(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Id_Reporte, Descripcion, Fecha_R FROM Reporte WHERE Id_Paciente IS NOT NULL";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        printf("\nReportes realizados por Pacientes:\n");
        printf("ID Reporte | Descripción | Fecha Reporte\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%s | %s | %s\n",
                   sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1),
                   sqlite3_column_text(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
}

void ReporteEmpleados(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Id_Reporte, Descripcion, Fecha_R FROM Reporte WHERE Id_Empleado IS NOT NULL";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        printf("\nReportes realizados por Empleados:\n");
        printf("ID Reporte | Descripción | Fecha Reporte\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%s | %s | %s\n",
                   sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1),
                   sqlite3_column_text(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
}

void ReporteMedicos(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT Id_Reporte, Descripcion, Fecha_R FROM Reporte WHERE Id_Medico IS NOT NULL";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        printf("\nReportes realizados por Médicos:\n");
        printf("ID Reporte | Descripción | Fecha Reporte\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%s | %s | %s\n",
                   sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1),
                   sqlite3_column_text(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
}  

void generarReportes(sqlite3 *db)
{
    printf("\n--- Generar Reportes ---\n");
    printf("1. Mostrar reportes de pacientes\n");
    printf("2. Mostrar reportes de empleados\n");
    printf("3. Mostrar reportes de medicos\n");
    printf("4. Volver\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);
}