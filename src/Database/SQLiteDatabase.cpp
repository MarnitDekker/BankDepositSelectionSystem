#include "SQLiteDatabase.h"
#pragma execution_character_set("utf-8")

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

std::vector<std::shared_ptr<Deposit>> SQLiteDatabase::getAllDeposits() {
    std::vector<std::shared_ptr<Deposit>> deposits;
    const char* sql = "SELECT d.id, d.name, d.interest_rate, d.term_months, "
        "d.min_amount, d.replenishable, d.withdrawable, "
        "d.capitalization, b.name, d.early_withdrawal_penalty, b.rating "
        "FROM deposits d JOIN banks b ON d.bank_id = b.id";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            bool earlyPenalty = sqlite3_column_int(stmt, 9) == 1;
            auto deposit = std::make_shared<Deposit>(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_double(stmt, 4),
                sqlite3_column_int(stmt, 5),
                sqlite3_column_int(stmt, 6),
                sqlite3_column_int(stmt, 7),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)),
                earlyPenalty
            );
            deposit->setBankRating(static_cast<int>(sqlite3_column_double(stmt, 10)));
            deposits.push_back(deposit);
        }
        sqlite3_finalize(stmt);
    }
    return deposits;
}

bool SQLiteDatabase::saveDepositScores(const std::vector<std::shared_ptr<Deposit>>& deposits) {
    if (!executeSQL("BEGIN TRANSACTION;")) return false;
    if (!executeSQL("DELETE FROM deposit_scores;")) {
        executeSQL("ROLLBACK;");
        return false;
    }
    const char* sql = "INSERT INTO deposit_scores (deposit_id, score) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        executeSQL("ROLLBACK;");
        return false;
    }
    for (const auto& deposit : deposits) {
        sqlite3_bind_int(stmt, 1, deposit->getId());
        sqlite3_bind_double(stmt, 2, deposit->getScore());
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            executeSQL("ROLLBACK;");
            return false;
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
    return executeSQL("COMMIT;");
}

static std::string escapeQuotes(const std::string& input) {
    std::string result;
    result.reserve(input.length() * 2);
    for (char c : input) {
        if (c == '\'') {
            result += "''";
        } else {
            result += c;
        }
    }
    return result;
}

bool SQLiteDatabase::addDeposit(const Deposit& deposit, int bankId) {
    std::string depositName = escapeQuotes(deposit.getName());

    const char* sql = "INSERT INTO deposits (bank_id, name, interest_rate, min_amount, term_months, replenishable, withdrawable, capitalization, early_withdrawal_penalty) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, bankId);
        sqlite3_bind_text(stmt, 2, depositName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, deposit.getInterestRate());
        sqlite3_bind_double(stmt, 4, deposit.getMinAmount());
        sqlite3_bind_int(stmt, 5, deposit.getTermMonths());
        sqlite3_bind_int(stmt, 6, deposit.isReplenishable() ? 1 : 0);
        sqlite3_bind_int(stmt, 7, deposit.isWithdrawable() ? 1 : 0);
        sqlite3_bind_int(stmt, 8, deposit.hasCapitalization() ? 1 : 0);
        sqlite3_bind_int(stmt, 9, deposit.isEarlyWithdrawalPenalized() ? 1 : 0);

        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        return result == SQLITE_DONE;
    }
    return false;
}

std::vector<std::pair<int, std::string>> SQLiteDatabase::getAllBanks() {
    std::vector<std::pair<int, std::string>> banks;
    const char* sql = "SELECT id, name FROM banks ORDER BY name;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            banks.emplace_back(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
            );
        }
        sqlite3_finalize(stmt);
    }
    return banks;
}

bool SQLiteDatabase::addBank(const std::string& name, const std::string& license, double rating) {
    const char* sql = "INSERT INTO banks (name, license, rating) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, license.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, rating);

        int result = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return result == SQLITE_DONE;
    }
    return false;
}

bool SQLiteDatabase::deleteBank(int bankId) {
    std::string sql = "DELETE FROM banks WHERE id = " + std::to_string(bankId) + ";";
    return executeSQL(sql);
}

bool SQLiteDatabase::deleteDeposit(int depositId) {
    std::string sql = "DELETE FROM deposits WHERE id = " + std::to_string(depositId) + ";";
    return executeSQL(sql);
}