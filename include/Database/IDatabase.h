#pragma once
#include <vector>
#include <memory>

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool executeSQL(const std::string& sql) = 0;
    virtual bool initializeDatabase() = 0;
};