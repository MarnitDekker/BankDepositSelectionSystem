#pragma once
#include "AppController.h"
#include "Client.h"

class UserInterface {
private:
    std::shared_ptr<AppController> app;
public:
    explicit UserInterface(std::shared_ptr<AppController> controller);

    Client createClient();
    void adminMenu();
    void clientMenu();
    void runMainLoop();
};