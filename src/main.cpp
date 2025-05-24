/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "paciente.h"
#include "medico.h"
#include "reportes.h"




#define STRBUF_LEN 128

char *readline(const char *prompt, char *buf, size_t size) {
    if (prompt) fputs(prompt, stdout);
    if (!fgets(buf, size, stdin)) return NULL;
    buf[strcspn(buf, "\n")] = '\0';
    return buf;
}

static int generalLlamadaDeVuelta(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        printf("%s: %s\t", names[i], values[i] ? values[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int init_database(sqlite3 *db) {
    const char *sql =
        "PRAGMA foreign_keys = ON;"
        // Pacientes
        "CREATE TABLE IF NOT EXISTS paciente ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nombre TEXT NOT NULL,"
        "  apellidos TEXT NOT NULL,"
        "  fecha_nac DATE NOT NULL,"
        "  genero TEXT CHECK(genero IN ('M','F','O')) NOT NULL,"
        "  telefono TEXT,"
        "  email TEXT"
        ");"
        // Médicos
        "CREATE TABLE IF NOT EXISTS medico ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nombre TEXT NOT NULL,"
        "  especialidad TEXT NOT NULL,"
        "  telefono TEXT,"
        "  email TEXT"
        ");"
        // Usuarios
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  password TEXT NOT NULL,"
        "  role TEXT CHECK(role IN ('admin','medico','paciente')) NOT NULL"
        ");"
        // Citas
        "CREATE TABLE IF NOT EXISTS cita ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  medico_id INTEGER NOT NULL,"
        "  fecha_hora DATETIME NOT NULL,"
        "  motivo TEXT,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE,"
        "  FOREIGN KEY(medico_id) REFERENCES medico(id) ON DELETE CASCADE"
        ");"
        // Historial Médico
        "CREATE TABLE IF NOT EXISTS historial_medico ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  fecha DATETIME NOT NULL,"
        "  descripcion TEXT NOT NULL,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE"
        ");"
        // Reportes de Pacientes
        "CREATE TABLE IF NOT EXISTS reportes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  fecha DATETIME NOT NULL DEFAULT (datetime('now')) ,"
        "  problema TEXT NOT NULL,"
        "  estado TEXT CHECK(estado IN ('abierto','cerrado')) NOT NULL DEFAULT 'abierto',"
        "  respuesta TEXT,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE"
        ");";
    return bd_exec(db, sql);
}

static void predeterminado(sqlite3 *db) {
    bd_exec(db,
      "INSERT OR IGNORE INTO usuarios(username,password,role) VALUES('admin','admin','admin');"
    );
}

static void leer(const char *prompt, char *buf, size_t len) {
    printf("%s", prompt);
    if (!fgets(buf, (int)len, stdin)) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\r\n")] = '\0';
}

static void menu_paciente(sqlite3 *db, int user_id) {
    int choice;
    char buf[STRBUF_LEN];
    do {
        printf("\n--- MENÚ PACIENTE ---\n");
        printf("1) Ver mis citas\n2) Ver historial médico\n");
        printf("3) Crear reporte\n4) Ver mis reportes\n");
        printf("0) Salir\nElige: ");
        scanf("%d%*c", &choice);
        switch (choice) {
            case 1: {
                char sql[256];
                snprintf(sql, sizeof(sql),
                    "SELECT id, fecha_hora, motivo FROM cita WHERE paciente_id=%d;",
                    user_id);
                bd_query(db, sql, generalLlamadaDeVuelta, NULL);
            } break;
            case 2:
                reporte_historial_paciente(db, user_id);
                break;
            case 3:
                readline("Describe tu problema: ", buf, sizeof(buf));
                reporte_usuario_create(db, user_id, buf);
                break;
            case 4:
                reporte_usuario_list(db, user_id);
                break;
        }
    } while (choice != 0);
}

static void menu_medico(sqlite3 *db) {
    int choice;
    char buf1[STRBUF_LEN], buf2[STRBUF_LEN];
    do {
        printf("\n--- MENÚ MÉDICO ---\n");
        printf("1) Crear cita\n2) Ver historial médico\n");
        printf("3) Reporte: Citas x Médico\n4) Reporte: Historial Paciente\n");
        printf("5) Ver reportes de pacientes\n0) Salir\nElige: ");
        scanf("%d%*c", &choice);
        int pid, mid;
        switch (choice) {
            case 1:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                printf("ID Médico: "); scanf("%d%*c", &mid);
                readline("Fecha y hora: ", buf1, sizeof(buf1));
                readline("Motivo: ", buf2, sizeof(buf2));
                cita_create(db, pid, mid, buf1, buf2);
                break;
            case 2:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                historial_list(db, pid);
                break;
            case 3:
                reporte_citas_por_medico(db);
                break;
            case 4:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                reporte_historial_paciente(db, pid);
                break;
            case 5:
                reporte_usuario_list_all(db);
                break;
        }
    } while (choice != 0);
}


static void menu_admin(sqlite3 *db) {
    int choice;
    char buf1[STRBUF_LEN], buf2[STRBUF_LEN], buf3[STRBUF_LEN];
    do {
        printf("\n--- MENÚ ADMIN ---\n");
        printf("1) Listar pacientes\n2) Crear paciente\n");
        printf("3) Listar médicos\n4) Crear médico\n");
        printf("5) Listar citas\n6) Crear cita\n");
        printf("7) Listar historial médico\n8) Crear historial médico\n");
        printf("9) Reporte: Citas x Médico\n10) Reporte: Historial Paciente\n");
        printf("11) Ver reportes de pacientes\n0) Salir\nElige: ");
        scanf("%d%*c", &choice);
        int pid, mid;
        switch (choice) {
            case 1:
                paciente_list(db);
                break;
            case 2:
                readline("Nombre: ", buf1, sizeof(buf1));
                readline("Apellidos: ", buf2, sizeof(buf2));
                readline("Fecha (YYYY-MM-DD): ", buf3, sizeof(buf3));
                paciente_create(db, buf1, buf2, buf3, "O", "", "");
                break;
            case 3:
                medico_list(db);
                break;
            case 4:
                readline("Nombre: ", buf1, sizeof(buf1));
                readline("Especialidad: ", buf2, sizeof(buf2));
                medico_create(db, buf1, buf2, "", "");
                break;
            case 5:
                cita_list(db);
                break;
            case 6:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                printf("ID Médico: "); scanf("%d%*c", &mid);
                readline("Fecha y hora: ", buf1, sizeof(buf1));
                readline("Motivo: ", buf2, sizeof(buf2));
                cita_create(db, pid, mid, buf1, buf2);
                break;
            case 7:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                historial_list(db, pid);
                break;
            case 8:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                readline("Fecha: ", buf1, sizeof(buf1));
                readline("Descripción: ", buf2, sizeof(buf2));
                historial_create(db, pid, buf1, buf2);
                break;
            case 9:
                reporte_citas_por_medico(db);
                break;
            case 10:
                printf("ID Paciente: "); scanf("%d%*c", &pid);
                reporte_historial_paciente(db, pid);
                break;
            case 11:
                reporte_usuario_list_all(db);
                break;
        }
    } while (choice != 0);
}

int main(void) {
    sqlite3 *db;
    char user[STRBUF_LEN], pass[STRBUF_LEN];
    Role role;
    int user_id = 0;

    if (bd_open("bd/MedSync.db", &db) != 0) {
        fprintf(stderr, "Error al abrir la BD.\n");
        return 1;
    }
    if (init_database(db) != SQLITE_OK) {
        fprintf(stderr, "Error al inicializar tablas.\n");
        bd_close(db);
        return 1;
    }
    ensure_default_admin(db);

    do {
        readline("Usuario: ", user, sizeof(user));
        readline("Contraseña: ", pass, sizeof(pass));
        role = autenticar(db, user, pass);
        if (role == ROLE_UNKNOWN) printf("Credenciales inválidas.\n");
    } while (role == ROLE_UNKNOWN);

    if (role == ROLE_PACIENTE) {
        printf("Ingresa tu ID de paciente: ");
        scanf("%d%*c", &user_id);
        menu_paciente(db, user_id);
    } else if (role == ROLE_MEDICO) {
        menu_medico(db);
    } else if (role == ROLE_ADMIN) {
        menu_admin(db);
    }

    bd_close(db);
    return 0;
}*/

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "paciente.h"
#include "medico.h"
#include "reportes.h"

using namespace std;

const int STRBUF_LEN = 128;

string readline(const string &prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

static int generalLlamadaDeVuelta(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        cout << names[i] << ": " << (values[i] ? values[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

static int init_database(sqlite3 *db) {
    const char *sql =
        "PRAGMA foreign_keys = ON;"
        "CREATE TABLE IF NOT EXISTS paciente ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nombre TEXT NOT NULL,"
        "  apellidos TEXT NOT NULL,"
        "  fecha_nac DATE NOT NULL,"
        "  genero TEXT CHECK(genero IN ('M','F','O')) NOT NULL,"
        "  telefono TEXT,"
        "  email TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS medico ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  nombre TEXT NOT NULL,"
        "  especialidad TEXT NOT NULL,"
        "  telefono TEXT,"
        "  email TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  password TEXT NOT NULL,"
        "  role TEXT CHECK(role IN ('admin','medico','paciente')) NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS cita ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  medico_id INTEGER NOT NULL,"
        "  fecha_hora DATETIME NOT NULL,"
        "  motivo TEXT,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE,"
        "  FOREIGN KEY(medico_id) REFERENCES medico(id) ON DELETE CASCADE"
        ");"
        "CREATE TABLE IF NOT EXISTS historial_medico ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  fecha DATETIME NOT NULL,"
        "  descripcion TEXT NOT NULL,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE"
        ");"
        "CREATE TABLE IF NOT EXISTS reportes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  paciente_id INTEGER NOT NULL,"
        "  fecha DATETIME NOT NULL DEFAULT (datetime('now')),"
        "  problema TEXT NOT NULL,"
        "  estado TEXT CHECK(estado IN ('abierto','cerrado')) NOT NULL DEFAULT 'abierto',"
        "  respuesta TEXT,"
        "  FOREIGN KEY(paciente_id) REFERENCES paciente(id) ON DELETE CASCADE"
        ");";
    return bd_exec(db, sql);
}

static void predeterminado(sqlite3 *db) {
    bd_exec(db, "INSERT OR IGNORE INTO usuarios(username,password,role) VALUES('admin','admin','admin');");
}

static void menu_paciente(sqlite3 *db, int user_id) {
    int choice;
    do {
        cout << "\n--- MENÚ PACIENTE ---\n";
        cout << "1) Ver mis citas\n2) Ver historial médico\n";
        cout << "3) Crear reporte\n4) Ver mis reportes\n";
        cout << "0) Salir\nElige: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                char sql[256];
                snprintf(sql, sizeof(sql), "SELECT id, fecha_hora, motivo FROM cita WHERE paciente_id=%d;", user_id);
                bd_query(db, sql, generalLlamadaDeVuelta, nullptr);
                break;
            }
            case 2:
                reporte_historial_paciente(db, user_id);
                break;
            case 3: {
                string problema = readline("Describe tu problema: ");
                reporte_usuario_create(db, user_id, problema.c_str());
                break;
            }
            case 4:
                reporte_usuario_list(db, user_id);
                break;
        }
    } while (choice != 0);
}

static void menu_medico(sqlite3 *db) {
    int choice;
    do {
        cout << "\n--- MENÚ MÉDICO ---\n";
        cout << "1) Crear cita\n2) Ver historial médico\n";
        cout << "3) Reporte: Citas x Médico\n4) Reporte: Historial Paciente\n";
        cout << "5) Ver reportes de pacientes\n0) Salir\nElige: ";
        cin >> choice;
        cin.ignore();

        int pid, mid;
        switch (choice) {
            case 1: {
                cout << "ID Paciente: "; cin >> pid;
                cout << "ID Médico: "; cin >> mid;
                cin.ignore();
                string fecha = readline("Fecha y hora: ");
                string motivo = readline("Motivo: ");
                cita_create(db, pid, mid, fecha.c_str(), motivo.c_str());
                break;
            }
            case 2:
                cout << "ID Paciente: "; cin >> pid;
                cin.ignore();
                historial_list(db, pid);
                break;
            case 3:
                reporte_citas_por_medico(db);
                break;
            case 4:
                cout << "ID Paciente: "; cin >> pid;
                cin.ignore();
                reporte_historial_paciente(db, pid);
                break;
            case 5:
                reporte_usuario_list_all(db);
                break;
        }
    } while (choice != 0);
}

static void menu_admin(sqlite3 *db) {
    int choice;
    do {
        cout << "\n--- MENÚ ADMIN ---\n";
        cout << "1) Listar pacientes\n2) Crear paciente\n";
        cout << "3) Listar médicos\n4) Crear médico\n";
        cout << "5) Listar citas\n6) Crear cita\n";
        cout << "7) Listar historial médico\n8) Crear historial médico\n";
        cout << "9) Reporte: Citas x Médico\n10) Reporte: Historial Paciente\n";
        cout << "11) Ver reportes de pacientes\n0) Salir\nElige: ";
        cin >> choice;
        cin.ignore();

        int pid, mid;
        switch (choice) {
            case 1:
                paciente_list(db);
                break;
            case 2: {
                string nombre = readline("Nombre: ");
                string apellidos = readline("Apellidos: ");
                string fecha = readline("Fecha (YYYY-MM-DD): ");
                paciente_create(db, nombre.c_str(), apellidos.c_str(), fecha.c_str(), "O", "", "");
                break;
            }
            case 3:
                medico_list(db);
                break;
            case 4: {
                string nombre = readline("Nombre: ");
                string especialidad = readline("Especialidad: ");
                medico_create(db, nombre.c_str(), especialidad.c_str(), "", "");
                break;
            }
            case 5:
                cita_list(db);
                break;
            case 6: {
                cout << "ID Paciente: "; cin >> pid;
                cout << "ID Médico: "; cin >> mid;
                cin.ignore();
                string fecha = readline("Fecha y hora: ");
                string motivo = readline("Motivo: ");
                cita_create(db, pid, mid, fecha.c_str(), motivo.c_str());
                break;
            }
            case 7:
                cout << "ID Paciente: "; cin >> pid;
                cin.ignore();
                historial_list(db, pid);
                break;
            case 8: {
                cout << "ID Paciente: "; cin >> pid;
                cin.ignore();
                string fecha = readline("Fecha: ");
                string descripcion = readline("Descripción: ");
                historial_create(db, pid, fecha.c_str(), descripcion.c_str());
                break;
            }
            case 9:
                reporte_citas_por_medico(db);
                break;
            case 10:
                cout << "ID Paciente: "; cin >> pid;
                cin.ignore();
                reporte_historial_paciente(db, pid);
                break;
            case 11:
                reporte_usuario_list_all(db);
                break;
        }
    } while (choice != 0);
}

int main() {
    sqlite3 *db;
    Role role;
    int user_id = 0;

    if (bd_open("bd/MedSync.db", &db) {
        cerr << "Error al abrir la BD." << endl;
        return 1;
    }
    if (init_database(db) {
        cerr << "Error al inicializar tablas." << endl;
        bd_close(db);
        return 1;
    }
    ensure_default_admin(db);

    do {
        string user = readline("Usuario: ");
        string pass = readline("Contraseña: ");
        role = autenticar(db, user.c_str(), pass.c_str());
        if (role == ROLE_UNKNOWN) cout << "Credenciales inválidas." << endl;
    } while (role == ROLE_UNKNOWN);

    if (role == ROLE_PACIENTE) {
        cout << "Ingresa tu ID de paciente: ";
        cin >> user_id;
        cin.ignore();
        menu_paciente(db, user_id);
    } else if (role == ROLE_MEDICO) {
        menu_medico(db);
    } else if (role == ROLE_ADMIN) {
        menu_admin(db);
    }

    bd_close(db);
    return 0;
}
