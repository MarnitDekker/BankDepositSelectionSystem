#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include "include/Client.h"
#include <sqlite3.h>
#include <windows.h>
#pragma execution_character_set("utf-8")

//Client createClient() {
//    double amount;
//    int term;
//    bool replenishable, withdrawable;
//
//    std::cout << "\n=== Ввод параметров вклада ===\n";
//
//    do {
//        std::cout << "Введите сумму вклада (руб): ";
//        std::cin >> amount;
//        if (amount <= 0) {
//            std::cout << "Ошибка: сумма должна быть положительной!\n";
//        }
//    } while (amount <= 0);
//
//    std::cout << "Введите срок вклада (мес, 0 - любой срок): ";
//    std::cin >> term;
//
//    std::cout << "Требуется возможность пополнения? (1 - Да, 0 - Нет): ";
//    std::cin >> replenishable;
//
//    std::cout << "Требуется возможность частичного снятия? (1 - Да, 0 - Нет): ";
//    std::cin >> withdrawable;
//
//    return Client(amount, term, replenishable, withdrawable);
//}

//void clientMenu(AppController& app) {
//    while (true) {
//        std::cout << "\n=== Система подбора банковских вкладов ===\n";
//        std::cout << "1. Подобрать вклад\n";
//        std::cout << "2. Просмотреть все вклады\n";
//        std::cout << "3. Выйти в главное меню\n";
//        std::cout << "Выберите действие: " << std::flush;
//        int choice;
//        std::cin >> choice;
//        if (choice == 1) {
//            Client client = createClient();
//        }
//        else if (choice == 2) {
//        }
//        else if (choice == 3) {
//            break;
//        }
//        else {
//            std::cout << "Неверный выбор. Попробуйте снова.\n";
//        }
//    }
//}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

    try {
        auto db = std::make_unique<SQLiteDatabase>("data/deposits.db");
        if (!db->connect()) {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
