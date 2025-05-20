#include "BankDepositSelectionSystem.h"
#include "include/Database/SQLiteDatabase.h"
#include "include/Analysis/BasicDepositAnalyzer.h"
#include "include/Client.h"
#include "include/AppController.h"
#include "include/Factory.h"
#include "include/UserInterface.h"
#pragma execution_character_set("utf-8")

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    std::locale::global(std::locale("ru_RU.utf8"));

    try {
        std::filesystem::path dbPath;
        {
            std::filesystem::path projectRoot = std::filesystem::current_path();
            std::cout << std::filesystem::current_path() << std::endl;
            bool found = false;

            for (int i = 0; i < 5; ++i) {
                if (std::filesystem::exists(projectRoot / "CMakeLists.txt")) {
                    found = true;
                    break;
                }
                projectRoot = projectRoot.parent_path();
            }

            if (!found) {
                projectRoot = std::filesystem::current_path();
            }

            dbPath = projectRoot / "data" / "deposits.db";
            std::filesystem::create_directories(dbPath.parent_path());
        }

        auto db = std::make_unique<SQLiteDatabase>(dbPath.string());
        if (!db->connect()) {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }

        AppController app(std::move(db), std::make_unique<BasicDepositAnalyzer>());
        UserInterface ui(&app);
        ui.runMainLoop();

    } catch (const std::exception& e) {
        std::cerr << "\n!!! Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}