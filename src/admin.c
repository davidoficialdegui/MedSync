#include "admin.h"
#include "bd.h"
#include <stdio.h>
#include <stdlib.h>

static int admin_list_cb(void *data, int cols, char **values, char **names) {
    for (int i = 0; i < cols; i++) {
        printf("%s = %s\t", names[i], values[i] ? values[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int admin_count_cb(void *data, int cols, char **values, char **names) {
    int *pcount = (int*)data;
    if (cols > 0 && values[0]) {
        *pcount = atoi(values[0]);
    }
    return 0;
}

int admin_create(sqlite3 *db, const char *username, const char *password) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO admin(username,password) VALUES('%s','%s');",
        username, password);
    return bd_exec(db, sql);
}

int admin_list(sqlite3 *db) {
    const char *sql = "SELECT id, username FROM admin;";
    return bd_query(db, sql, admin_list_cb, NULL);
}

int admin_authenticate(sqlite3 *db, const char *username, const char *password) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "SELECT COUNT(*) FROM admin "
        "WHERE username='%s' AND password='%s';",
        username, password);

    int count = 0;

    int rc = bd_query(db, sql, admin_count_cb, &count);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL en admin_authenticate: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return (count == 1) ? 1 : 0;
}