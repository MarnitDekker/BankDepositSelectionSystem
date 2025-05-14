#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include "include/Analysis/BasicDepositAnalyzer.h"
#include "include/Client.h"
#include "include/AppController.h"
#include "include/Factory.h"
#include <sqlite3.h>
#include <windows.h>
#pragma execution_character_set("utf-8")

Client createClient() {
    double amount;
    int term;
    bool replenishable, withdrawable;

    std::cout << "\n=== Ввод параметров вклада ===\n";

    do {
        std::cout << "Введите сумму вклада (руб): ";
        std::cin >> amount;
        if (amount <= 0) {
            std::cout << "Ошибка: сумма должна быть положительной!\n";
        }
    } while (amount <= 0);

    std::cout << "Введите срок вклада (мес, 0 - любой срок): ";
    std::cin >> term;

    std::cout << "Требуется возможность пополнения? (1 - Да, 0 - Нет): ";
    std::cin >> replenishable;

    std::cout << "Требуется возможность частичного снятия? (1 - Да, 0 - Нет): ";
    std::cin >> withdrawable;

    return Client(amount, term, replenishable, withdrawable);
}

void clientMenu(AppController& app) {
    while (true) {
        std::cout << "\n=== Система подбора банковских вкладов ===\n";
        std::cout << "1. Подобрать вклад\n";
        std::cout << "2. Просмотреть все вклады\n";
        std::cout << "3. Выйти в главное меню\n";
        std::cout << "Выберите действие: " << std::flush;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            Client client = createClient();
            app.logUserQuery(client);
            /*app.processClientRequest(client);*/
        }
        else if (choice == 2) {
            app.showAllDeposits();
        }
        else if (choice == 3) {
            break;
        }
        else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        auto db = std::make_unique<SQLiteDatabase>("data/deposits.db");
        if (!db->connect()) {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }
        AppController app(std::move(db), std::make_unique<BasicDepositAnalyzer>());
        //app.setRecommendationStrategy(Factory::createStrategy(Factory::StrategyType::TOP_RATE));
        //app.setReportGenerator(Factory::createReportGenerator(Factory::ReportType::HTML));
        //app.setConsoleReportGenerator(Factory::createReportGenerator(Factory::ReportType::TEXT));
        while (true) {
            std::cout << "\n=== Добро пожаловать в систему подбора вкладов ===\n";
            std::cout << "1. Меню клиента\n";
            std::cout << "2. Меню администратора\n";
            std::cout << "3. Выйти\n";
            std::cout << "Выберите режим: " << std::flush;
            int mode;
            std::cin >> mode;
            if (mode == 1) {
                clientMenu(app);
            }
            else if (mode == 2) {
            }
            else if (mode == 3) {
                break;
            }
            else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
