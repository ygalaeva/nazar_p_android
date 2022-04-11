//
//  TestFile.c
//  TestRequests
//
//  Created by Nazar on 28.03.2022.
//

#include "DatabaseHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 * openSQLiteDB(const char * path) {
    printf("sqlite3_libversion: %s\n", sqlite3_libversion());

    sqlite3 * database;

    if (sqlite3_open(path, &database) == SQLITE_OK) {
        return database;
    }

    printf("sqlite3_open != SQLITE_OK\n");
    return NULL;
}

void ensureTableExistsSQLiteDB(sqlite3 * database) {
    // using AUTOINCREMENT limits to max 9223372036854775807 records
    const char * sql = "CREATE TABLE IF NOT EXISTS requests(id INTEGER PRIMARY KEY, subject TEXT NOT NULL, payload TEXT NOT NULL)";
    sqlite3_stmt * statement;
    int sqlite3PrepareResult = sqlite3_prepare(database, sql, -1, &statement, NULL);
    if (sqlite3PrepareResult != SQLITE_OK) {
        printf("%s, sqlite3_prepare result %d\n", __func__, sqlite3PrepareResult);
    }
    int sqlite3StepResult = sqlite3_step(statement);
    if (sqlite3StepResult != SQLITE_DONE) {
        printf("%s, sqlite3_step result %d\n", __func__, sqlite3StepResult);
    }
    int sqlite3FinalizeResult = sqlite3_finalize(statement);
    if (sqlite3FinalizeResult != SQLITE_OK) {
        printf("sqlite3_finalize result %d\n", sqlite3FinalizeResult);
    }
}

void closeSQLiteDB(struct sqlite3 * database) {
    if (sqlite3_close(database) != SQLITE_OK) {
        printf("sqlite3_close != SQLITE_OK\n");
    }
}

sqlite3_int64 * insertSQLiteDB(sqlite3 * database, const char * subject, const char * payload) {
    char * template = "INSERT INTO requests VALUES(NULL, '%s', '%s')\0";
    unsigned long len = strlen(template) + strlen(subject) + strlen(payload);
    char sql[len];
    sprintf(sql, template, subject, payload);
    sqlite3_stmt * statement;
    int sqlite3PrepareResult = sqlite3_prepare(database, sql, -1, &statement, NULL);
    if (sqlite3PrepareResult != SQLITE_OK) {
        printf("%s, sqlite3_prepare result %d\n", __func__, sqlite3PrepareResult);
        return NULL;
    }
    int sqlite3StepResult = sqlite3_step(statement);
    if (sqlite3StepResult != SQLITE_DONE) {
        printf("sqlite3_step result %d\n", sqlite3StepResult);
        return NULL;
    }
    int sqlite3FinalizeResult = sqlite3_finalize(statement);
    if (sqlite3FinalizeResult != SQLITE_OK) {
        printf("sqlite3_finalize result %d\n", sqlite3FinalizeResult);
        return NULL;
    }

    sqlite3_int64 identity = sqlite3_last_insert_rowid(database);
    sqlite3_int64 * result = malloc(sizeof(int));
    result = &identity;
    return result;
}

struct EventObject * selectNextEventSQLiteDB(sqlite3 * database) {
    char * sql = "SELECT id, subject, payload FROM requests ORDER BY id ASC LIMIT 1";

    sqlite3_stmt * statement;
    int sqlite3PrepareResult = sqlite3_prepare(database, sql, -1, &statement, NULL);
    if (sqlite3PrepareResult != SQLITE_OK) {
        printf("%s, sqlite3_prepare result %d\n", __func__, sqlite3PrepareResult);
        return NULL;
    }

    struct EventObject * result = NULL;
    if (sqlite3_step(statement) == SQLITE_ROW) {
        int id = sqlite3_column_int(statement, 0);
        const unsigned char * subject = sqlite3_column_text(statement, 1);
        const unsigned char * payload = sqlite3_column_text(statement, 2);

        result = malloc(sizeof(struct EventObject));
        result->id = id;

        result->subject = malloc(sizeof(const unsigned char));
//        memcpy(result->subject, (const char *)subject, strlen((const char *)subject));
        strcpy(result->subject, (const char *)subject);

        result->payload = malloc(sizeof(const unsigned char));
        strcpy(result->payload, (const char *)payload);
    }

    int sqlite3FinalizeResult = sqlite3_finalize(statement);
    if (sqlite3FinalizeResult != SQLITE_OK) printf("sqlite3_finalize result %d\n", sqlite3FinalizeResult);
    
    return result;
}

void deleteSQLiteDB(sqlite3 * database, sqlite3_int64 id) {
    char * template = "DELETE FROM requests WHERE id=%d";
    unsigned long len = strlen(template) + strlen("9223372036854775807");
    char sql[len];
    sprintf(sql, template, id);

    sqlite3_stmt * statement;
    int sqlite3PrepareResult = sqlite3_prepare(database, sql, -1, &statement, NULL);
    if (sqlite3PrepareResult != SQLITE_OK) {
        printf("%s, sqlite3_prepare result %d\n", __func__, sqlite3PrepareResult);
        return;
    }

    int sqlite3StepResult = sqlite3_step(statement);
    if (sqlite3StepResult != SQLITE_DONE) {
        printf("sqlite3_step result %d\n", sqlite3StepResult);
    }

    int sqlite3FinalizeResult = sqlite3_finalize(statement);
    if (sqlite3FinalizeResult != SQLITE_OK) {
        printf("sqlite3_finalize result %d\n", sqlite3FinalizeResult);
    }
}
