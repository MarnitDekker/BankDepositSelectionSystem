#pragma once
#include <string>

class Bank {
private:
    int id;
    std::string name;
    std::string license;
    double rating;
public:
    Bank(int id, const std::string& name, const std::string& license, double rating);

    int getId() const;
    std::string getName() const;
    std::string getLicense() const;
    double getRating() const;
};