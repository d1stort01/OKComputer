// MapNetwork.cpp - 地图网络管理类实现
#include "MapNetwork.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

// 构造函数
MapNetwork::MapNetwork(bool useMatrix, const std::string& cityFile, const std::string& routeFile) 
    : fileManager(cityFile, routeFile), useAdjacencyMatrix(useMatrix) {
    
    // 根据选择创建图对象
    if (useAdjacencyMatrix) {
        graph = std::make_unique<AdjacencyMatrix>(false);  // 无向图
    } else {
        graph = std::make_unique<AdjacencyList>(false);    // 无向图
    }
}

// 设置图类型
void MapNetwork::setGraphType(bool useMatrix) {
    if (useMatrix != useAdjacencyMatrix) {
        // 保存当前数据
        std::vector<City> cities = graph->getAllVertexIds().empty() ? 
            std::vector<City>() : 
            std::vector<City>(graph->getAllVertexIds().begin(), graph->getAllVertexIds().end());
        
        // 重新创建图对象
        if (useMatrix) {
            graph = std::make_unique<AdjacencyMatrix>(false);
        } else {
            graph = std::make_unique<AdjacencyList>(false);
        }
        
        useAdjacencyMatrix = useMatrix;
        
        // 恢复数据
        loadData();
    }
}

// 添加城市
bool MapNetwork::addCity(const City& city) {
    return graph->addVertex(city);
}

// 删除城市（根据ID）
bool MapNetwork::removeCity(int cityId) {
    return graph->removeVertex(cityId);
}

// 删除城市（根据名称）
bool MapNetwork::removeCity(const std::string& cityName) {
    City* city = findCity(cityName);
    if (city) {
        return removeCity(city->getId());
    }
    return false;
}

// 查找城市（根据ID）
City* MapNetwork::findCity(int cityId) {
    return graph->getVertex(cityId);
}

// 查找城市（根据名称）
City* MapNetwork::findCity(const std::string& cityName) {
    return graph->getVertex(cityName);
}

// 显示所有城市
void MapNetwork::displayAllCities() const {
    std::cout << "\n=== 所有城市 ===" << std::endl;
    std::cout << std::setw(8) << "城市ID" << std::setw(15) << "城市名称" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    auto cityIds = graph->getAllVertexIds();
    for (int cityId : cityIds) {
        std::string cityName = graph->getCityName(cityId);
        std::cout << std::setw(8) << cityId << std::setw(15) << cityName << std::endl;
    }
    
    std::cout << "总计: " << cityIds.size() << " 个城市" << std::endl;
}

// 获取所有城市ID
std::vector<int> MapNetwork::getAllCityIds() const {
    return graph->getAllVertexIds();
}

// 添加路线（根据城市ID）
bool MapNetwork::addRoute(int fromCityId, int toCityId, int distance) {
    return graph->addEdge(fromCityId, toCityId, distance);
}

// 添加路线（根据城市名称）
bool MapNetwork::addRoute(const std::string& fromCity, const std::string& toCity, int distance) {
    City* from = findCity(fromCity);
    City* to = findCity(toCity);
    
    if (from && to) {
        return addRoute(from->getId(), to->getId(), distance);
    }
    return false;
}

// 删除路线（根据城市ID）
bool MapNetwork::removeRoute(int fromCityId, int toCityId) {
    return graph->removeEdge(fromCityId, toCityId);
}

// 删除路线（根据城市名称）
bool MapNetwork::removeRoute(const std::string& fromCity, const std::string& toCity) {
    City* from = findCity(fromCity);
    City* to = findCity(toCity);
    
    if (from && to) {
        return removeRoute(from->getId(), to->getId());
    }
    return false;
}

// 检查路线是否存在
bool MapNetwork::hasRoute(int fromCityId, int toCityId) const {
    return graph->hasEdge(fromCityId, toCityId);
}

// 获取路线距离
int MapNetwork::getRouteDistance(int fromCityId, int toCityId) const {
    return graph->getEdgeWeight(fromCityId, toCityId);
}

// 获取邻近城市（根据城市ID）
std::vector<int> MapNetwork::getNeighboringCities(int cityId) const {
    return graph->getNeighbors(cityId);
}

// 获取邻近城市（根据城市名称）
std::vector<int> MapNetwork::getNeighboringCities(const std::string& cityName) const {
    City* city = const_cast<MapNetwork*>(this)->findCity(cityName);
    if (city) {
        return getNeighboringCities(city->getId());
    }
    return std::vector<int>();
}

// 深度优先遍历（根据城市ID）
std::vector<int> MapNetwork::traverseDFS(int startCityId) {
    return graph->dfs(startCityId);
}

// 深度优先遍历（根据城市名称）
std::vector<int> MapNetwork::traverseDFS(const std::string& startCity) {
    City* city = findCity(startCity);
    if (city) {
        return traverseDFS(city->getId());
    }
    return std::vector<int>();
}

// 广度优先遍历（根据城市ID）
std::vector<int> MapNetwork::traverseBFS(int startCityId) {
    return graph->bfs(startCityId);
}

// 广度优先遍历（根据城市名称）
std::vector<int> MapNetwork::traverseBFS(const std::string& startCity) {
    City* city = findCity(startCity);
    if (city) {
        return traverseBFS(city->getId());
    }
    return std::vector<int>();
}

// 查找最短路径（根据城市ID）
PathResult MapNetwork::findShortestPath(int fromCityId, int toCityId) {
    auto result = graph->dijkstra(fromCityId, toCityId);
    
    PathResult pathResult;
    pathResult.path = result.first;
    pathResult.totalDistance = result.second;
    pathResult.found = (result.second != -1);
    
    return pathResult;
}

// 查找最短路径（根据城市名称）
PathResult MapNetwork::findShortestPath(const std::string& fromCity, const std::string& toCity) {
    City* from = findCity(fromCity);
    City* to = findCity(toCity);
    
    if (from && to) {
        return findShortestPath(from->getId(), to->getId());
    }
    
    PathResult result;
    result.found = false;
    return result;
}

// 获取城市数量
int MapNetwork::getCityCount() const {
    return graph->getVertexCount();
}

// 获取路线数量
int MapNetwork::getRouteCount() const {
    int count = 0;
    auto cityIds = graph->getAllVertexIds();
    
    for (size_t i = 0; i < cityIds.size(); ++i) {
        for (size_t j = i + 1; j < cityIds.size(); ++j) {
            if (graph->hasEdge(cityIds[i], cityIds[j])) {
                count++;
            }
        }
    }
    
    return count;
}

// 检查网络是否为空
bool MapNetwork::isNetworkEmpty() const {
    return graph->getVertexCount() == 0;
}

// 保存数据
bool MapNetwork::saveData() const {
    // 获取所有城市
    std::vector<City> cities;
    auto cityIds = graph->getAllVertexIds();
    for (int cityId : cityIds) {
        cities.emplace_back(cityId, graph->getCityName(cityId));
    }
    
    // 保存城市数据
    if (!fileManager.saveCities(cities)) {
        return false;
    }
    
    // 保存路线数据
    if (!fileManager.saveRoutes(*graph)) {
        return false;
    }
    
    return true;
}

// 加载数据
bool MapNetwork::loadData() {
    // 加载城市数据
    std::vector<City> cities;
    if (fileManager.loadCities(cities)) {
        for (const auto& city : cities) {
            graph->addVertex(city);
        }
    }
    
    // 加载路线数据
    fileManager.loadRoutes(*graph);
    
    return true;
}

// 创建默认网络
bool MapNetwork::createDefaultNetwork() {
    clearNetwork();
    
    // 添加默认城市
    std::vector<City> defaultCities = {
        City(1, "北京"),
        City(2, "上海"),
        City(3, "广州"),
        City(4, "深圳"),
        City(5, "杭州"),
        City(6, "南京"),
        City(7, "武汉"),
        City(8, "成都")
    };
    
    for (const auto& city : defaultCities) {
        graph->addVertex(city);
    }
    
    // 添加默认路线
    addRoute(1, 2, 1200);  // 北京-上海
    addRoute(1, 6, 1000);  // 北京-南京
    addRoute(2, 5, 200);   // 上海-杭州
    addRoute(3, 4, 150);   // 广州-深圳
    addRoute(2, 7, 800);   // 上海-武汉
    addRoute(5, 3, 1200);  // 杭州-广州
    addRoute(7, 8, 1100);  // 武汉-成都
    addRoute(6, 7, 500);   // 南京-武汉
    
    return true;
}

// 清空网络
bool MapNetwork::clearNetwork() {
    graph->clear();
    return true;
}

// 显示网络信息
void MapNetwork::displayNetworkInfo() const {
    std::cout << "\n=== 地图网络信息 ===" << std::endl;
    std::cout << "图类型: " << getGraphType() << std::endl;
    std::cout << "城市数量: " << getCityCount() << std::endl;
    std::cout << "路线数量: " << getRouteCount() << std::endl;
    std::cout << "网络状态: " << (isNetworkEmpty() ? "空" : "已加载数据") << std::endl;
}

// 显示路线矩阵
void MapNetwork::displayRouteMatrix() const {
    graph->display();
}

// 获取图类型
std::string MapNetwork::getGraphType() const {
    return useAdjacencyMatrix ? "邻接矩阵" : "邻接表";
}

// 检查是否使用邻接矩阵
bool MapNetwork::isUsingAdjacencyMatrix() const {
    return useAdjacencyMatrix;
}