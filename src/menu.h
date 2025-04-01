#include "sqlite3.h"
#ifndef MENU_H
#define MENU_H

///habrá que hacer includes pero de momento suficiente

void menuMedico(sqlite3 *db);
void menuPaciente(sqlite3 *db);
void menuAdministracion(sqlite3 *db);


#endif