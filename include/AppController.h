#pragma once
#include "IDatabase.h"

class AppController {
private:
    std::unique_ptr<IDatabase> database;
};