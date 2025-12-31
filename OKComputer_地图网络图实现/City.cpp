// City.cpp - 城市信息类实现
#include "City.h"
#include <iostream>

// 默认构造函数
City::City() : id(0), name("") {}

// 带参数的构造函数
City::City(int cityId, const std::string& cityName) : id(cityId), name(cityName) {}

// 拷贝构造函数
City::City(const City& other) : id(other.id), name(other.name) {}

// 赋值运算符重载
City& City::operator=(const City& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
    }
    return *this;
}

// 等于运算符重载
bool City::operator==(const City& other) const {
    return id == other.id;
}

// 不等于运算符重载
bool City::operator!=(const City& other) const {
    return !(*this == other);
}

// Getter 方法
int City::getId() const {
    return id;
}

// Setter 方法
void City::setId(int cityId) {
    id = cityId;
}

// Getter 方法
std::string City::getName() const {
    return name;
}

// Setter 方法
void City::setName(const std::string& cityName) {
    name = cityName;
}

// 显示城市信息
void City::display() const {
    std::cout << "城市ID: " << id << ", 城市名称: " << name << std::endl;
}

// 友元函数：输出流运算符重载
std::ostream& operator<<(std::ostream& os, const City& city) {
    os << "City{" << city.id << ", " << city.name << "}";
    return os;
}