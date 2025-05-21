#ifndef BD_H
#define BD_H

#include <sqlite3.h>


int bd_open(const char *filename, sqlite3 **db_out);

void bd_close(sqlite3 *db);

int bd_exec(sqlite3 *db, const char *sql);


int bd_query(sqlite3 *db, const char *sql,
             int (*callback)(void*,int,char**,char**),
             void *param);

#endif // BD_H