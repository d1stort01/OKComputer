// MenuSystem.h - 菜单系统类声明
#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "MapNetwork.h"
#include "UserManager.h"
#include <string>

// 菜单系统类
class MenuSystem {
private:
    MapNetwork& mapNetwork;
    UserManager& userManager;
    bool running;
    
    // 显示主菜单
    void displayMainMenu();
    
    // 显示用户管理菜单
    void displayUserMenu();
    
    // 显示城市管理菜单
    void displayCityMenu();
    
    // 显示路线管理菜单
    void displayRouteMenu();
    
    // 显示导航菜单
    void displayNavigationMenu();
    
    // 显示图操作菜单
    void displayGraphMenu();
    
    // 显示数据管理菜单
    void displayDataMenu();
    
    // 处理主菜单选择
    void handleMainMenu(int choice);
    
    // 处理用户管理菜单选择
    void handleUserMenu(int choice);
    
    // 处理城市管理菜单选择
    void handleCityMenu(int choice);
    
    // 处理路线管理菜单选择
    void handleRouteMenu(int choice);
    
    // 处理导航菜单选择
    void handleNavigationMenu(int choice);
    
    // 处理图操作菜单选择
    void handleGraphMenu(int choice);
    
    // 处理数据管理菜单选择
    void handleDataMenu(int choice);
    
    // 用户管理功能
    void handleUserLogin();
    void handleUserRegister();
    void handleUserLogout();
    void handleChangePassword();
    void handleDisplayCurrentUser();
    
    // 城市管理功能
    void handleAddCity();
    void handleRemoveCity();
    void handleFindCity();
    void handleDisplayAllCities();
    
    // 路线管理功能
    void handleAddRoute();
    void handleRemoveRoute();
    void handleCheckRoute();
    void handleDisplayRouteInfo();
    
    // 导航功能
    void handleFindShortestPath();
    void handleFindNeighbors();
    
    // 图操作功能
    void handleDFSTraverse();
    void handleBFSTraverse();
    void handleSwitchGraphType();
    void handleDisplayNetworkInfo();
    
    // 数据管理功能
    void handleSaveData();
    void handleLoadData();
    void handleCreateDefaultNetwork();
    void handleClearNetwork();
    
    // 工具函数
    void clearScreen();
    void pauseScreen();
    void printHeader(const std::string& title);
    void printSeparator();
    int getIntInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
    bool confirmAction(const std::string& action);
    
public:
    // 构造函数
    MenuSystem(MapNetwork& network, UserManager& userManager);
    
    // 启动菜单系统
    void start();
    
    // 停止菜单系统
    void stop();
    
    // 检查系统是否正在运行
    bool isRunning() const;
};

#endif // MENUSYSTEM_H