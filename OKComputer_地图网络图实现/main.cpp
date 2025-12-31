// main.cpp - 地图网络图分析系统主程序
#include <iostream>
#include "MapNetwork.h"
#include "UserManager.h"
#include "MenuSystem.h"
#include <exception>

int main() {
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "   地图网络图分析系统" << std::endl;
        std::cout << "   Map Network Graph Analysis System" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\n正在初始化系统..." << std::endl;
        
        // 创建系统组件
        MapNetwork mapNetwork(true);  // 默认使用邻接矩阵
        UserManager userManager("users.txt");
        MenuSystem menuSystem(mapNetwork, userManager);
        
        std::cout << "系统初始化完成！" << std::endl;
        std::cout << "\n功能说明：" << std::endl;
        std::cout << "- 用户管理系统：支持用户注册、登录、密码管理" << std::endl;
        std::cout << "- 地图网络管理：支持城市和路线的增删改查" << std::endl;
        std::cout << "- 图算法实现：DFS、BFS遍历和Dijkstra最短路径" << std::endl;
        std::cout << "- 双图结构：支持邻接矩阵和邻接表切换" << std::endl;
        std::cout << "- 数据持久化：自动保存到文件" << std::endl;
        
        std::cout << "\n按Enter键开始...";
        std::cin.get();
        
        // 加载默认数据
        mapNetwork.createDefaultNetwork();
        std::cout << "\n已加载默认网络数据（8个城市，8条路线）" << std::endl;
        
        // 启动菜单系统
        menuSystem.start();
        
        std::cout << "\n感谢使用地图网络图分析系统！" << std::endl;
        std::cout << "再见！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "发生未知错误！" << std::endl;
        return 1;
    }
    
    return 0;
}