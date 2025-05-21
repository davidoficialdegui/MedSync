#ifndef ADMIN_H
#define ADMIN_H


#include <sqlite3.h>

int admin_create(sqlite3 *db, const char *username, const char *password);
void ensure_default_admin(sqlite3 *db);
int admin_list(sqlite3 *db);

int admin_authenticate(sqlite3 *db, const char *username, const char *password);

#endif 
