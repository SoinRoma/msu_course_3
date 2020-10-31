#include<iostream>
#include<cstdlib>
#include "sqlite3.h"
#pragma once

using namespace std;


void sqlExec(sqlite3* db, const char *sql, int(*callback)(void*, int, char**, char**), void *, char **permsg);

int callback(void *NotUsed, int argc, char **argv, char **azColName);

void error_msg(char* msg);

void ReadTableFomFile(sqlite3* db, const std::string& file_name, const std::string& table_name, const std::string& column_types);

void WriteTableToFile(sqlite3* db, const std::string& file_name, char** table, int nRows, int nCols);

void WriteSelectionToFile(sqlite3* db, const std::string& file_name, const std::string& select_query);

void TestTables();
  