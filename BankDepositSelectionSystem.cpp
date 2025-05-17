#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include "include/Analysis/BasicDepositAnalyzer.h"
#include "include/Client.h"
#include "include/AppController.h"
#include "include/Factory.h"
#include <sqlite3.h>
#include <windows.h>
#include <filesystem>
#include <fcntl.h>
#include <io.h>
#include <limits> 
#include <fstream>
#include <filesystem>
//#include <string>

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
//            continue;
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

//#include <iostream>
//#include <limits> // для std::numeric_limits

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
        std::cout << "Выберите действие: " << std::flush;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            Client client = createClient();
            app.logUserQuery(client);
            app.processClientRequest(client);
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

std::string cp1251_to_utf8(const std::string& cp1251str) {
    int wchars_num = MultiByteToWideChar(1251, 0, cp1251str.c_str(), -1, NULL, 0);
    std::wstring wstr(wchars_num, 0);
    MultiByteToWideChar(1251, 0, cp1251str.c_str(), -1, &wstr[0], wchars_num);

    int utf8_num = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8str(utf8_num, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], utf8_num, NULL, NULL);

    if (!utf8str.empty() && utf8str.back() == '\0') utf8str.pop_back();
    return utf8str;
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
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "Список банков:\n";
            auto banks = app.getAllBanks();
            std::vector<std::pair<int, std::string>> nonEmptyBanks;
            int idx = 1;
            for (const auto& bank : banks) {
                if (!bank.second.empty()) {
                    std::cout << idx << ". " << bank.second << std::endl;
                    nonEmptyBanks.push_back(bank);
                    ++idx;
                }
            }
            if (nonEmptyBanks.empty()) {
                std::cout << "Нет доступных банков!\n";
                continue;
            }
            int bankChoice = 0;
            std::cout << "Выберите номер банка из списка: ";
            std::cin >> bankChoice;
            if (bankChoice < 1 || bankChoice >(int)nonEmptyBanks.size()) {
                std::cout << "Некорректный выбор банка!\n";
                continue;
            }
            int bankId = nonEmptyBanks[bankChoice - 1].first;
            std::string bankName = nonEmptyBanks[bankChoice - 1].second;
            std::cout << "Введите параметры нового вклада:\n";
            int id = 0;
            double rate, minAmount;
            int term;
            bool replenishable, withdrawable, capitalization, earlyWithdrawal;

            std::filesystem::path dirPath = getProjectRoot() / "data";
            std::filesystem::path filePath = dirPath / "nameDeposit.txt";

            std::string tempName;
            std::cout << "Введите название вклада: ";
            std::cin.ignore();
            std::getline(std::cin, tempName);

            std::string utf8Name = cp1251_to_utf8(tempName);

            std::ofstream fout(filePath.string());
            if (fout.is_open()) {
                fout << utf8Name;
                fout.close();
            }
            else {
                std::cout << "Ошибка при создании файла " << filePath << "!" << std::endl;
            }

            std::string name;
            {
                std::ifstream fin(filePath.string());
                if (fin.is_open()) {
                    std::getline(fin, name);
                    fin.close();
                    std::cout << "[DEBUG] Прочитано из файла: " << name << std::endl;
                }
                else {
                    std::cout << "Ошибка открытия файла " << filePath << "!" << std::endl;
                }
            }
            std::cout << "[DEBUG] name: " << name << std::endl;
            for (unsigned char c : name) std::cout << std::hex << (int)c << ' ';
            std::cout << std::endl;

            std::cout << "Ставка (%): "; std::cin >> rate;
            std::cout << "Срок (мес): "; std::cin >> term;
            std::cout << "Мин. сумма: "; std::cin >> minAmount;
            std::cout << "Пополнение (1 - Да, 0 - Нет): "; std::cin >> replenishable;
            std::cout << "Снятие (1 - Да, 0 - Нет): "; std::cin >> withdrawable;
            std::cout << "Капитализация (1 - Да, 0 - Нет): "; std::cin >> capitalization;
            std::cout << "Штраф за досрочное снятие (1 - Да, 0 - Нет): "; std::cin >> earlyWithdrawal;
            Deposit dep(id, name, rate, term, minAmount, replenishable, withdrawable, capitalization, bankName, earlyWithdrawal);
            if (app.addNewDeposit(dep, bankId)) {
                std::cout << "Вклад успешно добавлен!\n";
            }
            else {
                std::cout << "Ошибка при добавлении вклада.\n";
            }

            std::remove(filePath.string().c_str());
        }
        else if (choice == 2) {
            std::cout << "Введите ID вклада для удаления: ";
            int delId; std::cin >> delId;
            if (app.deleteDeposit(delId)) {
                std::cout << "Вклад успешно удалён.\n";
            }
            else {
                std::cout << "Ошибка при удалении вклада.\n";
            }
        }
        else if (choice == 3) {
        }
        else if (choice == 4) {
        }
        else if (choice == 5) {
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
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
            std::cin >> mode;
            if (mode == 1) {
                clientMenu(app);
            } else if (mode == 2) {
                adminMenu(app);
            } else if (mode == 3) {
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