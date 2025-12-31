// Graph.cpp - 图基类及其实现类定义
#include "Graph.h"
#include <iostream>
#include <unordered_set>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <map>
#include <set>

// ==================== Graph 基类实现 ====================

// 构造函数
Graph::Graph(bool isDirected) : directed(isDirected) {}

// 获取顶点数量
int Graph::getVertexCount() const {
    return cities.size();
}

// 检查顶点是否存在
bool Graph::hasVertex(int cityId) const {
    for (const auto& city : cities) {
        if (city.getId() == cityId) {
            return true;
        }
    }
    return false;
}

// 检查顶点是否存在（根据名称）
bool Graph::hasVertex(const std::string& cityName) const {
    for (const auto& city : cities) {
        if (city.getName() == cityName) {
            return true;
        }
    }
    return false;
}

// 获取顶点
City* Graph::getVertex(int cityId) {
    for (auto& city : cities) {
        if (city.getId() == cityId) {
            return &city;
        }
    }
    return nullptr;
}

// 获取顶点（根据名称）
City* Graph::getVertex(const std::string& cityName) {
    for (auto& city : cities) {
        if (city.getName() == cityName) {
            return &city;
        }
    }
    return nullptr;
}

// 获取所有顶点ID
std::vector<int> Graph::getAllVertexIds() const {
    std::vector<int> ids;
    for (const auto& city : cities) {
        ids.push_back(city.getId());
    }
    return ids;
}

// 深度优先搜索
std::vector<int> Graph::dfs(int startId) const {
    std::vector<int> result;
    if (!hasVertex(startId)) {
        return result;
    }
    
    std::unordered_set<int> visited;
    std::stack<int> stack;
    
    stack.push(startId);
    visited.insert(startId);
    
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        result.push_back(current);
        
        // 获取所有邻居并按逆序压入栈中
        auto neighbors = getNeighbors(current);
        std::reverse(neighbors.begin(), neighbors.end());
        
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                stack.push(neighbor);
            }
        }
    }
    
    return result;
}

// 广度优先搜索
std::vector<int> Graph::bfs(int startId) const {
    std::vector<int> result;
    if (!hasVertex(startId)) {
        return result;
    }
    
    std::unordered_set<int> visited;
    std::queue<int> queue;
    
    queue.push(startId);
    visited.insert(startId);
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        result.push_back(current);
        
        auto neighbors = getNeighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }
    
    return result;
}

// Dijkstra最短路径算法
std::pair<std::vector<int>, int> Graph::dijkstra(int startId, int endId) const {
    std::vector<int> path;
    int distance = 0;
    
    if (!hasVertex(startId) || !hasVertex(endId)) {
        return {path, -1};
    }
    
    if (startId == endId) {
        return {{startId}, 0};
    }
    
    // 距离数组
    std::map<int, int> distances;
    // 前驱节点数组
    std::map<int, int> predecessors;
    // 访问标记
    std::set<int> visited;
    // 优先队列：(距离, 节点ID)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    
    // 初始化
    auto allIds = getAllVertexIds();
    for (int id : allIds) {
        distances[id] = INT_MAX;
    }
    distances[startId] = 0;
    pq.push({0, startId});
    
    while (!pq.empty()) {
        int current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();
        
        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);
        
        // 如果到达目标节点
        if (current == endId) {
            break;
        }
        
        // 遍历所有邻居
        auto neighbors = getNeighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) != visited.end()) {
                continue;
            }
            
            int edgeWeight = getEdgeWeight(current, neighbor);
            if (edgeWeight == -1) continue;  // 无边
            
            int newDist = distances[current] + edgeWeight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                predecessors[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }
    
    // 构建路径
    if (distances[endId] == INT_MAX) {
        return {path, -1};  // 无路径
    }
    
    // 从终点回溯到起点
    int current = endId;
    while (current != startId) {
        path.push_back(current);
        current = predecessors[current];
    }
    path.push_back(startId);
    
    // 反转路径
    std::reverse(path.begin(), path.end());
    
    return {path, distances[endId]};
}

// 获取城市名称
std::string Graph::getCityName(int cityId) const {
    for (const auto& city : cities) {
        if (city.getId() == cityId) {
            return city.getName();
        }
    }
    return "";
}

// ==================== AdjacencyMatrix 实现 ====================

// 构造函数
AdjacencyMatrix::AdjacencyMatrix(bool isDirected) : Graph(isDirected), INF(INT_MAX) {}

// 扩展矩阵大小
void AdjacencyMatrix::resizeMatrix(int newSize) {
    int oldSize = matrix.size();
    if (newSize > oldSize) {
        matrix.resize(newSize);
        for (int i = 0; i < newSize; ++i) {
            matrix[i].resize(newSize, INF);
        }
        // 初始化对角线
        for (int i = oldSize; i < newSize; ++i) {
            matrix[i][i] = 0;
        }
    }
}

// 添加顶点
bool AdjacencyMatrix::addVertex(const City& city) {
    if (hasVertex(city.getId())) {
        return false;  // 顶点已存在
    }
    
    cities.push_back(city);
    resizeMatrix(city.getId() + 1);
    return true;
}

// 删除顶点
bool AdjacencyMatrix::removeVertex(int cityId) {
    if (!hasVertex(cityId)) {
        return false;
    }
    
    // 从城市列表中删除
    cities.erase(
        std::remove_if(cities.begin(), cities.end(),
            [cityId](const City& city) { return city.getId() == cityId; }),
        cities.end()
    );
    
    // 注意：这里简化处理，实际应该收缩矩阵
    // 但为了保持ID一致性，我们保留矩阵大小
    return true;
}

// 添加边
bool AdjacencyMatrix::addEdge(int fromId, int toId, int weight) {
    if (!hasVertex(fromId) || !hasVertex(toId)) {
        return false;
    }
    
    resizeMatrix(std::max(fromId, toId) + 1);
    matrix[fromId][toId] = weight;
    
    if (!directed) {
        matrix[toId][fromId] = weight;
    }
    
    return true;
}

// 删除边
bool AdjacencyMatrix::removeEdge(int fromId, int toId) {
    if (!hasVertex(fromId) || !hasVertex(toId)) {
        return false;
    }
    
    matrix[fromId][toId] = INF;
    if (!directed) {
        matrix[toId][fromId] = INF;
    }
    
    return true;
}

// 检查边是否存在
bool AdjacencyMatrix::hasEdge(int fromId, int toId) const {
    if (fromId >= matrix.size() || toId >= matrix.size()) {
        return false;
    }
    return matrix[fromId][toId] != INF;
}

// 获取边权重
int AdjacencyMatrix::getEdgeWeight(int fromId, int toId) const {
    if (fromId >= matrix.size() || toId >= matrix.size()) {
        return -1;
    }
    if (matrix[fromId][toId] == INF) {
        return -1;
    }
    return matrix[fromId][toId];
}

// 获取邻居节点
std::vector<int> AdjacencyMatrix::getNeighbors(int cityId) const {
    std::vector<int> neighbors;
    if (cityId >= matrix.size()) {
        return neighbors;
    }
    
    for (int i = 0; i < matrix[cityId].size(); ++i) {
        if (matrix[cityId][i] != INF && matrix[cityId][i] != 0) {
            neighbors.push_back(i);
        }
    }
    
    return neighbors;
}

// 显示邻接矩阵
void AdjacencyMatrix::display() const {
    std::cout << "\n=== 邻接矩阵 ===" << std::endl;
    
    // 显示列标题
    std::cout << std::setw(4) << " ";
    for (int i = 0; i < matrix.size(); ++i) {
        std::cout << std::setw(6) << i;
    }
    std::cout << std::endl;
    
    // 显示矩阵
    for (int i = 0; i < matrix.size(); ++i) {
        std::cout << std::setw(4) << i;
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == INF) {
                std::cout << std::setw(6) << "INF";
            } else {
                std::cout << std::setw(6) << matrix[i][j];
            }
        }
        std::cout << std::endl;
    }
}

// 清空图
void AdjacencyMatrix::clear() {
    cities.clear();
    matrix.clear();
}

// ==================== AdjacencyList 实现 ====================

// 构造函数
AdjacencyList::AdjacencyList(bool isDirected) : Graph(isDirected) {}

// 添加顶点
bool AdjacencyList::addVertex(const City& city) {
    if (hasVertex(city.getId())) {
        return false;
    }
    
    cities.push_back(city);
    // 确保邻接表中有这个城市的条目
    if (adjList.find(city.getId()) == adjList.end()) {
        adjList[city.getId()] = std::vector<std::pair<int, int>>();
    }
    return true;
}

// 删除顶点
bool AdjacencyList::removeVertex(int cityId) {
    if (!hasVertex(cityId)) {
        return false;
    }
    
    // 从城市列表中删除
    cities.erase(
        std::remove_if(cities.begin(), cities.end(),
            [cityId](const City& city) { return city.getId() == cityId; }),
        cities.end()
    );
    
    // 从邻接表中删除
    adjList.erase(cityId);
    
    // 删除所有指向该顶点的边
    for (auto& pair : adjList) {
        auto& neighbors = pair.second;
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                [cityId](const std::pair<int, int>& edge) { return edge.first == cityId; }),
            neighbors.end()
        );
    }
    
    return true;
}

// 添加边
bool AdjacencyList::addEdge(int fromId, int toId, int weight) {
    if (!hasVertex(fromId) || !hasVertex(toId)) {
        return false;
    }
    
    // 添加到fromId的邻接表
    adjList[fromId].push_back({toId, weight});
    
    if (!directed) {
        // 添加到toId的邻接表（无向图）
        adjList[toId].push_back({fromId, weight});
    }
    
    return true;
}

// 删除边
bool AdjacencyList::removeEdge(int fromId, int toId) {
    if (!hasVertex(fromId) || !hasVertex(toId)) {
        return false;
    }
    
    // 从fromId的邻接表中删除
    auto& fromNeighbors = adjList[fromId];
    fromNeighbors.erase(
        std::remove_if(fromNeighbors.begin(), fromNeighbors.end(),
            [toId](const std::pair<int, int>& edge) { return edge.first == toId; }),
        fromNeighbors.end()
    );
    
    if (!directed) {
        // 从toId的邻接表中删除（无向图）
        auto& toNeighbors = adjList[toId];
        toNeighbors.erase(
            std::remove_if(toNeighbors.begin(), toNeighbors.end(),
                [fromId](const std::pair<int, int>& edge) { return edge.first == fromId; }),
            toNeighbors.end()
        );
    }
    
    return true;
}

// 检查边是否存在
bool AdjacencyList::hasEdge(int fromId, int toId) const {
    auto it = adjList.find(fromId);
    if (it == adjList.end()) {
        return false;
    }
    
    for (const auto& edge : it->second) {
        if (edge.first == toId) {
            return true;
        }
    }
    return false;
}

// 获取边权重
int AdjacencyList::getEdgeWeight(int fromId, int toId) const {
    auto it = adjList.find(fromId);
    if (it == adjList.end()) {
        return -1;
    }
    
    for (const auto& edge : it->second) {
        if (edge.first == toId) {
            return edge.second;
        }
    }
    return -1;
}

// 获取邻居节点
std::vector<int> AdjacencyList::getNeighbors(int cityId) const {
    std::vector<int> neighbors;
    auto it = adjList.find(cityId);
    
    if (it != adjList.end()) {
        for (const auto& edge : it->second) {
            neighbors.push_back(edge.first);
        }
    }
    
    return neighbors;
}

// 显示邻接表
void AdjacencyList::display() const {
    std::cout << "\n=== 邻接表 ===" << std::endl;
    
    for (const auto& pair : adjList) {
        int cityId = pair.first;
        std::cout << "城市 " << cityId << " (" << getCityName(cityId) << ") -> ";
        
        for (const auto& edge : pair.second) {
            std::cout << "[" << edge.first << "(距离:" << edge.second << ")] ";
        }
        std::cout << std::endl;
    }
}

// 清空图
void AdjacencyList::clear() {
    cities.clear();
    adjList.clear();
}