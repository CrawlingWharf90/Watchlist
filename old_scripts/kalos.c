#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "sqlite3.c"

int main() {
    sqlite3 *db;
    char *errMsg = 0;

    // Open or create a database file
    if (sqlite3_open("movies.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // SQL statement for inserting a movie
    const char *insertSql = "INSERT INTO Movies (Title, Director, ReleaseYear) VALUES (?, ?, ?)";
    
    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, insertSql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Bind values to the parameters in the SQL statement
    const char *title = "Example Movie";
    const char *director = "John Doe";
    int releaseYear = 2023;

    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, director, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, releaseYear);

    // Execute the SQL statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}
