// MenuSystem.cpp - 菜单系统类实现
#include "MenuSystem.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// 构造函数
MenuSystem::MenuSystem(MapNetwork& network, UserManager& userMgr) 
    : mapNetwork(network), userManager(userMgr), running(false) {}

// 启动菜单系统
void MenuSystem::start() {
    running = true;
    
    while (running) {
        displayMainMenu();
        int choice = getIntInput("请选择操作: ");
        handleMainMenu(choice);
    }
}

// 停止菜单系统
void MenuSystem::stop() {
    running = false;
}

// 检查系统是否正在运行
bool MenuSystem::isRunning() const {
    return running;
}

// 显示主菜单
void MenuSystem::displayMainMenu() {
    clearScreen();
    printHeader("地图网络图分析系统");
    
    std::cout << "1. 用户管理" << std::endl;
    std::cout << "2. 城市管理" << std::endl;
    std::cout << "3. 路线管理" << std::endl;
    std::cout << "4. 导航功能" << std::endl;
    std::cout << "5. 图操作" << std::endl;
    std::cout << "6. 数据管理" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    
    printSeparator();
    
    // 显示当前登录状态
    if (userManager.isLoggedIn()) {
        std::cout << "当前用户: " << userManager.getCurrentUser()->getUsername() << std::endl;
    } else {
        std::cout << "当前状态: 未登录" << std::endl;
    }
    
    printSeparator();
}

// 显示用户管理菜单
void MenuSystem::displayUserMenu() {
    clearScreen();
    printHeader("用户管理");
    
    std::cout << "1. 用户登录" << std::endl;
    std::cout << "2. 用户注册" << std::endl;
    std::cout << "3. 用户登出" << std::endl;
    std::cout << "4. 修改密码" << std::endl;
    std::cout << "5. 显示当前用户" << std::endl;
    std::cout << "6. 显示所有用户" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 显示城市管理菜单
void MenuSystem::displayCityMenu() {
    clearScreen();
    printHeader("城市管理");
    
    std::cout << "1. 添加城市" << std::endl;
    std::cout << "2. 删除城市" << std::endl;
    std::cout << "3. 查找城市" << std::endl;
    std::cout << "4. 显示所有城市" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 显示路线管理菜单
void MenuSystem::displayRouteMenu() {
    clearScreen();
    printHeader("路线管理");
    
    std::cout << "1. 添加路线" << std::endl;
    std::cout << "2. 删除路线" << std::endl;
    std::cout << "3. 检查路线是否存在" << std::endl;
    std::cout << "4. 显示路线信息" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 显示导航菜单
void MenuSystem::displayNavigationMenu() {
    clearScreen();
    printHeader("导航功能");
    
    std::cout << "1. 查找最短路径" << std::endl;
    std::cout << "2. 查找邻近城市" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 显示图操作菜单
void MenuSystem::displayGraphMenu() {
    clearScreen();
    printHeader("图操作");
    
    std::cout << "1. 深度优先遍历" << std::endl;
    std::cout << "2. 广度优先遍历" << std::endl;
    std::cout << "3. 切换图类型" << std::endl;
    std::cout << "4. 显示网络信息" << std::endl;
    std::cout << "5. 显示邻接矩阵/表" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 显示数据管理菜单
void MenuSystem::displayDataMenu() {
    clearScreen();
    printHeader("数据管理");
    
    std::cout << "1. 保存数据" << std::endl;
    std::cout << "2. 加载数据" << std::endl;
    std::cout << "3. 创建默认网络" << std::endl;
    std::cout << "4. 清空网络" << std::endl;
    std::cout << "0. 返回主菜单" << std::endl;
    
    printSeparator();
}

// 处理主菜单选择
void MenuSystem::handleMainMenu(int choice) {
    switch (choice) {
        case 1:
            handleUserMenuChoice();
            break;
        case 2:
            handleCityMenuChoice();
            break;
        case 3:
            handleRouteMenuChoice();
            break;
        case 4:
            handleNavigationMenuChoice();
            break;
        case 5:
            handleGraphMenuChoice();
            break;
        case 6:
            handleDataMenuChoice();
            break;
        case 0:
            if (confirmAction("退出系统")) {
                stop();
            }
            break;
        default:
            std::cout << "无效的选择，请重试。" << std::endl;
            pauseScreen();
    }
}

// 处理用户管理菜单选择
void MenuSystem::handleUserMenuChoice() {
    while (true) {
        displayUserMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleUserLogin(); break;
            case 2: handleUserRegister(); break;
            case 3: handleUserLogout(); break;
            case 4: handleChangePassword(); break;
            case 5: handleDisplayCurrentUser(); break;
            case 6: handleDisplayAllUsers(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理城市管理菜单选择
void MenuSystem::handleCityMenuChoice() {
    while (true) {
        displayCityMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleAddCity(); break;
            case 2: handleRemoveCity(); break;
            case 3: handleFindCity(); break;
            case 4: handleDisplayAllCities(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理路线管理菜单选择
void MenuSystem::handleRouteMenuChoice() {
    while (true) {
        displayRouteMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleAddRoute(); break;
            case 2: handleRemoveRoute(); break;
            case 3: handleCheckRoute(); break;
            case 4: handleDisplayRouteInfo(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理导航菜单选择
void MenuSystem::handleNavigationMenuChoice() {
    while (true) {
        displayNavigationMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleFindShortestPath(); break;
            case 2: handleFindNeighbors(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理图操作菜单选择
void MenuSystem::handleGraphMenuChoice() {
    while (true) {
        displayGraphMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleDFSTraverse(); break;
            case 2: handleBFSTraverse(); break;
            case 3: handleSwitchGraphType(); break;
            case 4: handleDisplayNetworkInfo(); break;
            case 5: handleDisplayRouteInfo(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理数据管理菜单选择
void MenuSystem::handleDataMenuChoice() {
    while (true) {
        displayDataMenu();
        int choice = getIntInput("请选择操作: ");
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1: handleSaveData(); break;
            case 2: handleLoadData(); break;
            case 3: handleCreateDefaultNetwork(); break;
            case 4: handleClearNetwork(); break;
            default:
                std::cout << "无效的选择，请重试。" << std::endl;
                pauseScreen();
        }
    }
}

// 处理用户登录
void MenuSystem::handleUserLogin() {
    std::string username = getStringInput("请输入用户名: ");
    std::string password = getStringInput("请输入密码: ");
    
    if (userManager.login(username, password)) {
        std::cout << "登录成功！欢迎 " << username << std::endl;
    } else {
        std::cout << "登录失败！用户名或密码错误。" << std::endl;
    }
    
    pauseScreen();
}

// 处理用户注册
void MenuSystem::handleUserRegister() {
    std::string username = getStringInput("请输入用户名: ");
    std::string password = getStringInput("请输入密码: ");
    std::string confirmPassword = getStringInput("请确认密码: ");
    
    if (password != confirmPassword) {
        std::cout << "两次输入的密码不一致！" << std::endl;
    } else if (userManager.registerUser(username, password)) {
        std::cout << "注册成功！" << std::endl;
    } else {
        std::cout << "注册失败！用户名已存在或输入无效。" << std::endl;
    }
    
    pauseScreen();
}

// 处理用户登出
void MenuSystem::handleUserLogout() {
    if (userManager.isLoggedIn()) {
        userManager.logout();
        std::cout << "登出成功！" << std::endl;
    } else {
        std::cout << "当前没有用户登录。" << std::endl;
    }
    
    pauseScreen();
}

// 处理修改密码
void MenuSystem::handleChangePassword() {
    if (!userManager.isLoggedIn()) {
        std::cout << "请先登录！" << std::endl;
        pauseScreen();
        return;
    }
    
    std::string oldPassword = getStringInput("请输入原密码: ");
    std::string newPassword = getStringInput("请输入新密码: ");
    std::string confirmPassword = getStringInput("请确认新密码: ");
    
    if (newPassword != confirmPassword) {
        std::cout << "两次输入的密码不一致！" << std::endl;
    } else {
        std::string username = userManager.getCurrentUser()->getUsername();
        if (userManager.changePassword(username, oldPassword, newPassword)) {
            std::cout << "密码修改成功！" << std::endl;
        } else {
            std::cout << "密码修改失败！原密码错误。" << std::endl;
        }
    }
    
    pauseScreen();
}

// 处理显示当前用户
void MenuSystem::handleDisplayCurrentUser() {
    if (userManager.isLoggedIn()) {
        std::cout << "当前用户: " << userManager.getCurrentUser()->getUsername() << std::endl;
    } else {
        std::cout << "当前没有用户登录。" << std::endl;
    }
    
    pauseScreen();
}

// 处理显示所有用户
void MenuSystem::handleDisplayAllUsers() {
    userManager.displayAllUsers();
    pauseScreen();
}

// 处理添加城市
void MenuSystem::handleAddCity() {
    int cityId = getIntInput("请输入城市ID: ");
    std::string cityName = getStringInput("请输入城市名称: ");
    
    if (mapNetwork.addCity(City(cityId, cityName))) {
        std::cout << "城市添加成功！" << std::endl;
    } else {
        std::cout << "城市添加失败！城市ID已存在。" << std::endl;
    }
    
    pauseScreen();
}

// 处理删除城市
void MenuSystem::handleRemoveCity() {
    std::cout << "选择删除方式:" << std::endl;
    std::cout << "1. 按城市ID删除" << std::endl;
    std::cout << "2. 按城市名称删除" << std::endl;
    
    int choice = getIntInput("请选择: ");
    
    if (choice == 1) {
        int cityId = getIntInput("请输入城市ID: ");
        if (confirmAction("删除城市")) {
            if (mapNetwork.removeCity(cityId)) {
                std::cout << "城市删除成功！" << std::endl;
            } else {
                std::cout << "城市删除失败！城市不存在。" << std::endl;
            }
        }
    } else if (choice == 2) {
        std::string cityName = getStringInput("请输入城市名称: ");
        if (confirmAction("删除城市")) {
            if (mapNetwork.removeCity(cityName)) {
                std::cout << "城市删除成功！" << std::endl;
            } else {
                std::cout << "城市删除失败！城市不存在。" << std::endl;
            }
        }
    } else {
        std::cout << "无效的选择。" << std::endl;
    }
    
    pauseScreen();
}

// 处理查找城市
void MenuSystem::handleFindCity() {
    std::cout << "选择查找方式:" << std::endl;
    std::cout << "1. 按城市ID查找" << std::endl;
    std::cout << "2. 按城市名称查找" << std::endl;
    
    int choice = getIntInput("请选择: ");
    
    if (choice == 1) {
        int cityId = getIntInput("请输入城市ID: ");
        City* city = mapNetwork.findCity(cityId);
        if (city) {
            std::cout << "找到城市: ";
            city->display();
        } else {
            std::cout << "未找到城市！" << std::endl;
        }
    } else if (choice == 2) {
        std::string cityName = getStringInput("请输入城市名称: ");
        City* city = mapNetwork.findCity(cityName);
        if (city) {
            std::cout << "找到城市: ";
            city->display();
        } else {
            std::cout << "未找到城市！" << std::endl;
        }
    } else {
        std::cout << "无效的选择。" << std::endl;
    }
    
    pauseScreen();
}

// 处理显示所有城市
void MenuSystem::handleDisplayAllCities() {
    mapNetwork.displayAllCities();
    pauseScreen();
}

// 处理添加路线
void MenuSystem::handleAddRoute() {
    std::cout << "选择添加方式:" << std::endl;
    std::cout << "1. 按城市ID添加" << std::endl;
    std::cout << "2. 按城市名称添加" << std::endl;
    
    int choice = getIntInput("请选择: ");
    
    if (choice == 1) {
        int fromId = getIntInput("请输入起点城市ID: ");
        int toId = getIntInput("请输入终点城市ID: ");
        int distance = getIntInput("请输入距离: ");
        
        if (mapNetwork.addRoute(fromId, toId, distance)) {
            std::cout << "路线添加成功！" << std::endl;
        } else {
            std::cout << "路线添加失败！城市不存在。" << std::endl;
        }
    } else if (choice == 2) {
        std::string fromCity = getStringInput("请输入起点城市名称: ");
        std::string toCity = getStringInput("请输入终点城市名称: ");
        int distance = getIntInput("请输入距离: ");
        
        if (mapNetwork.addRoute(fromCity, toCity, distance)) {
            std::cout << "路线添加成功！" << std::endl;
        } else {
            std::cout << "路线添加失败！城市不存在。" << std::endl;
        }
    } else {
        std::cout << "无效的选择。" << std::endl;
    }
    
    pauseScreen();
}

// 处理删除路线
void MenuSystem::handleRemoveRoute() {
    std::cout << "选择删除方式:" << std::endl;
    std::cout << "1. 按城市ID删除" << std::endl;
    std::cout << "2. 按城市名称删除" << std::endl;
    
    int choice = getIntInput("请选择: ");
    
    if (choice == 1) {
        int fromId = getIntInput("请输入起点城市ID: ");
        int toId = getIntInput("请输入终点城市ID: ");
        
        if (confirmAction("删除路线")) {
            if (mapNetwork.removeRoute(fromId, toId)) {
                std::cout << "路线删除成功！" << std::endl;
            } else {
                std::cout << "路线删除失败！路线不存在。" << std::endl;
            }
        }
    } else if (choice == 2) {
        std::string fromCity = getStringInput("请输入起点城市名称: ");
        std::string toCity = getStringInput("请输入终点城市名称: ");
        
        if (confirmAction("删除路线")) {
            if (mapNetwork.removeRoute(fromCity, toCity)) {
                std::cout << "路线删除成功！" << std::endl;
            } else {
                std::cout << "路线删除失败！路线不存在。" << std::endl;
            }
        }
    } else {
        std::cout << "无效的选择。" << std::endl;
    }
    
    pauseScreen();
}

// 处理检查路线是否存在
void MenuSystem::handleCheckRoute() {
    std::string fromCity = getStringInput("请输入起点城市名称: ");
    std::string toCity = getStringInput("请输入终点城市名称: ");
    
    City* from = mapNetwork.findCity(fromCity);
    City* to = mapNetwork.findCity(toCity);
    
    if (from && to) {
        if (mapNetwork.hasRoute(from->getId(), to->getId())) {
            int distance = mapNetwork.getRouteDistance(from->getId(), to->getId());
            std::cout << "路线存在！距离: " << distance << " 公里" << std::endl;
        } else {
            std::cout << "路线不存在！" << std::endl;
        }
    } else {
        std::cout << "城市不存在！" << std::endl;
    }
    
    pauseScreen();
}

// 处理显示路线信息
void MenuSystem::handleDisplayRouteInfo() {
    mapNetwork.displayRouteMatrix();
    pauseScreen();
}

// 处理查找最短路径
void MenuSystem::handleFindShortestPath() {
    std::string fromCity = getStringInput("请输入起点城市名称: ");
    std::string toCity = getStringInput("请输入终点城市名称: ");
    
    PathResult result = mapNetwork.findShortestPath(fromCity, toCity);
    
    if (result.found) {
        std::cout << "找到最短路径！" << std::endl;
        std::cout << "总距离: " << result.totalDistance << " 公里" << std::endl;
        std::cout << "路径: ";
        
        for (size_t i = 0; i < result.path.size(); ++i) {
            int cityId = result.path[i];
            std::string cityName = mapNetwork.findCity(cityId)->getName();
            std::cout << cityName;
            if (i < result.path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "未找到路径！" << std::endl;
    }
    
    pauseScreen();
}

// 处理查找邻近城市
void MenuSystem::handleFindNeighbors() {
    std::string cityName = getStringInput("请输入城市名称: ");
    
    auto neighbors = mapNetwork.getNeighboringCities(cityName);
    
    if (!neighbors.empty()) {
        std::cout << "邻近城市:" << std::endl;
        for (int neighborId : neighbors) {
            City* city = mapNetwork.findCity(neighborId);
            if (city) {
                int distance = mapNetwork.getRouteDistance(
                    mapNetwork.findCity(cityName)->getId(), neighborId);
                std::cout << "  - " << city->getName() << " (距离: " << distance << " 公里)" << std::endl;
            }
        }
    } else {
        std::cout << "该城市没有邻近城市，或城市不存在。" << std::endl;
    }
    
    pauseScreen();
}

// 处理深度优先遍历
void MenuSystem::handleDFSTraverse() {
    std::string startCity = getStringInput("请输入起始城市名称: ");
    
    auto result = mapNetwork.traverseDFS(startCity);
    
    if (!result.empty()) {
        std::cout << "DFS遍历结果:" << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            int cityId = result[i];
            City* city = mapNetwork.findCity(cityId);
            if (city) {
                std::cout << city->getName();
                if (i < result.size() - 1) {
                    std::cout << " -> ";
                }
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "遍历失败！起始城市不存在或网络为空。" << std::endl;
    }
    
    pauseScreen();
}

// 处理广度优先遍历
void MenuSystem::handleBFSTraverse() {
    std::string startCity = getStringInput("请输入起始城市名称: ");
    
    auto result = mapNetwork.traverseBFS(startCity);
    
    if (!result.empty()) {
        std::cout << "BFS遍历结果:" << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            int cityId = result[i];
            City* city = mapNetwork.findCity(cityId);
            if (city) {
                std::cout << city->getName();
                if (i < result.size() - 1) {
                    std::cout << " -> ";
                }
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "遍历失败！起始城市不存在或网络为空。" << std::endl;
    }
    
    pauseScreen();
}

// 处理切换图类型
void MenuSystem::handleSwitchGraphType() {
    std::cout << "当前图类型: " << mapNetwork.getGraphType() << std::endl;
    std::cout << "1. 邻接矩阵" << std::endl;
    std::cout << "2. 邻接表" << std::endl;
    
    int choice = getIntInput("请选择图类型: ");
    
    if (choice == 1) {
        mapNetwork.setGraphType(true);
        std::cout << "已切换到邻接矩阵。" << std::endl;
    } else if (choice == 2) {
        mapNetwork.setGraphType(false);
        std::cout << "已切换到邻接表。" << std::endl;
    } else {
        std::cout << "无效的选择。" << std::endl;
    }
    
    pauseScreen();
}

// 处理显示网络信息
void MenuSystem::handleDisplayNetworkInfo() {
    mapNetwork.displayNetworkInfo();
    pauseScreen();
}

// 处理保存数据
void MenuSystem::handleSaveData() {
    if (mapNetwork.saveData()) {
        std::cout << "数据保存成功！" << std::endl;
    } else {
        std::cout << "数据保存失败！" << std::endl;
    }
    
    pauseScreen();
}

// 处理加载数据
void MenuSystem::handleLoadData() {
    if (mapNetwork.loadData()) {
        std::cout << "数据加载成功！" << std::endl;
    } else {
        std::cout << "数据加载失败！可能是文件不存在。" << std::endl;
    }
    
    pauseScreen();
}

// 处理创建默认网络
void MenuSystem::handleCreateDefaultNetwork() {
    if (confirmAction("创建默认网络（将清空现有数据）")) {
        if (mapNetwork.createDefaultNetwork()) {
            std::cout << "默认网络创建成功！" << std::endl;
        } else {
            std::cout << "默认网络创建失败！" << std::endl;
        }
    }
    
    pauseScreen();
}

// 处理清空网络
void MenuSystem::handleClearNetwork() {
    if (confirmAction("清空网络（将删除所有数据）")) {
        if (mapNetwork.clearNetwork()) {
            std::cout << "网络清空成功！" << std::endl;
        } else {
            std::cout << "网络清空失败！" << std::endl;
        }
    }
    
    pauseScreen();
}

// 清屏
void MenuSystem::clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

// 暂停屏幕
void MenuSystem::pauseScreen() {
    std::cout << "\n按Enter键继续...";
    std::cin.ignore();
    std::cin.get();
}

// 打印标题
void MenuSystem::printHeader(const std::string& title) {
    std::cout << std::string(50, '=') << std::endl;
    std::cout << " " << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

// 打印分隔线
void MenuSystem::printSeparator() {
    std::cout << std::string(50, '-') << std::endl;
}

// 获取整数输入
int MenuSystem::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "输入无效，请输入一个整数。" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// 获取字符串输入
std::string MenuSystem::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// 确认操作
bool MenuSystem::confirmAction(const std::string& action) {
    std::string response;
    std::cout << "确认" << action << "吗? (y/n): ";
    std::getline(std::cin, response);
    return (response == "y" || response == "Y");
}
