#include "../UserInterface.h"
#pragma execution_character_set("utf-8")

UserInterface::UserInterface(std::shared_ptr<AppController> controller)
    : app(controller) {
}

Client UserInterface::createClient() {
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

void UserInterface::clientMenu() {
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
            app->logUserQuery(client);
            app->processClientRequest(client);
            break;
        }
        case 2: {
            app->showAllDeposits();
            break;
        }
        case 3: {
            return;
        }
        default: {
            std::cout << "Неверный выбор. Введите число от 1 до 3.\n";
        }
        }
    }
}

void UserInterface::adminMenu() {
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
        std::cout << "6. Просмотреть все банки\n";
        std::cout << "7. Выйти в главное меню\n";
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
            app->handleAddDeposit();
            break;
        }
        case 2: {
            app->handleDeleteDeposit();
            break;
        }
        case 3: {
            app->showUserQueryHistory();
            break;
        } case 4: {
            app->handleAddBank();
            break;
        }
        case 5: {
            app->handleDeleteBank();
            break;
        }
        case 6: {
            app->showAllBanksDetailed();
            break;
        }
        case 7: {
            return;
        }
        default: {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
            break;
        }
        }
    }
}

void UserInterface::runMainLoop() {
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

        switch (mode) {
        case 1: {
            clientMenu();
            break;
        }
        case 2: {
            adminMenu(); 
            break;
        }
        case 3: {
            std::filesystem::remove("user_queries.log");
            return;
        }
        default: 
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}