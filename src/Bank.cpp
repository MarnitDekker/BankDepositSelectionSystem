#include "Bank.h"

Bank::Bank(int id, const std::string& name, const std::string& license, double rating)
    : id(id), name(name), license(license), rating(rating) {
}

int Bank::getId() const { return id; }
std::string Bank::getName() const { return name; }
std::string Bank::getLicense() const { return license; }
double Bank::getRating() const { return rating; }