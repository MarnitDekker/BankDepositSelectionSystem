#include "SQLiteDatabase.h"
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <filesystem>

SQLiteDatabase::SQLiteDatabase(const std::string& dbPath)
    : dbPath(dbPath), db(nullptr) {
}

SQLiteDatabase::~SQLiteDatabase() {
    disconnect();
}

bool SQLiteDatabase::connect() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Ошибка подключения к БД: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    executeSQL("PRAGMA encoding = 'UTF-8';");
    return true;
}

void SQLiteDatabase::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool SQLiteDatabase::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}