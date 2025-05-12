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

    const char* checkTables = "SELECT count(*) FROM sqlite_master;";
    sqlite3_stmt* stmt;
    bool isNewDB = false;

    if (sqlite3_prepare_v2(db, checkTables, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            isNewDB = true;
        }
        sqlite3_finalize(stmt);
    }

    executeSQL("PRAGMA encoding = 'UTF-8';");

    if (isNewDB) {
        return initializeDatabase();
    }

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

bool SQLiteDatabase::initializeDatabase() {
    const std::string createBanksTable =
        "CREATE TABLE IF NOT EXISTS banks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "license TEXT,"
        "rating REAL);";

    const std::string createDepositsTable =
        "CREATE TABLE IF NOT EXISTS deposits ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "bank_id INTEGER,"
        "name TEXT NOT NULL,"
        "interest_rate REAL NOT NULL,"
        "term_months INTEGER NOT NULL,"
        "min_amount REAL NOT NULL,"
        "replenishable INTEGER DEFAULT 0,"
        "withdrawable INTEGER DEFAULT 0,"
        "capitalization INTEGER DEFAULT 0,"
        "early_withdrawal_penalty INTEGER DEFAULT 0,"
        "FOREIGN KEY (bank_id) REFERENCES banks(id));";

    const std::string createScoresTable =
        "CREATE TABLE IF NOT EXISTS deposit_scores ("
        "deposit_id INTEGER PRIMARY KEY,"
        "score REAL NOT NULL,"
        "calculation_date TEXT DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (deposit_id) REFERENCES deposits(id));";

    if (!executeSQL(createBanksTable) ||
        !executeSQL(createDepositsTable) ||
        !executeSQL(createScoresTable)) {
        return false;
    }

    const char* checkData = "SELECT COUNT(*) FROM deposits;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkData, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            insertSampleData();
        }
        sqlite3_finalize(stmt);
    }

    return true;
}

void SQLiteDatabase::insertSampleData() {
    const char* checkData = "SELECT COUNT(*) FROM banks;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, checkData, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            const std::vector<std::string> insertQueries = {
                "INSERT INTO banks (name, license, rating) VALUES ('Сбербанк', '1234', 4.8);",
                "INSERT INTO banks (name, license, rating) VALUES ('ВТБ', '5678', 4.5);",
                "INSERT INTO banks (name, license, rating) VALUES ('Тинькофф', '9012', 4.3);",

                "INSERT INTO deposits (bank_id, name, interest_rate, term_months, min_amount, "
                "replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES "
                "(1, 'Пополняемый', 6.5, 12, 50000, 1, 0, 1, 1);",

                "INSERT INTO deposits (bank_id, name, interest_rate, term_months, min_amount, "
                "replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES "
                "(1, 'Классический', 7.0, 24, 10000, 1, 0, 0, 1);",

                "INSERT INTO deposits (bank_id, name, interest_rate, term_months, min_amount, "
                "replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES "
                "(2, 'Максимальный доход', 8.5, 36, 100000, 0, 0, 1, 1);",

                "INSERT INTO deposits (bank_id, name, interest_rate, term_months, min_amount, "
                "replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES "
                "(3, 'Универсальный', 7.2, 12, 30000, 1, 1, 0, 0);",

                "INSERT INTO deposits (bank_id, name, interest_rate, term_months, min_amount, "
                "replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES "
                "(3, 'Оптимальный', 7.8, 6, 50000, 0, 0, 1, 1);"
            };

            for (const auto& query : insertQueries) {
                executeSQL(query);
            }
        }
        sqlite3_finalize(stmt);
    }
}