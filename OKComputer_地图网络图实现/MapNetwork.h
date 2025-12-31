// MapNetwork.h - 地图网络管理类声明
#ifndef MAPNETWORK_H
#define MAPNETWORK_H

#include "Graph.h"
#include "City.h"
#include "FileManager.h"
#include <memory>
#include <vector>

// 地图网络管理类
class MapNetwork {
private:
    std::unique_ptr<Graph> graph;           // 图数据（可以是邻接矩阵或邻接表）
    FileManager fileManager;                // 文件管理器
    bool useAdjacencyMatrix;                // 是否使用邻接矩阵
    
public:
    // 构造函数
    MapNetwork(bool useMatrix = true, const std::string& cityFile = "cities.txt", 
               const std::string& routeFile = "routes.txt");
    
    // 设置图类型
    void setGraphType(bool useMatrix);
    
    // 城市管理功能
    bool addCity(const City& city);
    bool removeCity(int cityId);
    bool removeCity(const std::string& cityName);
    City* findCity(int cityId);
    City* findCity(const std::string& cityName);
    void displayAllCities() const;
    std::vector<int> getAllCityIds() const;
    
    // 路线管理功能
    bool addRoute(int fromCityId, int toCityId, int distance);
    bool addRoute(const std::string& fromCity, const std::string& toCity, int distance);
    bool removeRoute(int fromCityId, int toCityId);
    bool removeRoute(const std::string& fromCity, const std::string& toCity);
    bool hasRoute(int fromCityId, int toCityId) const;
    int getRouteDistance(int fromCityId, int toCityId) const;
    
    // 邻近节点查询
    std::vector<int> getNeighboringCities(int cityId) const;
    std::vector<int> getNeighboringCities(const std::string& cityName) const;
    
    // 图遍历功能
    std::vector<int> traverseDFS(int startCityId);
    std::vector<int> traverseDFS(const std::string& startCity);
    std::vector<int> traverseBFS(int startCityId);
    std::vector<int> traverseBFS(const std::string& startCity);
    
    // 导航功能
    PathResult findShortestPath(int fromCityId, int toCityId);
    PathResult findShortestPath(const std::string& fromCity, const std::string& toCity);
    
    // 网络统计信息
    int getCityCount() const;
    int getRouteCount() const;
    bool isNetworkEmpty() const;
    
    // 数据持久化
    bool saveData() const;
    bool loadData();
    bool createDefaultNetwork();
    bool clearNetwork();
    
    // 显示网络信息
    void displayNetworkInfo() const;
    void displayRouteMatrix() const;
    
    // 图类型信息
    std::string getGraphType() const;
    bool isUsingAdjacencyMatrix() const;
};

#endif // MAPNETWORK_H