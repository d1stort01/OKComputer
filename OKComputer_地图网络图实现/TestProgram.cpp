// TestProgram.cpp - 地图网络系统测试程序
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "MapNetwork.h"
#include "UserManager.h"
#include "FileManager.h"

class TestProgram {
private:
    MapNetwork* mapNetwork;
    UserManager* userManager;
    FileManager* fileManager;
    
    void printTestHeader(const std::string& testName) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "测试: " << testName << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void printSubTest(const std::string& subTest) {
        std::cout << "\n--- " << subTest << " ---" << std::endl;
    }
    
    void pauseForScreenshot() {
        std::cout << "\n[按Enter键继续...]";
        std::cin.get();
    }
    
public:
    TestProgram() {
        mapNetwork = new MapNetwork(true);
        userManager = new UserManager("test_users.txt");
        fileManager = new FileManager("test_cities.txt", "test_routes.txt");
    }
    
    ~TestProgram() {
        delete mapNetwork;
        delete userManager;
        delete fileManager;
    }
    
    void runAllTests() {
        std::cout << "地图网络图分析系统 - 完整功能测试" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "\n本测试将演示系统的所有功能。" << std::endl;
        std::cout << "每个测试步骤后都会暂停，方便截图。" << std::endl;
        std::cout << "\n按Enter键开始测试...";
        std::cin.get();
        
        testUserManagement();
        testCityManagement();
        testRouteManagement();
        testGraphAlgorithms();
        testFileOperations();
        testEdgeCases();
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "所有测试完成！" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void testUserManagement() {
        printTestHeader("用户管理功能测试");
        
        // 测试用户注册
        printSubTest("用户注册");
        std::cout << "注册用户 'admin'..." << std::endl;
        bool result = userManager->registerUser("admin", "admin123");
        std::cout << "注册结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "注册用户 'testuser'..." << std::endl;
        result = userManager->registerUser("testuser", "test123");
        std::cout << "注册结果: " << (result ? "成功" : "失败") << std::endl;
        
        // 测试重复注册
        std::cout << "尝试重复注册 'admin'..." << std::endl;
        result = userManager->registerUser("admin", "newpass");
        std::cout << "注册结果: " << (result ? "成功" : "失败（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 测试用户登录
        printSubTest("用户登录");
        std::cout << "用户 'admin' 登录..." << std::endl;
        result = userManager->login("admin", "admin123");
        std::cout << "登录结果: " << (result ? "成功" : "失败") << std::endl;
        
        if (result) {
            std::cout << "当前用户: " << userManager->getCurrentUser()->getUsername() << std::endl;
        }
        
        std::cout << "用户 'admin' 使用错误密码登录..." << std::endl;
        result = userManager->login("admin", "wrongpass");
        std::cout << "登录结果: " << (result ? "成功" : "失败（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 测试修改密码
        printSubTest("修改密码");
        std::cout << "修改用户 'admin' 的密码..." << std::endl;
        result = userManager->changePassword("admin", "admin123", "newpass123");
        std::cout << "修改结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "使用新密码登录..." << std::endl;
        userManager->logout();
        result = userManager->login("admin", "newpass123");
        std::cout << "登录结果: " << (result ? "成功" : "失败") << std::endl;
        
        pauseForScreenshot();
    }
    
    void testCityManagement() {
        printTestHeader("城市管理功能测试");
        
        // 测试添加城市
        printSubTest("添加城市");
        std::cout << "添加城市: 北京 (ID: 1)" << std::endl;
        bool result = mapNetwork->addCity(City(1, "北京"));
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加城市: 上海 (ID: 2)" << std::endl;
        result = mapNetwork->addCity(City(2, "上海"));
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加城市: 广州 (ID: 3)" << std::endl;
        result = mapNetwork->addCity(City(3, "广州"));
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加城市: 深圳 (ID: 4)" << std::endl;
        result = mapNetwork->addCity(City(4, "深圳"));
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        // 测试重复添加
        std::cout << "尝试重复添加北京 (ID: 1)..." << std::endl;
        result = mapNetwork->addCity(City(1, "新北京"));
        std::cout << "添加结果: " << (result ? "成功" : "失败（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 测试查找城市
        printSubTest("查找城市");
        std::cout << "查找城市ID 2..." << std::endl;
        City* city = mapNetwork->findCity(2);
        if (city) {
            std::cout << "找到: ";
            city->display();
        }
        
        std::cout << "查找城市名称 '广州'..." << std::endl;
        city = mapNetwork->findCity("广州");
        if (city) {
            std::cout << "找到: ";
            city->display();
        }
        
        std::cout << "查找不存在的城市 '成都'..." << std::endl;
        city = mapNetwork->findCity("成都");
        std::cout << "查找结果: " << (city ? "找到" : "未找到（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 显示所有城市
        printSubTest("显示所有城市");
        mapNetwork->displayAllCities();
        
        pauseForScreenshot();
        
        // 测试删除城市
        printSubTest("删除城市");
        std::cout << "删除城市ID 4 (深圳)..." << std::endl;
        result = mapNetwork->removeCity(4);
        std::cout << "删除结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "删除城市名称 '广州'..." << std::endl;
        result = mapNetwork->removeCity("广州");
        std::cout << "删除结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "尝试删除不存在的城市..." << std::endl;
        result = mapNetwork->removeCity(99);
        std::cout << "删除结果: " << (result ? "成功" : "失败（预期）") << std::endl;
        
        pauseForScreenshot();
    }
    
    void testRouteManagement() {
        printTestHeader("路线管理功能测试");
        
        // 重新添加城市
        mapNetwork->addCity(City(3, "广州"));
        mapNetwork->addCity(City(4, "深圳"));
        mapNetwork->addCity(City(5, "杭州"));
        mapNetwork->addCity(City(6, "南京"));
        mapNetwork->addCity(City(7, "武汉"));
        mapNetwork->addCity(City(8, "成都"));
        
        // 测试添加路线
        printSubTest("添加路线");
        std::cout << "添加路线: 北京 -> 上海 (距离: 1200km)" << std::endl;
        bool result = mapNetwork->addRoute(1, 2, 1200);
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加路线: 上海 -> 杭州 (距离: 200km)" << std::endl;
        result = mapNetwork->addRoute(2, 5, 200);
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加路线: 北京 -> 南京 (距离: 1000km)" << std::endl;
        result = mapNetwork->addRoute(1, 6, 1000);
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "添加路线: 杭州 -> 广州 (距离: 1200km)" << std::endl;
        result = mapNetwork->addRoute(5, 3, 1200);
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        // 测试重复添加
        std::cout << "尝试重复添加北京->上海路线..." << std::endl;
        result = mapNetwork->addRoute(1, 2, 1500);
        std::cout << "添加结果: " << (result ? "成功" : "失败") << std::endl;
        
        pauseForScreenshot();
        
        // 测试检查路线
        printSubTest("检查路线");
        std::cout << "检查北京->上海路线..." << std::endl;
        result = mapNetwork->hasRoute(1, 2);
        std::cout << "路线存在: " << (result ? "是" : "否") << std::endl;
        
        if (result) {
            int distance = mapNetwork->getRouteDistance(1, 2);
            std::cout << "路线距离: " << distance << " 公里" << std::endl;
        }
        
        std::cout << "检查不存在的路线 北京->成都..." << std::endl;
        result = mapNetwork->hasRoute(1, 8);
        std::cout << "路线存在: " << (result ? "是" : "否（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 测试获取邻近城市
        printSubTest("获取邻近城市");
        std::cout << "获取北京的邻近城市..." << std::endl;
        auto neighbors = mapNetwork->getNeighboringCities(1);
        std::cout << "邻近城市数量: " << neighbors.size() << std::endl;
        for (int cityId : neighbors) {
            City* city = mapNetwork->findCity(cityId);
            if (city) {
                int distance = mapNetwork->getRouteDistance(1, cityId);
                std::cout << "  - " << city->getName() << " (距离: " << distance << "km)" << std::endl;
            }
        }
        
        pauseForScreenshot();
        
        // 显示邻接矩阵
        printSubTest("显示邻接矩阵");
        mapNetwork->displayRouteMatrix();
        
        pauseForScreenshot();
        
        // 测试删除路线
        printSubTest("删除路线");
        std::cout << "删除北京->南京路线..." << std::endl;
        result = mapNetwork->removeRoute(1, 6);
        std::cout << "删除结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "再次检查北京->南京路线..." << std::endl;
        result = mapNetwork->hasRoute(1, 6);
        std::cout << "路线存在: " << (result ? "是" : "否（预期）") << std::endl;
        
        pauseForScreenshot();
    }
    
    void testGraphAlgorithms() {
        printTestHeader("图算法测试");
        
        // 添加更多路线形成完整网络
        mapNetwork->addRoute(2, 7, 800);   // 上海-武汉
        mapNetwork->addRoute(6, 7, 500);   // 南京-武汉
        mapNetwork->addRoute(7, 8, 1100);  // 武汉-成都
        mapNetwork->addRoute(3, 4, 150);   // 广州-深圳
        
        // 测试DFS遍历
        printSubTest("深度优先搜索 (DFS)");
        std::cout << "从北京开始进行DFS遍历..." << std::endl;
        auto dfsResult = mapNetwork->traverseDFS(1);
        
        std::cout << "DFS遍历结果:" << std::endl;
        for (size_t i = 0; i < dfsResult.size(); ++i) {
            int cityId = dfsResult[i];
            City* city = mapNetwork->findCity(cityId);
            if (city) {
                std::cout << city->getName();
                if (i < dfsResult.size() - 1) {
                    std::cout << " -> ";
                }
            }
        }
        std::cout << std::endl;
        
        pauseForScreenshot();
        
        // 测试BFS遍历
        printSubTest("广度优先搜索 (BFS)");
        std::cout << "从北京开始进行BFS遍历..." << std::endl;
        auto bfsResult = mapNetwork->traverseBFS(1);
        
        std::cout << "BFS遍历结果:" << std::endl;
        for (size_t i = 0; i < bfsResult.size(); ++i) {
            int cityId = bfsResult[i];
            City* city = mapNetwork->findCity(cityId);
            if (city) {
                std::cout << city->getName();
                if (i < bfsResult.size() - 1) {
                    std::cout << " -> ";
                }
            }
        }
        std::cout << std::endl;
        
        pauseForScreenshot();
        
        // 测试最短路径
        printSubTest("Dijkstra最短路径算法");
        
        std::cout << "查找北京到上海的最短路径..." << std::endl;
        auto pathResult = mapNetwork->findShortestPath(1, 2);
        
        if (pathResult.found) {
            std::cout << "找到最短路径！" << std::endl;
            std::cout << "总距离: " << pathResult.totalDistance << " 公里" << std::endl;
            std::cout << "路径: ";
            for (size_t i = 0; i < pathResult.path.size(); ++i) {
                int cityId = pathResult.path[i];
                City* city = mapNetwork->findCity(cityId);
                if (city) {
                    std::cout << city->getName();
                    if (i < pathResult.path.size() - 1) {
                        std::cout << " -> ";
                    }
                }
            }
            std::cout << std::endl;
        } else {
            std::cout << "未找到路径！" << std::endl;
        }
        
        pauseForScreenshot();
        
        std::cout << "查找北京到成都的最短路径..." << std::endl;
        pathResult = mapNetwork->findShortestPath(1, 8);
        
        if (pathResult.found) {
            std::cout << "找到最短路径！" << std::endl;
            std::cout << "总距离: " << pathResult.totalDistance << " 公里" << std::endl;
            std::cout << "路径: ";
            for (size_t i = 0; i < pathResult.path.size(); ++i) {
                int cityId = pathResult.path[i];
                City* city = mapNetwork->findCity(cityId);
                if (city) {
                    std::cout << city->getName();
                    if (i < pathResult.path.size() - 1) {
                        std::cout << " -> ";
                    }
                }
            }
            std::cout << std::endl;
        } else {
            std::cout << "未找到路径！" << std::endl;
        }
        
        pauseForScreenshot();
        
        std::cout << "查找不连通的路径（北京->深圳，无直接路线）..." << std::endl;
        pathResult = mapNetwork->findShortestPath(1, 4);
        
        if (pathResult.found) {
            std::cout << "找到路径！" << std::endl;
            std::cout << "总距离: " << pathResult.totalDistance << " 公里" << std::endl;
            std::cout << "路径: ";
            for (size_t i = 0; i < pathResult.path.size(); ++i) {
                int cityId = pathResult.path[i];
                City* city = mapNetwork->findCity(cityId);
                if (city) {
                    std::cout << city->getName();
                    if (i < pathResult.path.size() - 1) {
                        std::cout << " -> ";
                    }
                }
            }
            std::cout << std::endl;
        } else {
            std::cout << "未找到路径（预期，因为没有连接）！" << std::endl;
        }
        
        pauseForScreenshot();
    }
    
    void testFileOperations() {
        printTestHeader("文件操作测试");
        
        // 测试保存数据
        printSubTest("保存数据到文件");
        std::cout << "保存城市数据到 test_cities.txt..." << std::endl;
        bool result = fileManager->saveCities({City(1, "北京"), City(2, "上海"), City(3, "广州")});
        std::cout << "保存结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "保存路线数据到 test_routes.txt..." << std::endl;
        result = fileManager->saveRoutes(*mapNetwork);
        std::cout << "保存结果: " << (result ? "成功" : "失败") << std::endl;
        
        pauseForScreenshot();
        
        // 测试加载数据
        printSubTest("从文件加载数据");
        
        // 先清空网络
        mapNetwork->clearNetwork();
        std::cout << "清空网络，当前城市数量: " << mapNetwork->getCityCount() << std::endl;
        
        std::cout << "从文件加载城市数据..." << std::endl;
        std::vector<City> loadedCities;
        result = fileManager->loadCities(loadedCities);
        std::cout << "加载结果: " << (result ? "成功" : "失败") << std::endl;
        std::cout << "加载的城市数量: " << loadedCities.size() << std::endl;
        
        for (const auto& city : loadedCities) {
            mapNetwork->addCity(city);
        }
        
        std::cout << "从文件加载路线数据..." << std::endl;
        result = fileManager->loadRoutes(*mapNetwork);
        std::cout << "加载结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "加载后的网络状态:" << std::endl;
        mapNetwork->displayNetworkInfo();
        
        pauseForScreenshot();
        
        // 测试创建默认网络
        printSubTest("创建默认网络");
        std::cout << "创建默认网络..." << std::endl;
        result = mapNetwork->createDefaultNetwork();
        std::cout << "创建结果: " << (result ? "成功" : "失败") << std::endl;
        
        std::cout << "默认网络状态:" << std::endl;
        mapNetwork->displayNetworkInfo();
        mapNetwork->displayAllCities();
        
        pauseForScreenshot();
    }
    
    void testEdgeCases() {
        printTestHeader("边界情况测试");
        
        // 测试空图操作
        printSubTest("空图操作");
        MapNetwork emptyNetwork(true);
        
        std::cout << "在空图中查找城市..." << std::endl;
        City* city = emptyNetwork.findCity(1);
        std::cout << "查找结果: " << (city ? "找到" : "未找到（预期）") << std::endl;
        
        std::cout << "在空图中查找最短路径..." << std::endl;
        auto result = emptyNetwork.findShortestPath(1, 2);
        std::cout << "查找结果: " << (result.found ? "找到" : "未找到（预期）") << std::endl;
        
        pauseForScreenshot();
        
        // 测试单节点图
        printSubTest("单节点图");
        MapNetwork singleNodeNetwork(true);
        singleNodeNetwork.addCity(City(1, "单节点"));
        
        std::cout << "单节点网络信息:" << std::endl;
        singleNodeNetwork.displayNetworkInfo();
        
        std::cout << "在单节点图中查找路径..." << std::endl;
        result = singleNodeNetwork.findShortestPath(1, 1);
        std::cout << "查找结果: " << (result.found ? "找到" : "未找到") << std::endl;
        if (result.found) {
            std::cout << "路径长度: " << result.totalDistance << std::endl;
        }
        
        pauseForScreenshot();
        
        // 测试大图性能
        printSubTest("大规模图测试");
        std::cout << "创建包含20个城市的网络..." << std::endl;
        
        MapNetwork largeNetwork(true);
        for (int i = 1; i <= 20; ++i) {
            std::string cityName = "城市" + std::to_string(i);
            largeNetwork.addCity(City(i, cityName));
        }
        
        // 添加随机路线
        for (int i = 1; i <= 30; ++i) {
            int from = (rand() % 20) + 1;
            int to = (rand() % 20) + 1;
            if (from != to) {
                int distance = (rand() % 1000) + 100;
                largeNetwork.addRoute(from, to, distance);
            }
        }
        
        std::cout << "网络创建完成！" << std::endl;
        std::cout << "城市数量: " << largeNetwork.getCityCount() << std::endl;
        std::cout << "路线数量: " << largeNetwork.getRouteCount() << std::endl;
        
        std::cout << "测试最短路径算法性能..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        auto pathResult = largeNetwork.findShortestPath(1, 20);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "计算时间: " << duration.count() << " 毫秒" << std::endl;
        std::cout << "路径查找结果: " << (pathResult.found ? "找到" : "未找到") << std::endl;
        
        pauseForScreenshot();
        
        // 测试图类型切换
        printSubTest("图类型切换");
        std::cout << "当前图类型: " << mapNetwork->getGraphType() << std::endl;
        
        std::cout << "切换到邻接表..." << std::endl;
        mapNetwork->setGraphType(false);
        std::cout << "当前图类型: " << mapNetwork->getGraphType() << std::endl;
        
        std::cout << "网络状态切换后:" << std::endl;
        mapNetwork->displayNetworkInfo();
        
        std::cout << "切换回邻接矩阵..." << std::endl;
        mapNetwork->setGraphType(true);
        std::cout << "当前图类型: " << mapNetwork->getGraphType() << std::endl;
        
        pauseForScreenshot();
    }
};

int main() {
    TestProgram tester;
    tester.runAllTests();
    return 0;
}