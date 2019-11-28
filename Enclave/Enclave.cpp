#include "Enclave_t.h"
#include "sqlite3.h"
#include <string>

static sqlite3* db;  // Database connection object

// SQLite callback function for printing results
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i < argc; i++){
    std::string azColName_str = azColName[i];
    std::string argv_str = (argv[i] ? argv[i] : "NULL");
    ocall_print_string((azColName_str + " = " + argv_str + "\n").c_str());
  }
  ocall_print_string("\n");
  return 0;
}

void ecall_opendb() {
  int rc = sqlite3_open(":memory:", &db);  // Opening In-Memory database
  if (rc) {
    ocall_println_string("[SQLite3 Error] - can't open database connection: ");
    ocall_println_string(sqlite3_errmsg(db));
    return;
  }
  ocall_print_string("[SQLite3 Info] - Created database connection to :memory:");
}

void ecall_execute_sql(const char *sql) {
  int rc;
  char *zErrMsg = NULL;
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc) {
    ocall_print_string("[SQLite3 error] - ");
    ocall_println_string(sqlite3_errmsg(db));
    return;
  }
}

void ecall_closedb() {
  sqlite3_close(db);
  ocall_println_string("[SQLite3 Info] - Closed database connection");
}
