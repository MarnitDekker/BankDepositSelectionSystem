#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include "include/Analysis/BasicDepositAnalyzer.h"
#include "include/Client.h"
#include "include/AppController.h"
#include "include/Factory.h"
#include <sqlite3.h>
//#include <windows.h>
#include <limits> 
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <io.h>

#pragma execution_character_set("utf-8")

Client createClient() {
    double amount = 0;
    int term = 0;
    bool replenishable = false, withdrawable = false;

    std::cout << "\n=== Ввод параметров вклада ===\n";

    while (true) {
        std::cout << "Введите сумму вклада (не менее 5000 руб): ";
        if (!(std::cin >> amount)) {
            std::cout << "Ошибка: введите числовое значение!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (amount < 5000) {
            std::cout << "Ошибка: минимальная сумма вклада - 5000 руб!\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Введите срок вклада (мес, 0 - любой срок): ";
        if (!(std::cin >> term)) {
            std::cout << "Ошибка: введите целое число!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (term < 0) {
            std::cout << "Ошибка: срок не может быть отрицательным!\n";
            continue;
        }
        break;
    }

    int input;
    while (true) {
        std::cout << "Требуется возможность пополнения? (1 - Да, 0 - Нет): ";
        if (!(std::cin >> input)) {
            std::cout << "Ошибка: введите 1 или 0!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (input != 0 && input != 1) {
            std::cout << "Ошибка: введите только 1 или 0!\n";
            continue;
        }
        replenishable = (input == 1);
        break;
    }

    while (true) {
        std::cout << "Требуется возможность частичного снятия? (1 - Да, 0 - Нет): ";
        if (!(std::cin >> input)) {
            std::cout << "Ошибка: введите 1 или 0!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (input != 0 && input != 1) {
            std::cout << "Ошибка: введите только 1 или 0!\n";
            continue;
        }
        withdrawable = (input == 1);
        break;
    }

    return Client(amount, term, replenishable, withdrawable);
}

void clientMenu(AppController& app) {
    while (true) {
        std::cout << "\n=== Система подбора банковских вкладов ===\n";
        std::cout << "1. Подобрать вклад\n";
        std::cout << "2. Просмотреть все вклады\n";
        std::cout << "3. Выйти в главное меню\n";
        std::cout << "Выберите действие: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Ошибка: введите число от 1 до 3!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            Client client = createClient();
            app.logUserQuery(client);
            app.processClientRequest(client);
            break;
        } 
        case 2: {
            app.showAllDeposits();
            break;
        } 
        case 3: {
            return;
        } 
        default:
            std::cout << "Неверный выбор. Введите число от 1 до 3.\n";
        }
    }
}

std::filesystem::path getProjectRoot() {
    std::filesystem::path path = std::filesystem::current_path();
    for (int i = 0; i < 5; ++i) {
        if (std::filesystem::exists(path / "CMakeLists.txt")) {
            return path;
        }
        path = path.parent_path();
    }

    return std::filesystem::current_path();
}

std::filesystem::path getDatabasePath() {
    std::filesystem::path dbPath = getProjectRoot() / "data" / "deposits.db";

    std::filesystem::create_directories(dbPath.parent_path());

    return dbPath;
}

void adminMenu(AppController& app) {
    const std::string adminPassword = "12345";
    std::string inputPassword;
    std::cout << "\nВведите пароль администратора: ";
    std::cin >> inputPassword;
    if (inputPassword != adminPassword) {
        std::cout << "Неверный пароль. Доступ запрещён.\n";
        return;
    }
    while (true) {
        std::cout << "\n=== Меню администратора ===\n";
        std::cout << "1. Добавить новый вклад\n";
        std::cout << "2. Удалить вклад\n";
        std::cout << "3. Просмотреть историю запросов пользователей\n";
        std::cout << "4. Добавить новый банк\n";
        std::cout << "5. Удалить банк\n";
        std::cout << "6. Выйти в главное меню\n";
        std::cout << "Выберите действие: " << std::flush;
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Ошибка: введите число от 1 до 6!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        switch (choice) {
        case 1: {
            app.handleAddDeposit();
            break;
        }
        case 2: {
            std::cout << "Введите ID вклада для удаления: ";
            int delId;
            std::cin >> delId;
            if (app.deleteDeposit(delId)) {
                std::cout << "Вклад успешно удалён.\n";
            }
            else {
                std::cout << "Ошибка при удалении вклада.\n";
            }
            break;
        }
        case 3: {
            app.showUserQueryHistory();
            break;
        } case 4: {
            app.handleAddBank();
            break;
        }
        case 5: {
            break;
        }
        case 6: {
            return;
        }
        default: {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
            break;
        }
        }
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::locale::global(std::locale("ru_RU.utf8"));

    try {
        auto db = std::make_unique<SQLiteDatabase>(getDatabasePath().string());
        if (!db->connect()) {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }
        AppController app(std::move(db), std::make_unique<BasicDepositAnalyzer>());
        while (true) {
            std::cout << "\n=== Добро пожаловать в систему подбора вкладов ===\n";
            std::cout << "1. Меню клиента\n";
            std::cout << "2. Меню администратора\n";
            std::cout << "3. Выйти\n";
            std::cout << "Выберите режим: " << std::flush;
            int mode;
            if (!(std::cin >> mode)) {
                std::cout << "Ошибка: введите число от 1 до 3!\n";
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                continue;
            }
            if (mode == 1) {
                clientMenu(app);
            } else if (mode == 2) {
                adminMenu(app);
            } else if (mode == 3) {
                std::filesystem::remove("user_queries.log");
                break;
            } else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}