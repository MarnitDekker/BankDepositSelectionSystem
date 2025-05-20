#pragma once
#include "AppController.h"
#include "Client.h"

class UserInterface {
private:
    AppController* app;
public:
    explicit UserInterface(AppController* controller);

    Client createClient();
    void adminMenu();
    void clientMenu();
    void runMainLoop();
};