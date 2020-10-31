#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "CreateTables.h"




void sqlExec(sqlite3* db, const char *sql, int(*callback)(void*, int, char**, char**), void *, char **permsg)
{
    int rc;
    rc = sqlite3_exec(db, sql, callback, 0, permsg);
    if (rc)
    {
        sqlite3_close(db);
        error_msg(*permsg);
    }
}


int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void error_msg(char* msg)
{
    std::cout << msg << std::endl;
    exit(EXIT_FAILURE);
}

void ReadTableFomFile(sqlite3* db, const std::string& file_name, const std::string& table_name, const std::string& column_types)
{
    char* errmsg = nullptr;
    ifstream fin(file_name.c_str(), ios::in);
    std::string str;
    std::getline(fin, str);
    size_t columns_num = column_types.length();
    std::vector<std::string> record_columns(columns_num);
    std::string insert_query_common("insert into ");
    insert_query_common += (table_name + " values(");
    while (!fin.eof())
    {
        std::getline(fin, str);
        if (str.find_first_not_of(" \t") == string::npos)
            continue;
        size_t offset = 0;
        std::string insert_query_new;
        for(size_t i = 0; i < columns_num; i++)
        {
            size_t first_pos = str.find_first_not_of(" \t", offset);
            if (first_pos == string::npos)
            {
                fin.close();
                error_msg("Bad table file");
                return;
            }
            offset = str.find_first_of(" \t", first_pos);
            if (offset == string::npos)
                offset = str.length();
            record_columns[i] = str.substr(first_pos, offset - first_pos);
            if ((column_types[i] == 'i')||(column_types[i] == 'r'))
                insert_query_new = insert_query_new + record_columns[i] + ",";
            else
                insert_query_new = insert_query_new + "'" + record_columns[i] + "',";
        }
        if (!insert_query_new.empty())
            insert_query_new.back() = ')';
        insert_query_new.append(1,';');
        std::string insert_query = insert_query_common + insert_query_new;
        //std::cout << insert_query << std::endl;
        sqlExec(db, insert_query.c_str(), callback, 0, &errmsg);
    }
    fin.close();
}


void WriteTableToFile(sqlite3* db, const std::string& file_name, char** table, int nRows, int nCols)
{
    ofstream fout(file_name.c_str(), ios::out);
    for (int row = 0; row <= nRows; row++)
    {
        for (int col = 0; col < nCols; col++)
        {
            if (table[row * nCols + col])
                fout << table[row * nCols + col] << "   ";
            else
                fout << "NULL ";
        }
        fout << "\n";
    }
    fout.close();
}


void WriteSelectionToFile(sqlite3* db, const std::string& file_name, const std::string& select_query)
{
    char **table = nullptr;
    char* errmsg = nullptr;
    int nRows = 0, nCols = 0;
    ofstream fout(file_name.c_str(), ios::out);
    int rc = sqlite3_get_table(db, select_query.c_str(), &table, &nRows, &nCols, &errmsg);
    if (rc)
    {
        sqlite3_close(db);
        error_msg("Select operation unsuccessful");
    }
    WriteTableToFile(db, file_name, table, nRows, nCols);
    sqlite3_free_table(table);
}




void TestTables()
{
  sqlite3* db = nullptr;
  char **table = nullptr;
  char* errmsg = nullptr;
  int nRows = 0, nCols = 0;
  int rc = sqlite3_open("MiscData.db", &db);
  if(rc) 
  {
    sqlite3_close(db);
	error_msg("database is not opening");
  }
  //creating table Students (if it does not exist) 
  sqlExec(db, "drop table if exists Students", callback, 0, &errmsg);
  sqlExec(db, "create table Students(surname VARCHAR(30), name VARCHAR(30), patronymic VARCHAR(30), birth_date DATE, birth_place VARCHAR(30), height INT);", callback, 0, &errmsg);
  ReadTableFomFile(db, "Students.txt", "Students", "vvvdvi");
  
  //creating table Marks (if it does not exist) 
  sqlExec(db, "drop table if exists Marks", callback, 0, &errmsg);
  sqlExec(db, "create table Marks(surname VARCHAR(30), name VARCHAR(30), patronymic VARCHAR(30), average_mark REAL, state_exam_mark INT);", callback, 0, &errmsg);
  ReadTableFomFile(db, "Marks.txt", "Marks", "vvvri");
  
  //WriteSelectionToFile(db, "Selected Students.txt", "select distinct surname, average_mark from Marks");

  //avg
  //WriteSelectionToFile(db, "Selected Students.txt", "select avg(average_mark) from Marks");

  //Where
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, name, patronymic from Students where height > 175");

  //Подзапрос
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, name, patronymic from Marks where average_mark = (select max(average_mark) from Marks)");

  //Order by
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, name, patronymic, height from Students where (height > 175) order by height desc");

  //intersect
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, name, patronymic from Students where (height > 175) intersect select surname, name, patronymic, from Marks where (average_mark >= 4)");

  //group by
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, name, patronymic from Marks group by state_exam_mark");
  //WriteSelectionToFile(db, "Selected Students.txt", "select state_exam_mark, avg(average_mark) from Marks group by state_exam_mark");
  //WriteSelectionToFile(db, "Selected Students.txt", "select surname, max(height) from Students group by date(birth_date, 'start of year')");

  //having 
  //WriteSelectionToFile(db, "Selected Students.txt", "select state_exam_mark, avg(average_mark) from Marks group by state_exam_mark having state_exam_mark >= 4");
  //WriteSelectionToFile(db, "Selected Students.txt", "select state_exam_mark, avg(average_mark) from Marks where state_exam_mark >= 4 group by state_exam_mark");

  //join two tables
  //WriteSelectionToFile(db, "Selected Students.txt", "select * from Students, Marks");
  //WriteSelectionToFile(db, "Selected Students.txt", "select * from Students cross join Marks");

  //WriteSelectionToFile(db, "Selected Students.txt", "select * from Students, Marks where Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  //WriteSelectionToFile(db, "Selected Students.txt", "select * from Students inner join Marks on Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  //WriteSelectionToFile(db, "Selected Students.txt", "select Students.surname, Students.name, Students.patronymic, birth_date, birth_place, height, average_mark, state_exam_mark from Students inner join Marks on Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  //WriteSelectionToFile(db, "Selected Students.txt", "select Students.surname, Students.name, Students.patronymic, birth_date, birth_place, height, average_mark, state_exam_mark from Students where Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  //WriteSelectionToFile(db, "Selected Students.txt", "select Students.surname, Students.name, Students.patronymic, birth_date, birth_place, height, average_mark, state_exam_mark from Students inner join Marks on Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  
  //left join
  WriteSelectionToFile(db, "Selected Students.txt", "select Students.surname, Students.name, Students.patronymic, birth_date, birth_place, height, average_mark, state_exam_mark from Students left join Marks on Students.surname = Marks.surname and Students.name = Marks.name and Students.patronymic = Marks.patronymic");
  sqlite3_close(db);
}





