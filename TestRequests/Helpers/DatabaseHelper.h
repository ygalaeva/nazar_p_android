//
//  TestFile.h
//  TestRequests
//
//  Created by Nazar on 28.03.2022.
//

#ifndef TestFile_h
#define TestFile_h

#include <stdio.h>
#include <sqlite3.h>

struct EventObject {
    sqlite3_int64 id;
    char * subject;
    char * payload;
};

sqlite3 * openSQLiteDB(const char * path);

void ensureTableExistsSQLiteDB(sqlite3 * database);

void closeSQLiteDB(sqlite3 * database);

sqlite3_int64 * insertSQLiteDB(sqlite3 * database, const char * subject, const char * payload);

struct EventObject * selectNextEventSQLiteDB(sqlite3 * database);

void deleteSQLiteDB(sqlite3 * database, sqlite3_int64 id);

#endif /* TestFile_h */
