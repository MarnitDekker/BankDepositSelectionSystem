#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include <sqlite3.h>
#include <windows.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	auto db = std::make_unique<SQLiteDatabase>("data/deposits.db");
	if (!db->connect()) {
		throw std::runtime_error("Не удалось подключиться к базе данных");
	}
	std::cout << "." << std::endl;
	return 0;
}
