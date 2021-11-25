#pragma once
#include "sqlite3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

 int createDB(const char* s)
{
	sqlite3* DB;

	int exit = 0;
	exit = sqlite3_open(s, &DB);

	sqlite3_close(DB);

	return 0;
}

 int createTable(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS CODES("
		"FIRST TEXT PRIMARY KEY , "
		"SECOND      TEXT NOT NULL, "
		"THIRD    TEXT NOT NULL, "
		"FOURTH     TEXT  NOT NULL, "
		"FIFTH   TEXT, "
		"SIXTH     TEXT );";
	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}

 int insertData(const char* s, vector<vector<string>>& csv)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	for (int j = 0; j < 170;j++) {
		vector<string> row = csv[j];
		string sql = "INSERT INTO CODES (FIRST,SECOND,THIRD,FOURTH,FIFTH,SIXTH) VALUES(";
		for (int i = 0; i < 6;i++) {
			sql = sql + "'" + row[i] + "',";
		}
		sql.pop_back();
		sql = sql + ");";
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in insertData function." << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Records "<<j+1<<" inserted Successfully!" << endl;
	}

	vector<string> row = csv[170];
	string sql = "INSERT INTO CODES (FIRST,SECOND,THIRD,FOURTH) VALUES(";
	for (int i = 0; i < 4; i++) {
		sql = sql + "'" + row[i] + "',";
	}
	sql.pop_back();
	sql = sql + ");";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in insertData function." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Records 171 inserted Successfully!" << endl;
	return 0;
}

 int updateData(const char* s)
{
	sqlite3* DB;
	char* messageError;

	string sql("DELETE FROM CODES WHERE SUBSTRING(SECOND,1,1) = '1' OR SUBSTRING(SECOND,1,1) = '0' OR SUBSTRING(SECOND,1,1) = '2' OR SUBSTRING(SECOND,1,1) = '3' OR SUBSTRING(SECOND,1,1) = '4' OR SUBSTRING(SECOND,1,1) = '5' OR SUBSTRING(SECOND,1,1) = '6' OR SUBSTRING(SECOND,1,1) = '7' OR SUBSTRING(SECOND,1,1) = '8' OR SUBSTRING(SECOND,1,1) = '9'");

	sqlite3_open(s, &DB);
	sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

	return 0;
}
