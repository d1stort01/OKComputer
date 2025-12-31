// UserManager.cpp - 用户管理类实现
#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// User 类实现

// 默认构造函数
User::User() : username(""), password("") {}

// 带参数的构造函数
User::User(const std::string& name, const std::string& pwd) : username(name), password(pwd) {}

// Getter 方法
std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

// 验证密码
bool User::verifyPassword(const std::string& pwd) const {
    return password == pwd;
}

// 友元函数：输出流运算符重载
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User{" << user.username << ", " << user.password << "}";
    return os;
}

// ==================== UserManager 类实现 ====================

// 构造函数
UserManager::UserManager(const std::string& filename) : userDataFile(filename), currentUser(nullptr) {
    loadFromFile();
}

// 析构函数
UserManager::~UserManager() {
    saveToFile();
}

// 从文件加载用户数据
bool UserManager::loadFromFile() {
    std::ifstream file(userDataFile);
    if (!file.is_open()) {
        return false;  // 文件不存在，首次运行
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password;
        
        if (iss >> username >> password) {
            users[username] = User(username, password);
        }
    }
    
    file.close();
    return true;
}

// 保存用户数据到文件
bool UserManager::saveToFile() const {
    std::ofstream file(userDataFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& pair : users) {
        file << pair.second.getUsername() << " " << pair.second.getPassword() << std::endl;
    }
    
    file.close();
    return true;
}

// 用户注册
bool UserManager::registerUser(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        return false;
    }
    
    if (userExists(username)) {
        return false;  // 用户已存在
    }
    
    users[username] = User(username, password);
    return saveToFile();
}

// 用户登录
bool UserManager::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;  // 用户不存在
    }
    
    if (it->second.verifyPassword(password)) {
        currentUser = &it->second;
        return true;
    }
    
    return false;  // 密码错误
}

// 用户登出
void UserManager::logout() {
    currentUser = nullptr;
}

// 检查用户是否存在
bool UserManager::userExists(const std::string& username) const {
    return users.find(username) != users.end();
}

// 获取当前登录用户
User* UserManager::getCurrentUser() const {
    return currentUser;
}

// 检查是否有用户已登录
bool UserManager::isLoggedIn() const {
    return currentUser != nullptr;
}

// 获取所有用户数量
int UserManager::getUserCount() const {
    return users.size();
}

// 显示所有用户信息
void UserManager::displayAllUsers() const {
    std::cout << "\n=== 所有用户信息 ===" << std::endl;
    std::cout << "总用户数: " << users.size() << std::endl;
    
    for (const auto& pair : users) {
        std::cout << "用户名: " << pair.second.getUsername() << std::endl;
    }
}

// 修改密码
bool UserManager::changePassword(const std::string& username, const std::string& oldPassword, 
                                const std::string& newPassword) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }
    
    if (!it->second.verifyPassword(oldPassword)) {
        return false;  // 原密码错误
    }
    
    // 更新密码
    users[username] = User(username, newPassword);
    
    // 如果是当前用户，更新当前用户指针
    if (currentUser && currentUser->getUsername() == username) {
        currentUser = &users[username];
    }
    
    return saveToFile();
}

// 删除用户
bool UserManager::deleteUser(const std::string& username) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }
    
    // 如果是当前用户，先登出
    if (currentUser && currentUser->getUsername() == username) {
        logout();
    }
    
    users.erase(it);
    return saveToFile();
}