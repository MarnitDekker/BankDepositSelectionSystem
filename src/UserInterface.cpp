#include "../UserInterface.h"
#pragma execution_character_set("utf-8")

UserInterface::UserInterface(std::shared_ptr<AppController> controller)
    : app(controller) {
}

Client UserInterface::createClient() {
    double amount = 0;
    int term = 0;
    bool replenishable = false, withdrawable = false;

    std::cout << "\n=== ���� ���������� ������ ===\n";

    while (true) {
        std::cout << "������� ����� ������ (�� ����� 5000 ���): ";
        if (!(std::cin >> amount)) {
            std::cout << "������: ������� �������� ��������!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (amount < 5000) {
            std::cout << "������: ����������� ����� ������ - 5000 ���!\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "������� ���� ������ (���, 0 - ����� ����): ";
        if (!(std::cin >> term)) {
            std::cout << "������: ������� ����� �����!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (term < 0) {
            std::cout << "������: ���� �� ����� ���� �������������!\n";
            continue;
        }
        break;
    }

    int input;
    while (true) {
        std::cout << "��������� ����������� ����������? (1 - ��, 0 - ���): ";
        if (!(std::cin >> input)) {
            std::cout << "������: ������� 1 ��� 0!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (input != 0 && input != 1) {
            std::cout << "������: ������� ������ 1 ��� 0!\n";
            continue;
        }
        replenishable = (input == 1);
        break;
    }

    while (true) {
        std::cout << "��������� ����������� ���������� ������? (1 - ��, 0 - ���): ";
        if (!(std::cin >> input)) {
            std::cout << "������: ������� 1 ��� 0!\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }
        if (input != 0 && input != 1) {
            std::cout << "������: ������� ������ 1 ��� 0!\n";
            continue;
        }
        withdrawable = (input == 1);
        break;
    }

    return Client(amount, term, replenishable, withdrawable);
}

void UserInterface::clientMenu() {
    while (true) {
        std::cout << "\n=== ������� ������� ���������� ������� ===\n";
        std::cout << "1. ��������� �����\n";
        std::cout << "2. ����������� ��� ������\n";
        std::cout << "3. ����� � ������� ����\n";
        std::cout << "�������� ��������: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "������: ������� ����� �� 1 �� 3!\n";
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
            std::cout << "�������� �����. ������� ����� �� 1 �� 3.\n";
        }
        }
    }
}

void UserInterface::adminMenu() {
    const std::string adminPassword = "12345";
    std::string inputPassword;
    std::cout << "\n������� ������ ��������������: ";
    std::cin >> inputPassword;
    if (inputPassword != adminPassword) {
        std::cout << "�������� ������. ������ ��������.\n";
        return;
    }
    while (true) {
        std::cout << "\n=== ���� �������������� ===\n";
        std::cout << "1. �������� ����� �����\n";
        std::cout << "2. ������� �����\n";
        std::cout << "3. ����������� ������� �������� �������������\n";
        std::cout << "4. �������� ����� ����\n";
        std::cout << "5. ������� ����\n";
        std::cout << "6. ����������� ��� �����\n";
        std::cout << "7. ����� � ������� ����\n";
        std::cout << "�������� ��������: " << std::flush;
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "������: ������� ����� �� 1 �� 6!\n";
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
            std::cout << "�������� �����. ���������� �����.\n";
            break;
        }
        }
    }
}

void UserInterface::runMainLoop() {
    while (true) {
        std::cout << "\n=== ����� ���������� � ������� ������� ������� ===\n";
        std::cout << "1. ���� �������\n";
        std::cout << "2. ���� ��������������\n";
        std::cout << "3. �����\n";
        std::cout << "�������� �����: " << std::flush;

        int mode;
        if (!(std::cin >> mode)) {
            std::cout << "������: ������� ����� �� 1 �� 3!\n";
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
            std::cout << "�������� �����. ���������� �����.\n";
        }
    }
}