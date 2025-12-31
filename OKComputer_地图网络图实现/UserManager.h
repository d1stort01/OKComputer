// UserManager.h - 用户管理类声明
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// 用户类
class User {
private:
    std::string username;
    std::string password;
    
public:
    User();
    User(const std::string& name, const std::string& pwd);
    
    // Getter 方法
    std::string getUsername() const;
    std::string getPassword() const;
    
    // 验证密码
    bool verifyPassword(const std::string& pwd) const;
    
    // 友元函数
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

// 用户管理类
class UserManager {
private:
    std::map<std::string, User> users;  // 用户名到用户对象的映射
    std::string userDataFile;           // 用户数据文件路径
    User* currentUser;                  // 当前登录用户
    
    // 从文件加载用户数据
    bool loadFromFile();
    
    // 保存用户数据到文件
    bool saveToFile() const;
    
public:
    // 构造函数
    UserManager(const std::string& filename = "users.txt");
    
    // 析构函数
    ~UserManager();
    
    // 用户注册
    bool registerUser(const std::string& username, const std::string& password);
    
    // 用户登录
    bool login(const std::string& username, const std::string& password);
    
    // 用户登出
    void logout();
    
    // 检查用户是否存在
    bool userExists(const std::string& username) const;
    
    // 获取当前登录用户
    User* getCurrentUser() const;
    
    // 检查是否有用户已登录
    bool isLoggedIn() const;
    
    // 获取所有用户数量
    int getUserCount() const;
    
    // 显示所有用户信息（管理员功能）
    void displayAllUsers() const;
    
    // 修改密码
    bool changePassword(const std::string& username, const std::string& oldPassword, 
                       const std::string& newPassword);
    
    // 删除用户
    bool deleteUser(const std::string& username);
};

#endif // USERMANAGER_H